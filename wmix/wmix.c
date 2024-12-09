/* WMix 3.0 -- a mixer using the OSS mixer API.
 * Copyright (C) 2000, 2001 timecop@japan.co.jp
 * Mixer code in version 3.0 based on mixer api library by
 * Daniel Richard G. <skunk@mit.edu>, which in turn was based on
 * the mixer code in WMix 2.x releases.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>

#include "include/common.h"
#include "include/mixer.h"
#include "include/misc.h"
#include "include/ui_x.h"
#include "include/mmkeys.h"
#include "include/config.h"
#include "include/mixer-oss.h"
#include "include/mixer-alsa.h"

void (*mixer_init)(const char *mixer_device,
                   bool verbose,
                   const char *exclude[]);
bool (*mixer_is_changed)(void);
int (*mixer_get_channel_count)(void);
int (*mixer_get_channel)(void);
const char *(*mixer_get_channel_name)(void);
const char *(*mixer_get_short_name)(void);
void (*mixer_set_channel)(int channel);
void (*mixer_set_channel_rel)(int delta_channel);
float (*mixer_get_volume)(void);
void (*mixer_set_volume)(float volume);
void (*mixer_set_volume_rel)(float delta_volume);
float (*mixer_get_balance)(void);
void (*mixer_set_balance)(float balance);
void (*mixer_set_balance_rel)(float delta_balance);
void (*mixer_toggle_mute)(void);
void (*mixer_toggle_rec)(void);
bool (*mixer_is_muted)(void);
bool (*mixer_is_stereo)(void);
bool (*mixer_is_rec)(void);
bool (*mixer_can_rec)(void);
bool (*is_exclude)(const char *short_name,
                   const char *exclude[]);
void (*mixer_tick)(void);

static Display *display;
static bool button_pressed = false;
static bool slider_pressed = false;
static double prev_button_press_time = 0.0;

static float display_height;
static float display_width;
static int mouse_drag_home_x;
static int mouse_drag_home_y;
static int idle_loop;

/* local stuff */
static void signal_catch(int sig);
static void button_press_event(XButtonEvent *event);
static void button_release_event(XButtonEvent *event);
static int  key_press_event(XKeyEvent *event);
static void motion_event(XMotionEvent *event);
static void choose_api(int api);


int main(int argc, char **argv)
{
    XEvent event;
    int rr_event_base, rr_error_base;
    Bool have_randr;

    config_init();
    parse_cli_options(argc, argv);
    config_read();
    config_set_defaults();
    choose_api(config.api);

    mixer_init(config.mixer_device, config.verbose, (const char **)config.exclude_channel);
    mixer_set_channel(0);

    display = XOpenDisplay(config.display_name);
    if (display == NULL) {
	const char *name;

	if (config.display_name) {
	    name = config.display_name;
	} else {
	    name = getenv("DISPLAY");
	    if (name == NULL) {
		fprintf(stderr, "wmix:error: Unable to open display, variable $DISPLAY not set\n");
		return EXIT_FAILURE;
	    }
	}
	fprintf(stderr, "wmix:error: Unable to open display \"%s\"\n", name);
	return EXIT_FAILURE;
    }

    have_randr = XRRQueryExtension(display, &rr_event_base, &rr_error_base);
    if (have_randr) {
        int rr_mask = RRScreenChangeNotifyMask;
        XRRSelectInput(display,
                       RootWindow(display, DefaultScreen(display)),
                       rr_mask);
    }

    display_width = (float)DisplayWidth(display, DefaultScreen(display)) / 4.0;
    display_height = (float)DisplayHeight(display, DefaultScreen(display)) / 2.0;

    dockapp_init(display, have_randr);
    new_window("wmix", 64, 64, argc, argv);
    new_osd(60);

    if (config.mmkeys)
	    mmkey_install(display);

    config_release();

    blit_string("wmix " VERSION);
    scroll_text(3, 4, 57, true);
    ui_update();

    /* add click regions */
    add_region(1, 37, 36, 25, 25);	/* knob */
    add_region(2, 4, 42, 27, 15);	/* balancer */
    add_region(3, 2, 26, 7, 10);	/* previous channel */
    add_region(4, 10, 26, 7, 10);	/* next channel */
    add_region(5, 39, 14, 20, 7);	/* mute toggle */
    add_region(6, 4, 14, 13, 7);	/* rec toggle */
    add_region(10, 3, 4, 56, 7);	/* re-scroll current channel name */

    /* setup up/down signal handler */
    create_pid_file();
    signal(SIGUSR1, (void *) signal_catch);
    signal(SIGUSR2, (void *) signal_catch);

    while (true) {
	if (button_pressed || slider_pressed || (XPending(display) > 0)) {
	    XNextEvent(display, &event);
	    switch (event.type) {
		case KeyPress:
		    if (key_press_event(&event.xkey))
			idle_loop = 0;
		    break;
		case Expose:
		    redraw_window();
		    break;
		case ButtonPress:
		    button_press_event(&event.xbutton);
		    idle_loop = 0;
		    break;
		case ButtonRelease:
		    button_release_event(&event.xbutton);
		    idle_loop = 0;
		    break;
		case MotionNotify:
		    /* process cursor change, or drag events */
		    motion_event(&event.xmotion);
		    idle_loop = 0;
		    break;
		case LeaveNotify:
		    /* go back to standard cursor */
		    if ((!button_pressed) && (!slider_pressed))
			set_cursor(NORMAL_CURSOR);
		    break;
		case DestroyNotify:
		    XCloseDisplay(display);
		    return EXIT_SUCCESS;
		default:
                    if (have_randr) {
                        if (event.type == rr_event_base + RRScreenChangeNotify) {
                            XRRUpdateConfiguration(&event);
                            ui_rrnotify();
                        }
                    }
		    break;
	    }
	} else {
	    usleep(100000);
            if (mixer_tick)
                mixer_tick();
	    scroll_text(3, 4, 57, false);
	    /* rescroll message after some delay */
	    if (idle_loop++ > 256) {
		scroll_text(3, 4, 57, true);
		idle_loop = 0;
	    }
	    /* get rid of OSD after a few seconds of idle */
	    if ((idle_loop > 15) && osd_mapped() && !button_pressed) {
		unmap_osd();
		idle_loop = 0;
	    }
	    if (mixer_is_changed())
		ui_update();
	}
    }
    return EXIT_SUCCESS;
}

static void signal_catch(int sig)
{
    switch (sig) {
	case SIGUSR1:
	    mixer_set_volume_rel(config.scrollstep);
	    if (!osd_mapped())
		map_osd();
	    if (osd_mapped())
		update_osd(mixer_get_volume(), false);
	    ui_update();
	    idle_loop = 0;
	    break;
	case SIGUSR2:
	    mixer_set_volume_rel(-config.scrollstep);
	    if (!osd_mapped())
		map_osd();
	    if (osd_mapped())
		update_osd(mixer_get_volume(), false);
	    ui_update();
	    idle_loop = 0;
	    break;
    }
}

static void choose_api(int api)
{
    if (api == 0) {
        mixer_init = &mixer_alsa_init;
        mixer_is_changed = &mixer_alsa_is_changed;
        mixer_get_channel_count = mixer_alsa_get_channel_count;
        mixer_get_channel = mixer_alsa_get_channel;
        mixer_get_channel_name = mixer_alsa_get_channel_name;
        mixer_get_short_name = mixer_alsa_get_short_name;
        mixer_set_channel = mixer_alsa_set_channel;
        mixer_set_channel_rel = mixer_alsa_set_channel_rel;
        mixer_get_volume = mixer_alsa_get_volume;
        mixer_set_volume = mixer_alsa_set_volume;
        mixer_set_volume_rel = mixer_alsa_set_volume_rel;
        mixer_get_balance = mixer_alsa_get_balance;
        mixer_set_balance = mixer_alsa_set_balance;
        mixer_set_balance_rel = mixer_alsa_set_balance_rel;
        mixer_toggle_mute = mixer_alsa_toggle_mute;
        mixer_toggle_rec = mixer_alsa_toggle_rec;
        mixer_is_muted = mixer_alsa_is_muted;
        mixer_is_stereo = mixer_alsa_is_stereo;
        mixer_is_rec = mixer_alsa_is_rec;
        mixer_can_rec = mixer_alsa_can_rec;
        mixer_tick = mixer_alsa_tick;
    } else if (api == 1) {
        mixer_init = &mixer_oss_init;
        mixer_is_changed = &mixer_oss_is_changed;
        mixer_get_channel_count = mixer_oss_get_channel_count;
        mixer_get_channel = mixer_oss_get_channel;
        mixer_get_channel_name = mixer_oss_get_channel_name;
        mixer_get_short_name = mixer_oss_get_short_name;
        mixer_set_channel = mixer_oss_set_channel;
        mixer_set_channel_rel = mixer_oss_set_channel_rel;
        mixer_get_volume = mixer_oss_get_volume;
        mixer_set_volume = mixer_oss_set_volume;
        mixer_set_volume_rel = mixer_oss_set_volume_rel;
        mixer_get_balance = mixer_oss_get_balance;
        mixer_set_balance = mixer_oss_set_balance;
        mixer_set_balance_rel = mixer_oss_set_balance_rel;
        mixer_toggle_mute = mixer_oss_toggle_mute;
        mixer_toggle_rec = mixer_oss_toggle_rec;
        mixer_is_muted = mixer_oss_is_muted;
        mixer_is_stereo = mixer_oss_is_stereo;
        mixer_is_rec = mixer_oss_is_rec;
        mixer_can_rec = mixer_oss_can_rec;
        mixer_tick = NULL;
    }
}

static void button_press_event(XButtonEvent *event)
{
    double button_press_time = get_current_time();
    int x = event->x;
    int y = event->y;
    bool double_click = false;

    /* handle wheel scrolling to adjust volume */
    if (config.mousewheel) {
	if (event->button == config.wheel_button_up) {
	    mixer_set_volume_rel(config.scrollstep);
	    if (!osd_mapped())
		map_osd();
	    if (osd_mapped())
		update_osd(mixer_get_volume(), false);
	    ui_update();
	    return;
	}
	if (event->button == config.wheel_button_down) {
	    mixer_set_volume_rel(-config.scrollstep);
	    if (!osd_mapped())
		map_osd();
	    if (osd_mapped())
		update_osd(mixer_get_volume(), false);
	    ui_update();
	    return;
	}
    }

    if ((button_press_time - prev_button_press_time) <= 0.5) {
	double_click = true;
	prev_button_press_time = 0.0;
    } else
	prev_button_press_time = button_press_time;

    switch (check_region(x, y)) {
	case 1:			/* on knob */
	    button_pressed = true;
	    slider_pressed = false;
	    mouse_drag_home_x = x;
	    mouse_drag_home_y = y;
	    if (double_click) {
		mixer_toggle_mute();
		ui_update();
	    }
	    break;
	case 2:			/* on slider */
	    button_pressed = false;
	    slider_pressed = true;
	    mouse_drag_home_x = x;
	    mouse_drag_home_y = y;
	    if (double_click) {
		mixer_set_balance(0.0);
		ui_update();
	    }
	    break;
	case 3:			/* previous channel */
	    mixer_set_channel_rel(-1);
	    blit_string(config.scrolltext ? mixer_get_channel_name() : mixer_get_short_name());
	    scroll_text(3, 4, 57, true);
	    unmap_osd();
	    map_osd();
	    ui_update();
	    break;
	case 4:			/* next channel */
	    mixer_set_channel_rel(1);
	    blit_string(config.scrolltext ? mixer_get_channel_name() : mixer_get_short_name());
	    scroll_text(3, 4, 57, true);
	    unmap_osd();
	    map_osd();
	    ui_update();
	    break;
	case 5:			/* toggle mute */
	    mixer_toggle_mute();
	    ui_update();
	    break;
	case 6:			/* toggle rec */
	    mixer_toggle_rec();
	    ui_update();
	    break;
	case 10:
	    scroll_text(3, 4, 57, true);
	    break;
	default:
	    printf("unknown region pressed\n");
	    break;
    }
}

static int key_press_event(XKeyEvent *event)
{
	if (event->keycode == mmkeys.raise_volume) {
		mixer_set_volume_rel(config.scrollstep);
		if (!osd_mapped())
			map_osd();
		if (osd_mapped())
			update_osd(mixer_get_volume(), false);
		ui_update();
		return 1;
	}
	if (event->keycode == mmkeys.lower_volume) {
		mixer_set_volume_rel(-config.scrollstep);
		if (!osd_mapped())
			map_osd();
		if (osd_mapped())
			update_osd(mixer_get_volume(), false);
		ui_update();
		return 1;
	}
	if (event->keycode == mmkeys.mute) {
		mixer_toggle_mute();
		ui_update();
		return 1;
	}

	/* Ignore other keys */
	return 0;
}

static void button_release_event(XButtonEvent *event)
{
    int x = event->x;
    int y = event->y;
    int region;

    region = check_region(x, y);

    if (region == 1)
	set_cursor(HAND_CURSOR);

    button_pressed = false;
    slider_pressed = false;
}

static void motion_event(XMotionEvent *event)
{
    int x = event->x;
    int y = event->y;
    int region;

    if ((x == mouse_drag_home_x) && (y == mouse_drag_home_y))
	return;

    region = check_region(x, y);

    if (button_pressed) {
	if (y != mouse_drag_home_y) {
	    float delta;

	    set_cursor(NULL_CURSOR);

	    delta = (float)(mouse_drag_home_y - y) / display_height;
	    knob_turn(delta);
	    if (!osd_mapped())
		map_osd();
	    if (osd_mapped())
		update_osd(mixer_get_volume(), false);
	}
	XWarpPointer(display, None, event->window, x, y, 0, 0,
			mouse_drag_home_x, mouse_drag_home_y);
	return;
    }

    if (slider_pressed) {
	if (x != mouse_drag_home_x) {
	    float delta;

	    set_cursor(NULL_CURSOR);

	    delta = (float)(x - mouse_drag_home_x) / display_width;
	    slider_move(delta);
	}
	XWarpPointer(display, None, event->window, x, y, 0, 0,
			mouse_drag_home_x, mouse_drag_home_y);
	return;
    }

    if (region == 1)
	set_cursor(HAND_CURSOR);
    else if (region == 2)
	set_cursor(BAR_CURSOR);
    else
	set_cursor(NORMAL_CURSOR);
}
