/* WMFishTime - Yet Another Waste of CPU Cycles!
 * Copyright (C) 2001 timecop@japan.co.jp
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
 * You did not receive a copy of the GNU General Public License along with
 * this program; chances are, you already have at least 10 copies of this
 * license somewhere on your system.  If you still want it, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, 
 * MA 02111-1307, USA.
 *
 * History :
 *
 * 13/04/2001 Mail check routine added by bright.eye@web.de (Marcel)
 *
 */

#define _GNU_SOURCE
#define VERSION "1.23"

/* general includes */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>

/* x11 includes */
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xresource.h>

#include "include/master.xpm"
#include "include/sprites.h"
#include "include/fishmon.h"
#include "include/chars.h"

/* *INDENT-OFF* */
/* drawing */
static void draw_cmap_image(void);
static void draw_sprite(int x, int y, int idx);
static void draw_sprite_alpha(int x, int y, int idx, int alpha);
static void draw_string(int dx, int dy, char *string);
static void draw_ascii(int x, int y, char digit);
static void anti_line(int x1, int y1, int x2, int y2, int linewidth, int color);
static void putpixel(int x, int y, float i, int linewidth, int color);

/* stuff */
static void fishmon_update(void);
static void bubble_update(void);
static void fish_update(void);
static void weed_update(void);
static void time_update(void);

/* misc support functions */
static void prepare_backbuffer(int solid);
static void prepare_sprites(void);
static void make_new_fishmon_dockapp(void);
static void parse_options(int argc, char **argv);
static void do_help(void);
static void do_version(void);

/* mail stuff */
static void check_mail(void);

/* global variables */
static FishMonData bm;
static int enable_check_mail = 0;	/* no mail check for default !!!  */
static int new_mail = 0;		/* no new mail for default !!!    */
static int broken_wm = 0;		/* need broken windowmanager fix? */

/* 34 sprites:
 * 0, 2, 4, 6, 8, 10, 12, 14 - fish left
 * 1, 3, 5, 7, 9, 11, 13, 15 - fish right
 * 16, 17, 18, 19, 20 - grass
 * 21, 22, 23, 24, 25 - small fish
 * 26, 27, 28, 29, 30 - bubbles
 * 31 - thermometer and the "liquid"
 */
static Sprite sp[34] = {
/* 00 */    { 17, 14, 0, 0, NULL },		/* left swim fish */
/* 01 */    { 17, 14, 18, 0, NULL },		/* right swim fish */
/* 02 */    { 17, 14, 0, 15, NULL },		/* left swim fish 1 */
/* 03 */    { 17, 14, 18, 15, NULL },		/* right swim fish 1 */
/* 04 */    { 17, 14, 0, 30, NULL },		/* left swim fish 2 */
/* 05 */    { 17, 14, 18, 30, NULL },		/* right swim fish 2 */
/* 06 */    { 17, 14, 0, 45, NULL },		/* left turn begin */
/* 07 */    { 17, 14, 18, 45, NULL },		/* right turn begin */
/* 08 */    { 17, 14, 0, 60, NULL },		/* facing */
/* 09 */    { 17, 14, 18, 60, NULL },		/* facing */
/* 10 */    { 17, 14, 0, 75, NULL },		/* left mouth open */
/* 11 */    { 17, 14, 18, 75, NULL },		/* right mouth open */
/* 12 */    { 17, 14, 0, 90, NULL },		/* left mouth open 1 */
/* 13 */    { 17, 14, 18, 90, NULL },		/* right mouth open 1 */
/* 14 */    { 17, 14, 0, 105, NULL },		/* left mouth open 2 */
/* 15 */    { 17, 14, 18, 105, NULL },		/* right mouth open 2 */

/* 16 */    { 17, 12, 0, 120, NULL },		/* grass 1 */
/* 17 */    { 17, 12, 18, 120, NULL },		/* grass 2 */
/* 18 */    { 17, 12, 0, 135, NULL },		/* grass 3 */
/* 19 */    { 17, 12, 18, 135, NULL },		/* grass 4 */
/* 20 */    { 17, 12, 0, 150, NULL },		/* grass 5 */

/* 21 */    { 11, 8, 0, 180, NULL },		/* small fish huge */
/* 22 */    { 11, 7, 0, 189, NULL },		/* small fish big */
/* 23 */    { 9, 6, 0, 197, NULL },		/* small fish small */
/* 24 */    { 7, 4, 10, 197, NULL },		/* small fish little */
/* 25 */    { 7, 3, 10, 202, NULL },		/* small fish tiny */

/* 26 */    { 5, 5, 19, 196, NULL },		/* bubble huge */
/* 27 */    { 5, 3, 19, 204, NULL },		/* bubble big */
/* 28 */    { 3, 3, 27, 196, NULL },		/* bubble small */
/* 29 */    { 2, 2, 27, 202, NULL },		/* bubble little */
/* 30 */    { 1, 1, 33, 196, NULL },		/* bubble tiny */

/* 31 */    { 6, 35, 26, 151, NULL },		/* thermometer case */

            { 0, 0, 0, 0, NULL }		/* terminator */
};

/* animation sequence loop for fish [4 frames, loop] */
static int fish_animation[4] = { 0, 2, 4, 2 };
/* animation sequence loop for weeds [8 frames, loop] */
static int weed_animation[8] = { 16, 17, 18, 19, 20, 19, 18, 17 };
/* animation sequence loop for turning fish [4 frames, no loop] */
static int turn_animation[8] = { 6, 8, 7, 1,    7, 9, 6, 0 };
/* "x offset" sequence for bubbles moving upward */
static int bubble_sequence[5] = { -2, -2, -1, 0, 0 };
/* keeps track of mouse focus */
static int proximity;
/* keeps track of distance between bubble state changes (depends on YMAX) */
static int bubble_state_change = 0;
/* day of week */
static char weekday[8][3] = { "SU", "MO", "TU", "WE", "TH", "FR", "SA" };
/* month */
static char month[12][4] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
    			   "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
/* *INDENT-ON* */

int main(int argc, char **argv)
{
    int ch;
    GdkEvent *event;
#ifdef PRO
    int frame = 0;
    int timer = 0;
#endif

    srand(time(NULL));

    /* initialize GDK */
    if (!gdk_init_check(&argc, &argv)) {
	fprintf(stderr,
		"GDK init failed, bye bye.  Check \"DISPLAY\" variable.\n");
	exit(-1);
    }
    gdk_rgb_init();

    /* parse command line options */
    parse_options(argc, argv);

    /* zero main data structure */
    memset(&bm, 0, sizeof(bm));

    bubble_state_change = (YMAX / 4);

    /* create dockapp window. creates windows, allocates memory, etc */
    make_new_fishmon_dockapp();

    /* update sprite array with sprite data */
    prepare_sprites();

    /* draw water "fade" and save it to backbuffer, and draw some other
     * static stuff */
    prepare_backbuffer(0);

    while (1) {
	while (gdk_events_pending()) {
	    event = gdk_event_get();
	    if (event) {
		switch (event->type) {
		case GDK_DESTROY:
		case GDK_DELETE:
		    gdk_exit(0);
		    exit(0);
		    break;
		case GDK_BUTTON_PRESS:
		    /* printf("button press\n"); */
		    break;
		case GDK_ENTER_NOTIFY:
		    proximity = 1;
		    for (ch = 0; ch < NRFISH; ch++)
			bm.fishes[ch].speed += (rand() % 2) + 1;
		    break;
		case GDK_LEAVE_NOTIFY:
		    proximity = 0;
		    /* get fish moving again, but slowly - checking out */
		    for (ch = 0; ch < NRFISH; ch++)
			bm.fishes[ch].speed = 1;
		    break;
		default:
		    break;
		}
		gdk_event_free(event);
	    }
	}

#ifdef PRO
	/* calculate FPS */
	if (timer != time(NULL)) {
	    timer = time(NULL);
	    printf("fps: %d\n", frame);
	    frame = 0;
	}
	frame++;
#endif

	usleep(20000);
	fishmon_update();

	/* actually draw the rgb buffer to screen */
	if (!broken_wm)
	    gdk_draw_rgb_image(bm.iconwin, bm.gc, 4, 4, 56, 56,
		    GDK_RGB_DITHER_NONE, bm.rgb, 56 * 3);
	else
	    gdk_draw_rgb_image(bm.win, bm.gc, 4, 4, 56, 56,
		    GDK_RGB_DITHER_NONE, bm.rgb, 56 * 3);
    }
    return 0;
}				/* main */

/* update fish, bubbles, temperature, etc */
static void fishmon_update(void)
{
    memcpy(&bm.rgb, &bm.bgr, RGBSIZE);
    memset(&bm.image, 0, CMAPSIZE);

    /* check for new mail if enabled and no new mail exists */
    if (enable_check_mail) {
	check_mail();
	if (new_mail)
	    weed_update();
    }

    /* update each fish position */
    fish_update();

    /* done with sprites - now draw colormap-based image */
    draw_cmap_image();

    /* draw bubbles - done after everything else because we blend them
     * on top of everything */
    bubble_update();

    /* draw thermometer 80 - alpha
    draw_sprite_alpha(49, 20, 31, 80); */

    /* update clock face */
    time_update();
}				/* fishmon_update */

static void time_update(void)
{
    struct tm *data;
    time_t cur_time;
    static time_t old_time;

    int hr, min, sec;
    static int osec = -1;
    static int oday = -1;
    static int hdx, hdy, mdx, mdy, sdx, sdy;

    double psi;

    cur_time = time(NULL);

    if (cur_time != old_time) {
	old_time = cur_time;

	data = localtime(&cur_time);

	hr = data->tm_hour % 12;
	min = data->tm_min;
	sec = data->tm_sec;

	/* hours */
	if ((sec % 15) == 0) {
	    psi = hr * (M_PI / 6.0);
	    psi += min * (M_PI / 360);
	    hdx = floor(sin(psi) * 26 * 0.55) + 28;
	    hdy = floor(-cos(psi) * 22 * 0.55) + 24;
	}

	/* minutes */
	if ((sec % 15) == 0) {
	    psi = min * (M_PI / 30.0);
	    psi += sec * (M_PI / 1800);
	    mdx = floor(sin(psi) * 26 * 0.7) + 28;
	    mdy = floor(-cos(psi) * 22 * 0.7) + 24;
	}
	
	/* seconds */
	if (osec != sec) {
	    psi = sec * (M_PI / 30.0);
	    sdx = floor(sin(psi) * 26 * 0.9) + 28;
	    sdy = floor(-cos(psi) * 22 * 0.9) + 24;
	    osec = sec;
	}

	/* see if we need to redraw the day/month/weekday deal */
	if (data->tm_mday != oday) {
	    oday = data->tm_mday;
	    /* redundant calculation for a reason */
	    psi = hr * (M_PI / 6.0);
	    psi += min * (M_PI / 360);
	    hdx = floor(sin(psi) * 26 * 0.55) + 28;
	    hdy = floor(-cos(psi) * 22 * 0.55) + 24;
	    psi = min * (M_PI / 30.0);
	    psi += sec * (M_PI / 1800);
	    mdx = floor(sin(psi) * 26 * 0.7) + 28;
	    mdy = floor(-cos(psi) * 22 * 0.7) + 24;

	    /* reflash the backbuffer / date / weekday */
	    prepare_backbuffer(0);
	}
    }

    /* must redraw each frame */
    anti_line(28, 24, hdx, hdy, 1, 0xbf0000);
    anti_line(28, 24, mdx, mdy, 1, 0x00aa00);
    anti_line(28, 24, sdx, sdy, 1, 0xc79f2b);

}

static void weed_update(void)
{
    int i;

    for (i = 0; i < 3; i++) {

	if (bm.weeds[i].delay++ <= 20) {
	    draw_sprite(bm.weeds[i].where, WEEDY,
			weed_animation[bm.weeds[i].frame]);
	    continue;
	}

	/* process frame shifting */
	bm.weeds[i].delay = 0;
	draw_sprite(bm.weeds[i].where, WEEDY,
		    weed_animation[bm.weeds[i].frame]);
	bm.weeds[i].frame++;
	if (bm.weeds[i].frame > 7)
	    bm.weeds[i].frame = 0;
    }
}

static void bubble_update(void)
{
    Bubble *bubbles = bm.bubbles;
    int i, x, y;
    int seq;

    /* make a new bubble, if needed */
    if ((bm.nr_bubbles < MAXBUBBLE) && ((rand() % 101) <= 32)) {
	bm.bubbles[bm.nr_bubbles].x = (rand() % XMAX);
	bm.bubbles[bm.nr_bubbles].y = MAKEY(YMAX);
	bm.bubbles[bm.nr_bubbles].dy = 0;
	bm.nr_bubbles++;
    }

    /* Update and draw the bubbles */
    for (i = 0; i < bm.nr_bubbles; i++) {
	/* Accelerate the bubble */
	bubbles[i].dy -= 64;

	/* Move the bubble vertically */
	bubbles[i].y += bubbles[i].dy;

	/* Did we lose it? */
	if (bubbles[i].y < 0) {
	    /* Yes; nuke it */
	    bubbles[i].x = bubbles[bm.nr_bubbles - 1].x;
	    bubbles[i].y = bubbles[bm.nr_bubbles - 1].y;
	    bubbles[i].dy = bubbles[bm.nr_bubbles - 1].dy;
	    bm.nr_bubbles--;

	    /* We must check the previously last bubble, which is
	     * now the current bubble, also. */
	    i--;
	    continue;
	}
	/* Draw the bubble */
	x = bubbles[i].x;
	y = bubbles[i].y;
	
	/* convert to standard dimension */
	y = REALY(y);
	/* calculate bubble sequence - 0 to 4 (determine bubble sprite idx) */
	seq = y / bubble_state_change;

	/* draw the bubble, using offset-to-center calculated from current
	 * sequence, and make the bubble bigger as we go up. 120 - alpha */
	draw_sprite_alpha(x + bubble_sequence[seq], y, 26 + seq, 120);
    }
}

static void fish_update(void)
{
    int i, j;

    for (i = 0; i < NRFISH; i++) {

	/* frozen fish doesn't need to be handled, or drawn */
	if (bm.fishes[i].speed == 0)
	    continue;

	/* randomly make fish change direction mid-way
	 * but only if it isn't turning already and if it isn't
	 * scared */
	if (((rand() % 255) == 128) && (bm.fishes[i].turn != 1) &&
		!proximity) {
	    bm.fishes[i].turn = 1;
	    bm.fishes[i].frame = 0;
	    bm.fishes[i].speed = 1;
	    bm.fishes[i].delay = 0;
	}

	/* move fish in horizontal direction, left or right */
	if (!bm.fishes[i].rev) {
	    bm.fishes[i].tx -= bm.fishes[i].speed;
	    if (bm.fishes[i].tx < -18 - bm.fishes[i].travel) {
		/* we moved out of bounds. change direction,
		 * position, speed. */
		bm.fishes[i].travel = rand() % 32;
		bm.fishes[i].tx = -18 - bm.fishes[i].travel;
		bm.fishes[i].rev = 1;
		bm.fishes[i].y = rand() % (YMAX - 14);
		bm.fishes[i].speed = proximity ? 0 : (rand() % 2) + 1;
	    }
	} else {
	    bm.fishes[i].tx += bm.fishes[i].speed;
	    if (bm.fishes[i].tx > XMAX + bm.fishes[i].travel) {
		/* we moved out of bounds. change direction,
		 * position, speed. */
		bm.fishes[i].travel = rand() % 32;
		bm.fishes[i].tx = XMAX + bm.fishes[i].travel;
		bm.fishes[i].rev = 0;
		bm.fishes[i].y = rand() % (YMAX - 14);
		bm.fishes[i].speed = proximity ? 0 : (rand() % 2) + 1;
	    }
	}

	/* move fish in vertical position randomly by one pixel up or down */
	j = rand() % 16;
	if (j == 8)
	    bm.fishes[i].y--;
	else if (j == 12)
	    bm.fishes[i].y++;

	/* handle fish currently turning around */
	if (bm.fishes[i].turn) {
	    int turnframe;

	    /* turn_animation array keeps sprite indexes for turning fish
	     * around.  0-3 - ltr turn, 4-7 = rtl turn - rev determines
	     * if we use ltr or rtl parts by multiplying by 4 */
	    turnframe =
		turn_animation[bm.fishes[i].frame +
			       ((bm.fishes[i].rev) * 4)];
	    draw_sprite(bm.fishes[i].tx, bm.fishes[i].y, turnframe);

	    /* because this is a special case, handle updating things here.
	     * notice, delay is lowered (quicker turn), and rev is also
	     * flipped after turn completion */
	    bm.fishes[i].delay += bm.fishes[i].speed;
	    if (bm.fishes[i].delay >= 5) {
		if (++bm.fishes[i].frame >= 4) {
		    bm.fishes[i].frame = 0;
		    bm.fishes[i].rev = !bm.fishes[i].rev;
		    bm.fishes[i].turn = 0;
		    bm.fishes[i].speed = (rand() % 2) + 1;
		}
		bm.fishes[i].delay = 0;
	    }
	    /* get to the next fish */
	    continue;

	} else {
	    /* animate fishes using fish_animation array */
	    draw_sprite(bm.fishes[i].tx, bm.fishes[i].y,
			bm.fishes[i].rev +
			fish_animation[bm.fishes[i].frame]);
	}

	/* switch to next swimming frame */
	bm.fishes[i].delay += bm.fishes[i].speed;
	if (bm.fishes[i].delay >= 10) {
	    if (++bm.fishes[i].frame > 3)
		bm.fishes[i].frame = 0;
	    bm.fishes[i].delay = 0;
	}
    }
}

static void prepare_backbuffer(int solid)
{
    int i;
    int j;
    double psi;
    int dx, dy;
    int idx;
    time_t curr_time;
    struct tm *data;
    char buffer[10];

    /* draw the water -> deep water gradient */
    for (i = 0; i < YMAX; i++) {
	for (j = 0; j < XMAX; j++) {
	    int t = (i * XMAX * 3) + j * 3;
	    if (solid) {
		bm.rgb[t] = 0x00;
		bm.rgb[t + 1] = 0x55;
		bm.rgb[t + 2] = 0xff;
	    } else {
		bm.rgb[t] = 0x00;
		bm.rgb[t + 1] = 0x55 - i;
		bm.rgb[t + 2] = 0xff - i * 2;
	    }
	}
    }

    /* now draw the clock face */
    for (i = 0; i < 60; i += 5) {
	psi = i * (M_PI / 30.0);
	dx = floor(sin(psi) * 25 * 0.9) + 29;
	dy = floor(-cos(psi) * 22 * 0.9) + 24;
	if ((i % 15) == 0) {
	    /* 0/15/30/45 */
	    dx -= 3;
	    dy -= 3;
	    idx = 26;
	} else {
	    /* everything else */
	    dx -= 1;
	    dy -= 1;
	    idx = 28;
	}
	draw_sprite_alpha(dx, dy, idx, 20);
    }

    curr_time = time(NULL);
    data = localtime(&curr_time);

    sprintf(buffer, "%s %02d-%s", weekday[data->tm_wday],
	    data->tm_mday, month[data->tm_mon]);

    draw_string(2, 47, buffer);

    /* copy it to the "frequent use" buffer */
    memcpy(&bm.bgr, &bm.rgb, RGBSIZE);

    
}				/* prepare_backbuffer */

static void copy_sprite_data(int sx, int sy, int w, int h, unsigned char *to)
{
    int i, j;

    for (i = 0; i < h; i++) {
	for (j = 0; j < w; j++) {
	    to[(i * w) + j] = THE_DATA[((sy + i) * HWIDTH) + (sx + j)];
	}
    }
}

static void prepare_sprites(void)
{
    int i = 0;

    i = height;
    i = 0;

    while (sp[i].w) {
	if (sp[i].data)
	    free(sp[i].data);
	sp[i].data = calloc(1, sp[i].w * sp[i].h);
	copy_sprite_data(sp[i].srcx, sp[i].srcy, sp[i].w, sp[i].h,
			 sp[i].data);
	i++;
    }

    for (i = 0; i < NRFISH; i++) {
	bm.fishes[i].y = 32;
	bm.fishes[i].rev = (i % 2) ? 1 : 0;
	bm.fishes[i].tx = rand() % XMAX;
	bm.fishes[i].speed = (rand() % 2) + 1;
    }

    bm.weeds[0].where = -5;
    bm.weeds[0].frame = rand() % 7;

    bm.weeds[1].where = 12;
    bm.weeds[1].frame = rand() % 7;

    bm.weeds[2].where = 42;
    bm.weeds[2].frame = rand() % 7;
}

/* This function makes the dockapp window */
static void make_new_fishmon_dockapp(void)
{
#define MASK GDK_BUTTON_PRESS_MASK | \
    GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK

    GdkWindowAttr attr;
    GdkWindowAttr attri;
    Window win;
    Window iconwin;

    XSizeHints sizehints;
    XWMHints wmhints;

    memset(&attr, 0, sizeof(GdkWindowAttr));

    attr.width = 64;
    attr.height = 64;
    attr.title = "fishmon";
    attr.event_mask = MASK;
    attr.wclass = GDK_INPUT_OUTPUT;
    attr.visual = gdk_visual_get_system();
    attr.colormap = gdk_colormap_get_system();
    attr.wmclass_name = "fishmon";
    attr.wmclass_class = "fishmon";
    attr.window_type = GDK_WINDOW_TOPLEVEL;

    /* make a copy for the iconwin - parameters are the same */
    memcpy(&attri, &attr, sizeof(GdkWindowAttr));

    sizehints.flags = USSize;
    sizehints.width = 64;
    sizehints.height = 64;

    bm.win = gdk_window_new(NULL, &attr,
			    GDK_WA_TITLE | GDK_WA_WMCLASS |
			    GDK_WA_VISUAL | GDK_WA_COLORMAP);
    if (!bm.win) {
	fprintf(stderr, "FATAL: cannot make toplevel window\n");
	exit(1);
    }

    bm.iconwin = gdk_window_new(bm.win, &attri,
				GDK_WA_TITLE | GDK_WA_WMCLASS);
    if (!bm.iconwin) {
	fprintf(stderr, "FATAL: cannot make icon window\n");
	exit(1);
    }

    win = GDK_WINDOW_XWINDOW(bm.win);
    iconwin = GDK_WINDOW_XWINDOW(bm.iconwin);
    XSetWMNormalHints(GDK_WINDOW_XDISPLAY(bm.win), win, &sizehints);

    wmhints.initial_state = WithdrawnState;
    wmhints.icon_window = iconwin;
    wmhints.icon_x = 0;
    wmhints.icon_y = 0;
    wmhints.window_group = win;
    wmhints.flags =
	StateHint | IconWindowHint | IconPositionHint | WindowGroupHint;
    XSetWMHints(GDK_WINDOW_XDISPLAY(bm.win), win, &wmhints);

    bm.gc = gdk_gc_new(bm.win);

    bm.pixmap =
	gdk_pixmap_create_from_xpm_d(bm.win, &(bm.mask), NULL, master_xpm);
    gdk_window_shape_combine_mask(bm.win, bm.mask, 0, 0);
    gdk_window_shape_combine_mask(bm.iconwin, bm.mask, 0, 0);

    gdk_window_set_back_pixmap(bm.win, bm.pixmap, False);
    gdk_window_set_back_pixmap(bm.iconwin, bm.pixmap, False);

    gdk_window_show(bm.win);

#undef MASK
}				/* make_new_fishmon_dockapp */

/********************************************************************
 *                                                                  *
 * Drawing Functions below                                          *
 *                                                                  *
 ********************************************************************/

/* draw XMAX x YMAX colormap image into the main rgb buffer */
static void draw_cmap_image(void)
{
    int i;
    unsigned char cmap;

    for (i = 0; i < CMAPSIZE; i++) {
	if ((cmap = bm.image[i]) != 0) {
	    int pos = i * 3;
	    bm.rgb[pos++] = THE_CMAP[cmap][0];
	    bm.rgb[pos++] = THE_CMAP[cmap][1];
	    bm.rgb[pos] = THE_CMAP[cmap][2];
	}
    }
}

/* draw a sprite into bm.image (palette-based) */
static void draw_sprite(int x, int y, int idx)
{
    /* bounding box of the clipped sprite */
    int dw, di, dh, ds;
    /* loop counters */
    int w, h;
    /* offset into image buffer */
    int pos;

    /* cmap reference for each pixel */
    unsigned char cmap;

    assert(idx >= 0);

    /* completely off the screen, don't bother drawing */
    if ((y < -(sp[idx].h)) || (y > YMAX) ||
	(x > XMAX) || (x < -(sp[idx].w)))
	return;

    /* do clipping for top side */
    ds = 0;
    if (y < 0)
	ds = -(y);

    /* do clipping for bottom side */
    dh = sp[idx].h;
    if ((y + sp[idx].h) > YMAX)
	dh = YMAX - y;

    /* do clipping for right side */
    dw = sp[idx].w;
    if (x > (XMAX - sp[idx].w))
	dw = sp[idx].w - (x - (XMAX - sp[idx].w));

    /* do clipping for left side */
    di = 0;
    if (x < 0)
	di = -(x);

    for (h = ds; h < dh; h++) {
	/* offset to beginning of current row */
	int ypos = (h + y) * XMAX;
	for (w = di; w < dw; w++) {
	    if ((cmap = sp[idx].data[h * sp[idx].w + w]) != 0) {
		pos = ypos + w + x;
		bm.image[pos] = cmap;
	    }
	}
    }
}

/* draw a sprite into bm.rgb with alpha-blend */
static void draw_sprite_alpha(int x, int y, int idx, int alpha)
{
    /* bounding box of the clipped sprite */
    int dw, di, dh, ds;
    /* loop counters */
    int w, h;
    /* offset into rgb buffer */
    int pos;

    /* cmap reference for each pixel */
    unsigned char cmap;

    /* completely off the screen, don't bother drawing */
    if ((y < -(sp[idx].h)) || (y > YMAX) ||
	(x > XMAX) || (x < -(sp[idx].w)))
	return;

    /* do clipping for top side */
    ds = 0;
    if (y < 0)
	ds = -(y);

    /* do clipping for bottom side */
    dh = sp[idx].h;
    if ((y + sp[idx].h) > YMAX)
	dh = YMAX - y;

    /* do clipping for right side */
    dw = sp[idx].w;
    if (x > (XMAX - sp[idx].w))
	dw = sp[idx].w - (x - (XMAX - sp[idx].w));

    /* do clipping for left side */
    di = 0;
    if (x < 0)
	di = -(x);

    for (h = ds; h < dh; h++) {
	/* offset to beginning of current row */
	int ypos = (h + y) * XMAX;
	for (w = di; w < dw; w++) {
	    if ((cmap = sp[idx].data[h * sp[idx].w + w]) != 0) {
		unsigned char r, g, b;
		
		pos = (ypos + w + x) * 3;
		r = THE_CMAP[cmap][0];
		g = THE_CMAP[cmap][1];
		b = THE_CMAP[cmap][2];

		bm.rgb[pos] = (alpha * (int) bm.rgb[pos]
				       + (256 - alpha) * (int) r) >> 8;
		
		bm.rgb[pos + 1] = (alpha * (int) bm.rgb[pos + 1]
			 + (256 - alpha) * (int) g) >> 8;
		
		bm.rgb[pos + 2] = (alpha * (int) bm.rgb[pos + 2]
			 + (256 - alpha) * (int) b) >> 8;
	    }
	}
    }
}

/* draw string using the draw_ascii function below.  Please make sure the
 * output doesn't go outside the dockapp as no checking is performed! */
static void draw_string(int dx, int dy, char *string)
{
    char c = 0;

    while ((c = *string++)) {
	draw_ascii(dx, dy, c);
	if (c != '-')
	    dx += 6;
	else
	    dx += 5;
    }
}

/* draw digits 0..9, letters A..Z, and "-"
 * Clipping not performed!  Must be inside dockapp at all times! */
static void draw_ascii(int dx, int dy, char digit)
{
    char *source = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ- ";
    int idx = strchr(source, digit) - source;
    int x, y;

    /* handle "space" here */
    if (idx == 37)
	return;

    for (y = 0; y < 7; y++) {
	for (x = 0; x < 6; x++) {
	    int src = font_data[(y * 222 + x) + (idx * 6)];
	    if (src) {
		int pos = ((y + dy) * XMAX * 3) + (x + dx) * 3;
		bm.rgb[pos + 0] = font_cmap[src][0];
		bm.rgb[pos + 1] = font_cmap[src][1];
		bm.rgb[pos + 2] = font_cmap[src][2];
	    }
	}
    }
}

/* draw antialiased line from (x1, y1) to (x2, y2), with width linewidth
 * color is an int like 0xRRGGBB */
static void anti_line(int x1, int y1, int x2, int y2, int linewidth, int color)
{
    int dx = abs(x1 - x2); 
    int dy = abs(y1 - y2);
    int error, sign, tmp;
    float ipix;
    int step = linewidth;

    if (dx >= dy) {
	if (x1 > x2) {
	    tmp = x1;
	    x1 = x2;
	    x2 = tmp;
	    tmp = y1;
	    y1 = y2;
	    y2 = tmp;
	}
	error = dx / 2;
	if (y2 > y1)
	    sign = step;
	else
	    sign = -step;

	putpixel(x1, y1, 1, linewidth, color);

	while (x1 < x2) {
	    if ((error -= dy) < 0) {
		y1 += sign;
		error += dx;
	    }
	    x1 += step;
	    ipix = (float)error / dx;

	    if (sign == step)
		ipix = 1 - ipix;

	    putpixel(x1, y1, 1, linewidth, color);
	    putpixel(x1, y1 - step, (1 - ipix), linewidth, color);
	    putpixel(x1, y1 + step, ipix, linewidth, color);
	}
	putpixel(x2, y2, 1, linewidth, color);
    } else {
	if (y1 > y2) {
	    tmp = x1;
	    x1 = x2;
	    x2 = tmp;
	    tmp = y1;
	    y1 = y2;
	    y2 = tmp;
	}
	error = dy / 2;

	if (x2 > x1)
	    sign = step;
	else
	    sign = -step;

	putpixel(x1, y1, 1, linewidth, color);

	while (y1 < y2) {
	    if ((error -= dx) < 0) {
		x1 += sign;
		error += dy;
	    }
	    y1 += step;
	    ipix = (float)error / dy;
	    
	    if (sign == step)
		ipix = 1 - ipix;

	    putpixel(x1 ,y1, 1, linewidth, color);
	    putpixel(x1 - step, y1, (1 - ipix), linewidth, color);
	    putpixel(x1 + step, y1, ipix, linewidth, color);
	}
	putpixel(x2, y2, 1, linewidth, color);
    }
}

/* put alpha-blended pixel on the backbuffer.  Uses floats, could be
 * optimized, probably */
static void putpixel(int x, int y, float i, int linewidth, int color)
{
    int pos = (y * XMAX * 3) + x * 3;

    unsigned char r = ((color >> 16) & 0xff) * i + (bm.rgb[pos]) * (1 - i);
    unsigned char g = ((color >> 8) & 0xff) * i + (bm.rgb[pos + 1]) * (1 - i);
    unsigned char b = (color & 0xff) * i + (bm.rgb[pos + 2]) * (1 - i);

    if (linewidth == 1) {
	bm.rgb[pos] = r;
	bm.rgb[pos + 1] = g;
	bm.rgb[pos + 2] = b;
    } else {
	int dx, dy;
	for (dx = x; dx < x + linewidth; dx++) {
	    for (dy = y; dy < y + linewidth; dy++) {
		pos = (dy * XMAX * 3) + dx * 3;
		bm.rgb[pos] = r;
		bm.rgb[pos + 1] = g;
		bm.rgb[pos + 2] = b;
	    }
	}
    }
}

/* Checks if unread mail exists.
 * Only every 100th call will actually do the check
 * Sets new_mail to true if new mail exists, otherwise false */
static void check_mail(void)
{
    static int timeout = 100;
    static char *mail = NULL;
    struct stat stat_buf;

    /* only the first and every 100th call ... */
    if (timeout++ < 100)
	return;

    timeout = 0;

    if (!mail) {
	mail = getenv("MAIL");
	/* printf("Mail file/dir read : %s\n",mail); */
    }

    /* environment variable MAIL is not set */
    if (!mail)
	return;

    /* Read state */
    if (stat(mail, &stat_buf) < 0) {
	new_mail = 0;
	return;
    }
    
    if ((stat_buf.st_ctime > stat_buf.st_atime) && stat_buf.st_size > 0)
	new_mail = 1; /* we got mail!!! */
    else
	new_mail = 0;
}

static void parse_options(int argc, char **argv)
{
    static int ch = 0;
    static struct option long_opts[] = {
	{ "h",		no_argument, NULL, 1 },
	{ "help",	no_argument, NULL, 1 },
	{ "v",		no_argument, NULL, 2 },
	{ "version",	no_argument, NULL, 2 },
	{ "c",		no_argument, &enable_check_mail, 1 },
	{ "check-mail", no_argument, &enable_check_mail, 1 },
	{ "b",		no_argument, &broken_wm, 1 },
	{ "broken",	no_argument, &broken_wm, 1 },
	{ 0, 0, 0, 0 }
    };

    while ((ch = getopt_long_only(argc, argv, "", long_opts, NULL)) != -1) {
	switch (ch) {
	    case 1:
		do_help();
		exit(0);
		break;
	    case 2:
		do_version();
		exit(0);
		break;
	}
    }
}

static void do_help(void)
{
    printf("Usage: wmfishtime [options]\n\n"
	   " -h\t--help\t\tshow this message and exit\n"
	   " -v\t--version\tshow version and exit\n"
	   " -c\t--check-mail\tenables check for new mail\n"
	   " -b\t--broken\tactivates broken window manager fix\n\n"
	   "Yet Another Waste of CPU Cycles! Dock app clock with\n"
	   "shy fish, bubbles and mail check functionality (disabled by default).\n"
	   "Try out the man page wmfishtime (1x).\n");
}

static void do_version(void)
{
    printf("wmfishtime %s\n", VERSION);
}
