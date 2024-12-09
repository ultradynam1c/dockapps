/*

 wmcube.c 
 Version 0.98 (2000-10-20)

 Robert Kling (robkli-8@student.luth.se)
 http://boombox.campus.luth.se/projects.php

 Contributions:
 	-n option patch by Thorsten Jens (thodi@et-inf.fho-emden.de) (2000-05-12)
 	Various bugfixes and optimizations by Jakob Borg (2000-05-13)
	Solaris Port by Dan Price (dp@rampant.org) (2000-07-16)
	OpenBSD Port by Brian Joseph Czapiga (rys@godsey.net) (2000-07-19)
	FreeBSD Port by Tai-hwa Liang (avatar@mmlab.cse.yzu.edu.tw) (2000-07-20)
	NetBSD Port by Jared Smolens <jsmolens+@andrew.cmu.edu> (2000-09-23)
	gdk gfx, dynamic colors, etc timecop@japan.co.jp (2001-02-13)

 This software is licensed through the GNU General Public Licence.
 
 See http://www.BenSinclair.com/dockapp/ for more wm dock apps.

 If you want to port wmcube to another OS the system specific code is
 sectioned the bottom of this file. See instructions there.

*/

#define _GNU_SOURCE
#define WMCUBE_VERSION "0.99"
#define REV_DATE "2001-02-13"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

#ifdef LINUX
/* forgotten includes */
#include <getopt.h>
#include <dirent.h>
#endif

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <utmp.h>
#include <dirent.h>

#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#ifdef FREEBSD
#include <kvm.h>
#endif

/* this is a somewhat compressed version of "digits[96 * 9]"
 * each 1/4 hex digit is a colormap entry. */
/* *INDENT-OFF* */
static unsigned char c0de[24 * 9 + 1] =
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x05\x40\x08\x25\x62\x56\x20\x22\x56\x25\x62\x56\x25\x62\x56\x00\x00\x00\x00\x00\x03\xc0\x0a\x00"
"\x10\x10\x04\x00\x10\x01\x10\x11\x00\x10\x00\x01\x10\x11\x01\x10\x10\x00\x00\x00\x03\xc0\x0a\x00"
"\x10\x10\x04\x00\x10\x01\x10\x11\x00\x10\x00\x01\x10\x11\x01\x00\x43\xff\xca\xaa\x3f\xfc\xaa\xa0"
"\x20\x20\x08\x25\x60\x56\x25\x62\x56\x25\x60\x02\x35\x72\x56\x01\x03\xff\xca\xaa\x3f\xfc\xaa\xa0"
"\x10\x10\x04\x10\x00\x01\x00\x10\x01\x10\x10\x01\x10\x10\x01\x04\x00\x00\x00\x00\x03\xc0\x0a\x00"
"\x10\x10\x04\x10\x00\x01\x00\x10\x01\x10\x10\x01\x10\x10\x01\x10\x10\x00\x00\x00\x03\xc0\x0a\x00"
"\x05\x40\x04\x25\x62\x56\x00\x22\x56\x25\x60\x02\x25\x62\x56\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"; /* "\x90/bin/sh" oops just kidding */
/* *INDENT-ON* */

typedef struct {		/* dockapp info structure */
    Display *d;
    int screen;
    GdkWindow *win;
    GdkWindow *iconwin;
    GdkGC *gc;
    GdkGC *dgc;			/* drawing gc for lines and shit */
    /* there is a off-by-one error in one of the line/hline functions */
    unsigned char rgb[57 * 57];	/* main image buffer */
    unsigned char buf[57 * 57];	/* secondary image buffer */
    GdkRgbCmap cmap;		/* main colormap */
    /* The colormap is divided like this:
     *   0 .. 240  cube shades (actually only use 0..26 right now
     * 241 .. 254  misc colors (label, buttons, etc)
     * 255         background color (0x202020) */
} Dockapp;

typedef struct {		/* mouse regions */
    int enable;
    int x;
    int y;
    int width;
    int height;
} MRegion;

#define CHAR_WIDTH 5
#define CHAR_HEIGHT 7
#define PI 3.1415926535

/**** Graphics ***********************************/
void redraw_window(void);
void new_window(void);
void setupcolor(short R, short G, short B);
int add_mr(int index, int x, int y, int width, int height);
int check_mr(int x, int y);
static void copy_xpm_area(int x, int y, int w, int h, int dx, int dy);
static void putpixel(int x, int y, int c);
void line(int x1, int y1, int x2, int y2, int c);
static void hline(int x1, int x2, int y, int c);
void trcopy(void);
void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int c);
void draw_cpu(int num, int x, int y);
void update_buffer(unsigned int cpu_usage);
void draw(int color);
void startup_seq();

/**** 3d specific ********************************/

void setupobj(char *filename);
void setUpAngles();
void rotate(int xang, int yang, int zang);
int normal(float p1[], float p2[], float p3[]);
int luminate(float p1[], float p2[], float p3[]);
void sortz(int nofelements);

/**** Application Management, I/O etc. ***********/

void prep_digits(void);
void print_help(void);
int loadobj(char *filename);
void mem_alloc_error(void *block);
int scan4objects(char *dir);
int next_object(void);
void die(void);

/**** System specific functions ******************/

int init_calc_cpu(void);
int calc_cpu_total(void);

/**** Global variables ***************************/

Dockapp dockapp;		/* dockapp info structure */
/* digits for the CPU load + buttons - filled in prep_digits */
static unsigned char digits[96 * 9];
MRegion mr[16];			/* mouse regions */
int xcenter, ycenter, zoff;
double cost[361], sint[361];
double acost[100];
float **matrix;
float **rmatrix;
int **planes;
int *plane_color;
int *zorder;
int *cline;
int nofcoords, noflines, nofplanes;
char *objects[1000];
int nof_objects = 0;
int show_load = 1;
int use_nice = 1;
int which_cpu = -1;
int planesORlines = 1;
char *pname;

float lum_vector[3] = { 0, 0, 100 };	/* Lightsource vector */

#ifdef FREEBSD
static kvm_t *kd;
static struct nlist nlst[] = { {"_cp_time"}, {0} };
#endif

void prep_digits(void)
{
    int i, k;
    unsigned char you, *cant, *c, me;

    cant = digits;
    c = c0de;

    for (i = 0; i < 216; i++) {
	you = *c++;
	for (k = 3; k >= 0; k--) {
	    me = (you >> 6) & 0x3;
	    *cant++ = (me) ? 240 + me : 255;
	    you <<= 2;
	}
    }
}

static void copy_xpm_area(x, y, w, h, dx, dy)
{
    int i, j;
    int spos, dpos;

    for (j = 0; j < h; j++) {
	spos = x + (96 * (y + j));
	dpos = dx + (56 * (dy + j));
	for (i = 0; i < w; i++) {
	    dockapp.buf[dpos++] = digits[spos++];
	}
    }
}

void redraw_window(void)
{
    gdk_draw_indexed_image(dockapp.win, dockapp.gc, 4, 4,
			   56, 56, GDK_RGB_DITHER_NORMAL,
			   dockapp.rgb, 56, &dockapp.cmap);
    gdk_draw_indexed_image(dockapp.iconwin, dockapp.gc, 4, 4,
			   56, 56, GDK_RGB_DITHER_NORMAL,
			   dockapp.rgb, 56, &dockapp.cmap);
}

void new_window(void)
{
    GdkWindowAttr attr;
    Window win, iconwin;
    XSizeHints sizehints;
    XWMHints wmhints;
    /* for mask */
    GdkBitmap *mask;
    unsigned char mask_data[8 * 64];
    int i;
    GdkColor bright;
    /* for that stupid shadow line */
    GdkColormap *map2;

    attr.width = 64;
    attr.height = 64;
    attr.title = "wmcube";
    attr.event_mask = GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK;
    attr.wclass = GDK_INPUT_OUTPUT;
    attr.visual = gdk_visual_get_system();
    attr.colormap = gdk_colormap_get_system();
    attr.wmclass_name = "wmcube";
    attr.wmclass_class = "wmcube";
    attr.window_type = GDK_WINDOW_TOPLEVEL;

    sizehints.flags = USSize;
    sizehints.width = 64;
    sizehints.height = 64;

    dockapp.win =
	gdk_window_new(NULL, &attr,
		       GDK_WA_TITLE | GDK_WA_WMCLASS | GDK_WA_VISUAL |
		       GDK_WA_COLORMAP);

    if (!dockapp.win) {
	fprintf(stderr, "Can't make toplevel window\n");
	exit(1);
    }

    memset(&attr, 0, sizeof(attr));

    attr.width = 64;
    attr.height = 64;
    attr.title = "wmcube";
    attr.event_mask = GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK;
    attr.wclass = GDK_INPUT_OUTPUT;
    attr.visual = gdk_visual_get_system();
    attr.colormap = gdk_colormap_get_system();
    attr.wmclass_name = "wmcube";
    attr.wmclass_class = "wmcube";
    attr.window_type = GDK_WINDOW_TOPLEVEL;

    dockapp.iconwin = gdk_window_new(dockapp.win, &attr,
				     GDK_WA_TITLE | GDK_WA_WMCLASS);

    if (!dockapp.iconwin) {
	fprintf(stderr, "Cannot make icon window\n");
	exit(1);
    }

    win = GDK_WINDOW_XWINDOW(dockapp.win);
    iconwin = GDK_WINDOW_XWINDOW(dockapp.iconwin);

    XSetWMNormalHints(GDK_WINDOW_XDISPLAY(dockapp.win), win, &sizehints);

    wmhints.initial_state = WithdrawnState;
    wmhints.icon_window = iconwin;
    wmhints.icon_x = 0;
    wmhints.icon_y = 0;
    wmhints.window_group = win;
    wmhints.flags =
	StateHint | IconWindowHint | IconPositionHint | WindowGroupHint;
    XSetWMHints(GDK_WINDOW_XDISPLAY(dockapp.win), win, &wmhints);

    /* make the mask rectangle */
    memset(mask_data, 0x00, 8 * 64);
    for (i = 3; i < 61; i++) {
	memset((mask_data + (i * 8)) + 1, 0xFF, 7);
	mask_data[i * 8] = 0xf8;
	mask_data[i * 8 + 7] = 0x1f;
    }
    mask = gdk_bitmap_create_from_data(dockapp.win, mask_data, 64, 64);

    dockapp.gc = gdk_gc_new(dockapp.win);
    dockapp.dgc = gdk_gc_new(dockapp.win);

    /* bwahaha, look at the shit we have to go through just to make a damn
     * color. fucking colormaps */
    map2 = gdk_colormap_get_system();
    gdk_color_parse("#c7c3c7", &bright);
    gdk_colormap_alloc_color(map2, &bright, False, True);
    gdk_gc_set_foreground(dockapp.dgc, &bright);

    /* combine mask with window */
    gdk_window_shape_combine_mask(dockapp.win, mask, 0, 0);
    gdk_window_shape_combine_mask(dockapp.iconwin, mask, 0, 0);

    gdk_window_show(dockapp.win);
}

int add_mr(int index, int x, int y, int width, int height)
{
    mr[index].enable = 1;
    mr[index].x = x;
    mr[index].y = y;
    mr[index].width = width;
    mr[index].height = height;
    return 0;
}

int check_mr(int x, int y)
{
    register int i;
    register int found = 0;

    for (i = 0; i < 32 && !found; i++) {
	if (mr[i].enable && x >= mr[i].x &&
	    x <= mr[i].x + mr[i].width &&
	    y >= mr[i].y && y <= mr[i].y + mr[i].height)
	    found = 1;
    }
    if (!found)
	return -1;
    return (i - 1);

}

int main(int argc, char **argv)
{
    int j, i = 0, rot_speed = 0, cpu_usage = 0, rot_step = 1;
#ifdef PRO
    int count = 2500;
#endif
    long screen_speed = 10000;	/* microseconds between screen updates (approx.) */
    long cpu_update = 490000;	/* microseconds between cpu update (approx.) */
    int loop;
    GdkEvent *event;
    GdkColor shade = { 0, 0, 0, 0 };
    char *tmp = NULL;

    char *rotdiv = { "25" };
    char *rotstep = { "1" };
    char *obj_filename = { "" };
    int rot;
    /* a little further into colormap, darker */
    int cube_color = 20;
    int c = 0;
    int invert_speed = 0;

    pname = strrchr(argv[0], '/');
    if (pname == NULL)
	pname = argv[0];

    srand((unsigned) time(NULL));
    opterr = 0;

    while ((c = getopt(argc, argv, "d:nhpbir:o:c:s:")) != -1) {
	switch (c) {
	case 'c':
	    which_cpu = atoi(optarg);
	    break;
	case 'd':
	    rotstep = optarg;
	    break;
	case 'h':
	    print_help();
	    return 1;
	case 'i':
	    invert_speed = 1;
	    break;
	case 'p':
	    show_load = 0;
	    break;
	case 'b':
	    /* this is an index into our colormap. */
	    cube_color = 0;
	    break;
	case 'r':
	    rotdiv = optarg;
	    break;
	case 'o':
	    obj_filename = optarg;
	    break;
	case 'n':
	    use_nice = 0;
	    break;
	case 's':
	    tmp = strdup(optarg);
	    break;
	case '?':
	    print_help();
	    return 1;
	default:
	    abort();
	}
    }

    /*
     * Validate that wmcube can run on this system given the parameters,
     * then setup the statistics gathering subsystem.
     */

    if (init_calc_cpu() != 0)
	die();

    /* unpack font + button images */
    prep_digits();

    /*
     * Scan directory for .wmc files and choose one randomly. If the user 
     * specified a particular file, load that one.
     */

#ifndef SOLARIS			/* scan4objects doesnt work on Solaris, load object immediatly */
    scan4objects(obj_filename);

    if (nof_objects != 0)
	next_object();
    else
#endif
	setupobj(obj_filename);

    /*
     * Various initializion stuff for the 3d-engine etc.
     */

    setUpAngles();

    rot = atoi(rotdiv);
    if ((rot >= 1) && (rot <= 100));
    else
	rot = 25;

    rot_step = atoi(rotstep);
    if (rot_step < 0)
	rot_step = -rot_step;

    if (calc_cpu_total() == -1)
	die();

    cpu_update /= screen_speed;
    loop = cpu_update;


    if (!gdk_init_check(&argc, &argv)) {
	fprintf(stderr, "GDK init failed\n");
	exit(1);
    }
    gdk_rgb_init();


    memset(dockapp.buf, 0xFF, 56 * 56);

    if (tmp) {
	if (!gdk_color_parse(tmp, &shade)) {
	    fprintf(stderr, "Incorrect color specification\n");
	    exit(1);
	}
	free(tmp);
	setupcolor(shade.red >> 8, shade.green >> 8, shade.blue >> 8);
    } else {
	setupcolor(0x20, 0xB2, 0xAE);
    }

    new_window();

    if (calc_cpu_total() == -1)
	die();

    /* index, left, top, right, bottom */
    add_mr(1, 51, 51, 6, 6);	/* + Zoom In */
    add_mr(5, 7, 51, 6, 6);	/* - Zoom Out */
    add_mr(3, 21, 51, 24, 7);	/*   Show cpu-load */
    add_mr(2, 4, 4, 56, 46);	/* Everywhere else (almost) to change object */

    /*
     * Main loop begins here
     */
    zoff = 3600;

#ifndef PRO
    while (1) {
#else
    while (count--) {
#endif
	i = (i + rot_speed + rot_step) % 360;

	rotate(i, i, i);

	memset(dockapp.rgb, 0xff, 56 * 56);
	draw(cube_color);
	trcopy();

	if (loop++ > cpu_update) {
	    loop = 0;
	    /*
	     * call calc_cpu_total to update statistics.  If some
	     * sort of bad event occurs, calc_cpu_total will return
	     * -1, and we exit.
	     */
	    if ((cpu_usage = calc_cpu_total()) == -1) {
		die();
	    }
	    rot_speed = abs(invert_speed * (100 / rot) - cpu_usage / rot);
	    /* redraw the CPU usage and update screen */
	    update_buffer(cpu_usage);
	}

	/* X Events */
	while (gdk_events_pending()) {
	    event = gdk_event_get();
	    if (event) {
		switch (event->type) {
		case GDK_EXPOSE:
		    /* redraw the shade + 3d window. This doesn't
		     * happen too often. Infact if you don't cover up
		     * the dock, only once */
		    gdk_draw_line(dockapp.win, dockapp.dgc, 60, 4, 60, 60);
		    gdk_draw_line(dockapp.iconwin, dockapp.dgc, 60, 4, 60,
				  60);
		    gdk_draw_line(dockapp.win, dockapp.dgc, 3, 60, 60, 60);
		    gdk_draw_line(dockapp.iconwin, dockapp.dgc, 3, 60, 60,
				  60);
		    redraw_window();
		    break;
		case GDK_DESTROY:
		    gdk_exit(0);
		    exit(0);
		    break;
		case GDK_BUTTON_PRESS:
		    j = check_mr(event->button.x, event->button.y);
		    switch (j) {
		    case 1:
			if (zoff > 750) {
			    copy_xpm_area(88, 0, 6, 9, 47, 46);
			    zoff -= 150;
			}
			break;
		    case 2:
			next_object();
			break;
		    case 3:
			if (show_load == 1)
			    show_load = 0;
			else
			    show_load = 1;
			ycenter = 14 - 2 * show_load;
			break;
		    case 5:
			copy_xpm_area(74, 0, 6, 9, 2, 46);
			zoff += 150;
			break;
		    }
		    break;
		default:
		    break;
		}
	    }
	}
	redraw_window();
#ifndef PRO
	usleep(screen_speed);
#endif
    }

    /*
     * Free up memory used by the object (dirty...) (also never gets here)
     */
    free(matrix);
    free(rmatrix);
    free(cline);

    return 0;
}


/**** Graphics ***********************************
 ************************************************/

void trcopy(void)
{
    unsigned int i = 56 * 56;
    while (i--)
	if (dockapp.buf[i] != 0xff)
	    dockapp.rgb[i] = dockapp.buf[i];
}

void update_buffer(unsigned int cpu_usage)
{
    memset(dockapp.buf, 0xFF, 56 * 56);
    if (show_load)
	draw_cpu(cpu_usage, 22, 46);
    copy_xpm_area(67, 0, 6, 9, 2, 46);
    copy_xpm_area(81, 0, 6, 9, 47, 46);
}

void draw(int color)
{
    int i;

    if (planesORlines) {

	sortz(nofplanes);
	for (i = 0; i < nofplanes; i++) {
	    if (normal
		(rmatrix[planes[zorder[i]][0]],
		 rmatrix[planes[zorder[i]][1]],
		 rmatrix[planes[zorder[i]][2]]) > 0) {

		triangle(xcenter + rmatrix[planes[zorder[i]][0]][0],
			 ycenter + rmatrix[planes[zorder[i]][0]][1],
			 xcenter + rmatrix[planes[zorder[i]][1]][0],
			 ycenter + rmatrix[planes[zorder[i]][1]][1],
			 xcenter + rmatrix[planes[zorder[i]][2]][0],
			 ycenter + rmatrix[planes[zorder[i]][2]][1],
			 plane_color[zorder[i]]);
	    }
	}

    } else {
	for (i = 0; i < noflines; i += 2)
	    line(xcenter + rmatrix[cline[i] - 1][0],
		 ycenter + rmatrix[cline[i] - 1][1],
		 xcenter + rmatrix[cline[i + 1] - 1][0],
		 ycenter + rmatrix[cline[i + 1] - 1][1], color);
    }
}

static void putpixel(int x, int y, int c)
{
    int pos;
    if ((x >= 0) && (x < 56) && (y >= 0) && (y <= 56)) {
	pos = x + (y * 56);
	dockapp.rgb[pos] = c;
    }
}

static void hline(int x1, int x2, int y, int c)
{
    register int i;

    if (c > 25)
	c = 25;

    if ((y >= 0) && (y <= 56)) {
	if (x1 < 0)
	    x1 = 0;
	else if (x1 > 56)
	    return;
	if (x2 > 56)
	    x2 = 56;
	else if (x2 < 0)
	    return;

	for (i = x1; i < x2; i++) {
	    register unsigned int pos;
	    pos = y * 56 + i;
	    dockapp.rgb[pos] = c;
	}
    }
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int c)
{				/* Draws a filled triangle */
    int k, k2, x, x_2, i, tmp1;

    int x1t, x2t;

    if (y3 < y2) {
	tmp1 = y2;
	y2 = y3;
	y3 = tmp1;
	tmp1 = x2;
	x2 = x3;
	x3 = tmp1;
    }

    if (y2 < y1) {
	tmp1 = y1;
	y1 = y2;
	y2 = tmp1;
	tmp1 = x1;
	x1 = x2;
	x2 = tmp1;
    }

    if (y3 < y2) {
	tmp1 = y2;
	y2 = y3;
	y3 = tmp1;
	tmp1 = x2;
	x2 = x3;
	x3 = tmp1;
    }

    if (y1 != y3)
	k = ((x1 - x3) << 6) / (y1 - y3);
    else
	k = (x1 - x3) << 6;

    if (y1 != y2)
	k2 = ((x1 - x2) << 6) / (y1 - y2);
    else
	k2 = (x1 - x2) << 6;

    x = x1 << 6;
    x_2 = x;
    i = y1;

    if (i != y2)
	do {
	    x += k;
	    x_2 += k2;
	    i++;

	    if ((x1t = x >> 6) > (x2t = x_2 >> 6))
		hline(x2t, x1t, i, c);
	    else
		hline(x1t, x2t, i, c);
	}
	while (i != y2);

    if (i == y3)
	return;

    if (y2 != y3)
	k2 = ((x2 - x3) << 6) / (y2 - y3);
    else
	k2 = ((x2 - x3) << 6);

    x_2 = x2 << 6;
    i = y2;
    do {
	x += k;
	x_2 += k2;
	i++;

	if ((x1t = x >> 6) > (x2t = x_2 >> 6))
	    hline(x2t, x1t, i, c);
	else
	    hline(x1t, x2t, i, c);
    }
    while (i != y3);
}

void draw_cpu(int num, int x, int y)
{
    char buf[5];
    int newx = x;
    int i;
    int c;

    if (num > 99) {
	newx -= CHAR_WIDTH;
    }

    if (num > 999) {
	newx -= CHAR_WIDTH;
    }

    sprintf(buf, "%02i%%", num);
    for (i = 0; (c = buf[i]); i++) {
	if (c == '%')
	    copy_xpm_area(60, 0, 7, 9, newx, y);
	else {
	    c -= '0';
	    copy_xpm_area(c * 6, 0, 7, 9, newx, y);
	}
	newx += 6;
    }
}

void line(int x1, int y1, int x2, int y2, int c)
{
    int i, deltax, deltay, numpixels,
	d, dinc1, dinc2, x, xinc1, xinc2, y, yinc1, yinc2;

    deltax = abs(x2 - x1);
    deltay = abs(y2 - y1);
    if (deltax >= deltay) {
	numpixels = deltax + 1;
	d = (deltay << 1) - deltax;
	dinc1 = deltay << 1;
	dinc2 = (deltay - deltax) << 1;
	xinc1 = 1;
	xinc2 = 1;
	yinc1 = 0;
	yinc2 = 1;
    } else {
	numpixels = deltay + 1;
	d = (deltax << 1) - deltay;
	dinc1 = deltax << 1;
	dinc2 = (deltax - deltay) << 1;
	xinc1 = 0;
	xinc2 = 1;
	yinc1 = 1;
	yinc2 = 1;
    }
    if (x1 > x2) {
	xinc1 = -xinc1;
	xinc2 = -xinc2;
    }
    if (y1 > y2) {
	yinc1 = -yinc1;
	yinc2 = -yinc2;
    }
    x = x1;
    y = y1;
    for (i = 1; i < numpixels; i++) {
	putpixel(x, y, c);
	if (d < 0) {
	    d = d + dinc1;
	    x = x + xinc1;
	    y = y + yinc1;
	} else {
	    d = d + dinc2;
	    x = x + xinc2;
	    y = y + yinc2;
	}
    }
}


/**** 3d specific ********************************
 ************************************************/

void rotate(int xang, int yang, int zang)
{
    float tx, ty, tz;
    int i;

    for (i = 0; i < nofcoords; i++) {
	tx = cost[yang] * matrix[i][0] - sint[yang] * matrix[i][2];
	tz = sint[yang] * matrix[i][0] + cost[yang] * matrix[i][2];
	ty = cost[zang] * matrix[i][1] - sint[zang] * tx;

	rmatrix[i][0] = (cost[zang] * tx + sint[zang] * matrix[i][1]);
	rmatrix[i][1] = (sint[xang] * tz + cost[xang] * ty);
	rmatrix[i][2] = (cost[xang] * tz - sint[xang] * ty);
    }

    if (planesORlines)
	for (i = 0; i < nofplanes; i++)
	    if (normal
		(rmatrix[planes[i][0]], rmatrix[planes[i][1]],
		 rmatrix[planes[i][2]]) > 0)
		plane_color[i] =
		    luminate(rmatrix[planes[i][0]], rmatrix[planes[i][1]],
			     rmatrix[planes[i][2]]);

    for (i = 0; i < nofcoords; i++) {
	/* Perspective correcting lines... */
	rmatrix[i][0] =
	    (rmatrix[i][0] * 256) / (2 * rmatrix[i][2] - zoff) + xcenter;
	rmatrix[i][1] =
	    (rmatrix[i][1] * 256) / (2 * rmatrix[i][2] - zoff) + ycenter;
    }
}

void sortz(int nofelements)
{				/* Insertion-sort the planes in increasing z-distance */

    int i, j, k;
    float key;
    float temparr[nofelements];

    for (i = 0; i < nofelements; i++) {
	zorder[i] = i;
	temparr[i] =
	    rmatrix[planes[i][0]][2] + rmatrix[planes[i][1]][2] +
	    rmatrix[planes[i][2]][2];
    }

    for (j = 1; j < nofelements; j++) {

	key = temparr[j];
	k = zorder[j];
	i = j - 1;

	while ((i > -1) && (temparr[i] > key)) {
	    temparr[i + 1] = temparr[i];
	    zorder[i + 1] = zorder[i--];
	}

	zorder[i + 1] = k;
	temparr[i + 1] = key;
    }
}

int normal(float p1[], float p2[], float p3[])
{
    return ((p1[0] - p3[0]) * (p2[1] - p3[1]) -
	    (p2[0] - p3[0]) * (p1[1] - p3[1]));
}

int luminate(float p1[], float p2[], float p3[])
{
    double x1 = (float) (p1[0] - p3[0]), y1 = (float) (p1[1] - p3[1]), z1 =
	(float) (p1[2] - p3[2]);
    double x2 = (float) (p2[0] - p3[0]), y2 = (float) (p2[1] - p3[1]), z2 =
	(float) (p2[2] - p3[2]);
    double nx = y1 * z2 - y2 * z1, ny = -(x1 * z2 - x2 * z1), nz =
	x1 * y2 - y1 * x2;

    return (int) (53 * (acost[(int)
			      (50 +
			       50 * (nx * lum_vector[0] +
				     ny * lum_vector[1] +
				     nz * lum_vector[2]) / (sqrt(nx * nx +
								 ny * ny +
								 nz * nz) *
							    sqrt(lum_vector
								 [0] *
								 lum_vector
								 [0] +
								 lum_vector
								 [1] *
								 lum_vector
								 [1] +
								 lum_vector
								 [2] *
								 lum_vector
								 [2])))] /
			PI));

    /* Do I smell optimization? :-) */
}

void setUpAngles(void)
{
    int i;
    for (i = 0; i < 361; i++) {
	cost[i] = cos((double) i * (2 * PI / (double) 360));
	sint[i] = sin((double) i * (2 * PI / (double) 360));
    }

    for (i = 0; i < 100; i++)
	acost[i] = acos((double) (-50 + i) / 50);
}

void setupcolor(short R, short G, short B)
{
    int i;
    int logi;
    unsigned char r, g, b;

    for (i = 0; i < 26; i++) {
	/* feel free to come up with a better formula here.
	 * I chose log due to lack of any better ideas. */
	logi = i * log(i + 1);
	r = MAX(0, R - logi);			/* R */
	g = MAX(0, G - logi);			/* G */
	b = MAX(0, B - logi);			/* B */
	dockapp.cmap.colors[i] = r << 16 | g << 8 | b;
    }

    /* setup misc colors */
    dockapp.cmap.colors[255] = 0x202020;	/* background */
    dockapp.cmap.colors[241] = 0x00b1ad;	/* load digit #1 */
    dockapp.cmap.colors[242] = 0x007571;	/* load digit #2 */
    dockapp.cmap.colors[243] = 0x004941;	/* icon dark */
}

void setupobj(char *filename)
{
    int i, j = 0;
    int biggest = 0;
    float scale = 1;

    xcenter = 14;
    ycenter = 14 - 2 * show_load;

    if (strcmp(filename, "") != 0)
	loadobj(filename);
    else {
	nofcoords = 8;
	noflines = 24;
	nofplanes = 12;
	planesORlines = 1;

	matrix = (float **) malloc(nofcoords * sizeof(float *));
	mem_alloc_error(matrix);
	planes = (int **) malloc(nofplanes * sizeof(int *));
	mem_alloc_error(planes);
	plane_color = (int *) malloc(nofplanes * sizeof(int));
	mem_alloc_error(plane_color);
	zorder = (int *) malloc(nofplanes * sizeof(int));
	mem_alloc_error(zorder);

	for (i = 0; i < nofplanes; i++)
	    zorder[i] = i;

	for (i = 0; i < nofcoords; i++) {
	    matrix[i] = (float *) malloc(3 * sizeof(float));
	    mem_alloc_error(matrix[i]);
	}

	for (i = 0; i < nofplanes; i++) {
	    planes[i] = (int *) malloc(3 * sizeof(int));
	    mem_alloc_error(planes[i]);
	}

	cline = (int *) malloc((noflines + 1) * sizeof(int));
	mem_alloc_error(cline);

	matrix[0][0] = -180;
	matrix[0][1] = -180;
	matrix[0][2] = 180;	/* 0 */
	matrix[1][0] = 180;
	matrix[1][1] = -180;
	matrix[1][2] = 180;	/* 1 */
	matrix[2][0] = 180;
	matrix[2][1] = 180;
	matrix[2][2] = 180;	/* 2 */
	matrix[3][0] = -180;
	matrix[3][1] = 180;
	matrix[3][2] = 180;	/* 3 */
	matrix[4][0] = -180;
	matrix[4][1] = -180;
	matrix[4][2] = -180;	/* 4 */
	matrix[5][0] = 180;
	matrix[5][1] = -180;
	matrix[5][2] = -180;	/* 5 */
	matrix[6][0] = 180;
	matrix[6][1] = 180;
	matrix[6][2] = -180;	/* 6 */
	matrix[7][0] = -180;
	matrix[7][1] = 180;
	matrix[7][2] = -180;	/* 7 */

	cline[0] = 1;
	cline[1] = 2;
	cline[2] = 2;
	cline[3] = 3;
	cline[4] = 3;
	cline[5] = 4;
	cline[6] = 4;
	cline[7] = 1;
	cline[8] = 5;
	cline[9] = 6;
	cline[10] = 6;
	cline[11] = 7;
	cline[12] = 7;
	cline[13] = 8;
	cline[14] = 8;
	cline[15] = 5;
	cline[16] = 1;
	cline[17] = 5;
	cline[18] = 2;
	cline[19] = 6;
	cline[20] = 3;
	cline[21] = 7;
	cline[22] = 4;
	cline[23] = 8;

	planes[0][0] = 0;
	planes[0][1] = 1;
	planes[0][2] = 3;
	planes[1][0] = 1;
	planes[1][1] = 2;
	planes[1][2] = 3;
	planes[2][0] = 1;
	planes[2][1] = 5;
	planes[2][2] = 6;
	planes[3][0] = 1;
	planes[3][1] = 6;
	planes[3][2] = 2;

	planes[4][0] = 4;
	planes[4][1] = 0;
	planes[4][2] = 3;
	planes[5][0] = 4;
	planes[5][1] = 3;
	planes[5][2] = 7;
	planes[6][0] = 3;
	planes[6][1] = 2;
	planes[6][2] = 7;
	planes[7][0] = 7;
	planes[7][1] = 2;
	planes[7][2] = 6;

	planes[8][0] = 4;
	planes[8][1] = 1;
	planes[8][2] = 0;
	planes[9][0] = 4;
	planes[9][1] = 5;
	planes[9][2] = 1;
	planes[10][0] = 5;
	planes[10][1] = 4;
	planes[10][2] = 7;
	planes[11][0] = 5;
	planes[11][1] = 7;
	planes[11][2] = 6;
    }

    rmatrix = (float **) realloc(rmatrix, nofcoords * sizeof(float *));
    mem_alloc_error(rmatrix);
    for (i = 0; i < nofcoords; i++) {
	rmatrix[i] = (float *) malloc(3 * sizeof(float));
	mem_alloc_error(rmatrix[i]);
    }

    /*
     * Find the longest discance between all coordinates relative to the origin
     */

    for (i = 0; i < nofcoords; i++) {
	j = (int)
	    sqrt((pow(matrix[i][0], 2) + pow(matrix[i][1], 2) +
		  pow(matrix[i][2], 2)));
	if (j > biggest)
	    biggest = j;
    }

    /*
     * Scale every coordinate using the calculated factor
     */

    scale = 280 / (float) biggest;

    for (i = 0; i < nofcoords; i++) {
	matrix[i][0] *= scale;
	matrix[i][1] *= scale;
	matrix[i][2] *= scale;
    }
}


/**** Application Management, I/O etc. ***********
 ************************************************/

void print_help(void)
{
    printf("\nwmCube %s (%s)\n\n", WMCUBE_VERSION, REV_DATE);

#ifndef SOLARIS
    printf
	("  -o <filename or directory>: load external 3d-object(s).\n\n");
#else
    printf("  -o <filename>: load external 3d-object.\n\n");
#endif

    printf
	("  -d x: rotate x degrees/step when the cpu is idle. (default 1)\n");
    printf
	("  -r x: rotate 1 degree faster every x percent of cpu-usage. (default 25)\n");

#ifdef LINUX
    printf
	("  -c x: which cpu (0,1,2..) to monitor. (default average over all)\n");
    printf("  -n  : exclude \"nice\" processes. (default OFF)\n");
#endif

#ifdef SOLARIS
    printf
	("  -c x: which cpu (0,1,2..) to monitor. (default average over all)\n");
#endif

#ifdef FREEBSD
    printf("  -n  : exclude \"nice\" processes. (default OFF)\n");
#endif

#ifdef NETBSD
    printf("  -n  : exclude \"nice\" processes. (default OFF)\n");
#endif

#ifdef OPENBSD
    printf("  -n  : exclude \"nice\" processes. (default OFF)\n");
#endif

    printf("  -b  : draw the cube in a brighter color. (default OFF)\n");
    printf("  -s x: parse color 'x' and use to draw 3D shaded objects\n");
    printf("  -i  : invert cube speed. (default OFF)\n");
    printf("  -p  : do not display cpu-load (default OFF)\n");
    printf("  -h  : display this helptext.\n\n");
}

void die(void)
{
    fprintf(stderr, "%s: exiting", pname);
    exit(1);
}

#ifndef SOLARIS			/* scan4objects doesnt work on Solaris because of alphasort */
int scan4objects(char *dir)
{
    struct dirent **names;
    int n;

    n = scandir(dir, &names, 0, alphasort);

    while (n-- > 0)
	if (strstr(names[n]->d_name, ".wmc") != NULL) {
	    objects[nof_objects] =
		(char *) malloc(strlen(dir) + strlen(names[n]->d_name) +
				2);
	    strcpy(objects[nof_objects], dir);
	    if (dir[strlen(dir)] != '/')
		strcat(objects[nof_objects], "/");
	    strcat(objects[nof_objects++], names[n]->d_name);
	}

    return nof_objects;
}

#endif

int next_object(void)
{
    if (nof_objects == 0)
	return -1;
    setupobj(objects[rand() % (nof_objects)]);

    return 0;
}


int loadobj(char *filename)
{

    FILE *fp;
    char tmp[64] = { "" };
    int i = 0, counter = 1;

    /* printf("\nLoading file %s...",filename); fflush(stdout); */

    if ((fp = fopen(filename, "rt")) == NULL) {
	printf("\nERROR: wmCube object-file not found (%s).\n\n",
	       filename);
	exit(0);
    }

    fscanf(fp, "%s", tmp);

    if (strcmp(tmp, "WMCUBE_COORDINATES") != 0) {
	printf
	    ("\nError in objectfile: it must start with WMCUBE_COORDINATES\n\n");
	fclose(fp);
	exit(0);
    }

    fscanf(fp, "%s", tmp);
    counter = atoi(tmp);

    while ((strcmp(tmp, "WMCUBE_LINES") != 0)
	   && (strcmp(tmp, "WMCUBE_PLANES") != 0)) {

	matrix = (float **) realloc(matrix, (i + 1) * sizeof(float *));
	mem_alloc_error(matrix);
	matrix[i] = (float *) malloc(3 * sizeof(float));
	mem_alloc_error(matrix[i]);
	fscanf(fp, "%f %f %f", &matrix[i][0], &matrix[i][1],
	       &matrix[i][2]);
	/* printf("\n%d: %f %f %f",atoi(tmp), matrix[i][0],matrix[i][1],matrix[i][2]); */

	if (atoi(tmp) != (++i)) {

	    printf("\nError in objectfile (WMCUBE_COORDINATES section):\n"
		   "the coordinates must be listed in order 1..n\n\n");
	    fclose(fp);
	    exit(0);
	}
	fscanf(fp, "%s", tmp);

	if (feof(fp)) {
	    printf
		("\nError in objectfile: you must have a section WMCUBE_LINES or WMCUBE_PLANES\n\n");
	    fclose(fp);
	    exit(0);
	}
    }

    nofcoords = i;
    i = 0;

    if (strcmp(tmp, "WMCUBE_LINES") == 0) {

	planesORlines = 0;
	while (1) {

	    cline = (int *) realloc(cline, (i + 2) * sizeof(int));
	    mem_alloc_error(cline);
	    fscanf(fp, "%d %d", &cline[i++], &cline[i++]);
	    /* printf("\n%d %d",cline[i-2],cline[i-1]); */
	    if (feof(fp))
		break;

	    if (cline[i - 2] > nofcoords || cline[i - 1] > nofcoords) {
		printf("\nError in objectfile (WMCUBE_LINES section):\n"
		       "coordinates %d or/and %d doesnt exist\n\n",
		       cline[i - 2], cline[i - 1]);
		fclose(fp);
		exit(0);
	    }
	}
	noflines = i - 2;
    } else if (strcmp(tmp, "WMCUBE_PLANES") == 0) {

	planesORlines = 1;
	while (1) {
	    planes = (int **) realloc(planes, (i + 1) * sizeof(int *));
	    mem_alloc_error(planes);
	    planes[i] = (int *) malloc(3 * sizeof(int));
	    mem_alloc_error(planes[i]);
	    fscanf(fp, "%d %d %d", &planes[i][0], &planes[i][1],
		   &planes[i][2]);
	    /* printf("\n%d: %d %d %d",i,planes[i][0],planes[i][1],planes[i][2]); */

	    planes[i][0]--;
	    planes[i][1]--;
	    planes[i][2]--;
	    /* printf("\n%d: %d %d %d\n",i,planes[i][0],planes[i][1],planes[i][2]); */

	    if (feof(fp))
		break;

	    if (planes[i][0] > nofcoords || planes[i][1] > nofcoords
		|| planes[i][2] > nofcoords) {
		printf("\nError in objectfile (WMCUBE_PLANES section):\n"
		       "coordinates %d or/and %d or/and %d doesnt exist\n\n",
		       planes[i][0], planes[i][1], planes[i][2]);
		fclose(fp);
		exit(0);
	    }
	    i++;
	}
	nofplanes = i;
	plane_color = (int *) malloc(nofplanes * sizeof(int));
	mem_alloc_error(plane_color);
	zorder = (int *) malloc(nofplanes * sizeof(int));
	mem_alloc_error(zorder);
	for (i = 0; i < nofplanes; i++)
	    zorder[i] = i;

    } else {
	printf
	    ("\nError in objectfile: you must have a section WMCUBE_LINES or WMCUBE_PLANES\n\n");
	fclose(fp);
	exit(0);
    }

    fclose(fp);
    return 1;
}

void mem_alloc_error(void *block)
{
    if (block == NULL) {
	printf("\nError allocating memory!\n\n");
	exit(1);
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Begin System Specific Code.  If you wish to port wmcube to a new platform,
 * you'll need to implement the following operations:
 *
 * int init_calc_cpu();
 * 	Perform feature tests to determine whether wmcube can run, and set up
 * 	any files/data structures/etc. to gather statistics.
 *
 * int calc_cpu_total();
 * 	return an integer reflecting the current CPU load
 */

#if defined LINUX

/*
 * init_calc_cpu doesn't have much to do on Linux, but it can check to see if
 * /proc/stat is available; if the user selected to monitor a particular CPU,
 * it can check it's existence.
 */
int init_calc_cpu()
{
    FILE *fp;
    int i;
    char cpuid[6];
    char check_cpu[6];

    sprintf(check_cpu, "cpu%d", which_cpu);

    if ((fp = fopen("/proc/stat", "rb")) == NULL) {
	perror("/proc/stat required for this system");
	return -1;
    }

    if (which_cpu == -1)
	return 0;

    for (i = -2; i < which_cpu; i++) {
	fscanf(fp, "%s", cpuid);
    }

    if (strcmp(check_cpu, cpuid) != 0) {
	fprintf(stderr, "ERROR: could not read cpu-load on %s. Are you "
		"sure you have an SMP system?\n", check_cpu);
	return -1;
    }
    return (0);
}

int calc_cpu_total()
{
    int total, used, t = 0, i;
    static int previous_total = 0, previous_used = 0;
    char cpuid[6];
    int cpu, nice, system, idle;
    FILE *fp;

    fp = fopen("/proc/stat", "rt");

    for (i = -2; i < which_cpu; i++) {
	fscanf(fp, "%s %d %d %d %d", cpuid, &cpu, &nice, &system, &idle);
    }

    fclose(fp);

    used = cpu + system + use_nice * nice;
    total = used + idle + (1 - use_nice) * nice;

    t = 100 * (double) (used - previous_used) / (double) (total -
							  previous_total);
    previous_total = total;
    previous_used = used;

    return t;
}

#elif defined SOLARIS
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <kstat.h>

static kstat_ctl_t *kc;
static kstat_t **cpu_ksp_list;
static kstat_t *the_cpu;
static int ncpus;

/*
 * The biggest subtlety of the Solaris port is that init_calc_cpu can be called
 * after the initial program setup.  This occurs when a 'kstat state change'
 * occurs.  Usually this means that a CPU has been taken on or off-line using
 * the psradm command.  Another possibility is that on server systems, a new
 * CPU might have been hot-added to a running system.
 *
 * As a result, init_calc_cpu frees any resources it might have setup if needed,
 * and reinitializes everything.
 */
int init_calc_cpu()
{
    kstat_t *ksp;
    int i = 0;

    if (kc == NULL) {
	if ((kc = kstat_open()) == NULL) {
	    fprintf(stderr, "wmcube: can't open /dev/kstat\n");
	    return -1;
	}
    }

    if (which_cpu != -1) {
	/*
	 * User selected to monitor a particular CPU.  find it...
	 */
	for (ksp = kc->kc_chain; ksp; ksp = ksp->ks_next) {
	    if ((strcmp(ksp->ks_module, "cpu_stat") == 0) &&
		(ksp->ks_instance == which_cpu)) {
		the_cpu = ksp;
		break;
	    }
	}
	if (the_cpu == NULL) {
	    fprintf(stderr, "CPU %d not found\n", which_cpu);
	    return -1;
	}
    } else {
	/*
	 * User selected to monitor all CPUs.  First, count them.
	 */
	for (ksp = kc->kc_chain; ksp; ksp = ksp->ks_next) {
	    if (strcmp(ksp->ks_module, "cpu_stat") == 0)
		i++;
	}

	if (cpu_ksp_list) {
	    free(cpu_ksp_list);
	}
	cpu_ksp_list = (kstat_t **) calloc(i * sizeof(kstat_t *), 1);
	ncpus = i;

	/*
	 * stash the ksp for each CPU.
	 */
	i = 0;
	for (ksp = kc->kc_chain; ksp; ksp = ksp->ks_next) {
	    if (strcmp(ksp->ks_module, "cpu_stat") == 0) {
		cpu_ksp_list[i] = ksp;
		i++;
	    }
	}
    }
    return 0;
}

int calc_cpu_total()
{
    int i;
    cpu_stat_t stat;
    static int previous_total = 0, previous_used = 0;
    int used, total, t, user = 0, wait = 0, kern = 0, idle = 0;

    /*
     * Read each cpu's data.  If the kstat chain has changed (a state change
     * has happened, maybe a new cpu was added to the system or one went
     * away), then reinitialize everything with init_calc_cpu().  Finally,
     * recursively call calc_cpu_total.
     *
     * We'll need to do a little better than this in the future, since we
     * could recurse too much in the pathological case here.
     */
    if (which_cpu == -1) {
	for (i = 0; i < ncpus; i++) {
	    if (kstat_read(kc, cpu_ksp_list[i], (void *) &stat) == -1) {
		if (init_calc_cpu() != 0) {
		    fprintf(stderr, "failed to "
			    "reinitialize following state " "change\n");
		    return (-1);
		}
		return (calc_cpu_total());
	    }
	    user += stat.cpu_sysinfo.cpu[CPU_USER];	/* user */
	    wait += stat.cpu_sysinfo.cpu[CPU_WAIT];	/* io wait */
	    kern += stat.cpu_sysinfo.cpu[CPU_KERNEL];	/* sys */
	    idle += stat.cpu_sysinfo.cpu[CPU_IDLE];	/*idle("free") */
	}
    } else {
	if (kstat_read(kc, the_cpu, (void *) &stat) == -1) {
	    if (init_calc_cpu() != 0) {
		fprintf(stderr, "failed to reinitialize "
			"following state change\n");
		return (-1);
	    }
	    return (calc_cpu_total());
	}
	user += stat.cpu_sysinfo.cpu[CPU_USER];	/* user */
	wait += stat.cpu_sysinfo.cpu[CPU_WAIT];	/* io wait */
	kern += stat.cpu_sysinfo.cpu[CPU_KERNEL];	/* sys */
	idle += stat.cpu_sysinfo.cpu[CPU_IDLE];	/* idle("free") */
    }

    used = user + wait + kern;
    total = used + idle;
    t = 100 * (double) (used - previous_used) /
	(double) (total - previous_total);
    previous_total = total;
    previous_used = used;
    return (t);
}

#elif defined FREEBSD
#include <nlist.h>
#include <fcntl.h>
#include <sys/dkstat.h>

int init_calc_cpu()
{

    if ((kd = kvm_open(NULL, NULL, NULL, O_RDONLY, "kvm_open")) == NULL) {
	printf("\nError: unable to open kvm\n\n");
	exit(0);
    }
    kvm_nlist(kd, nlst);
    if (nlst[0].n_type == 0) {
	printf("\nError: unable to get nlist\n\n");
	exit(1);
    }

    /* drop setgid & setuid (hi GOBBLES, who the fuck are you? */
    seteuid(getuid());
    setegid(getgid());

    if (geteuid() != getuid() || getegid() != getgid()) {
	printf("Unable to drop privileges\n");
	exit(1);
    }

    return 0;
}

int calc_cpu_total()
{
    int total, used, t = 0;
    static int previous_total = 0, previous_used = 0;
    int cpu, nice, system, idle;
    unsigned long int cpu_time[CPUSTATES];

    if (kvm_read(kd, nlst[0].n_value, &cpu_time, sizeof(cpu_time))
	!= sizeof(cpu_time)) {
	printf("\nError reading kvm\n\n");
	exit(0);
    }

    cpu = cpu_time[CP_USER];
    nice = cpu_time[CP_NICE];
    system = cpu_time[CP_SYS];
    idle = cpu_time[CP_IDLE];

    used = cpu + system + use_nice * nice;
    total = used + idle + (1 - use_nice) * nice;

    t = 100 * (double) (used - previous_used) / (double) (total -
							  previous_total);
    previous_total = total;
    previous_used = used;

    return t;
}

#elif defined OPENBSD
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/dkstat.h>

int init_calc_cpu()
{
    return 0;
}

int calc_cpu_total()
{
    int total, used, t=0;
    static int previous_total = 0, previous_used = 0;
    int cpu,nice,system,idle;
    unsigned long int cpu_time[CPUSTATES];

    int mib[2];
    size_t size;

    mib[0] = CTL_KERN;
    mib[1] = KERN_CPTIME;
    size = sizeof (cpu_time);

    if (sysctl(mib, 2, &cpu_time, &size, NULL, 0) < 0)
	return 0;

    cpu = cpu_time[CP_USER];
    nice = cpu_time[CP_NICE];
    system = cpu_time[CP_SYS];
    idle = cpu_time[CP_IDLE];

    used = cpu + system + use_nice*nice;
    total = used + idle + (1-use_nice)*nice;

    t = 100 * (double)(used - previous_used) / (double)(total - previous_total);
    previous_total = total;
    previous_used = used;

    return t;
}


#elif defined NETBSD		/* END OPENBSD */
#include <sys/sched.h>
#include <sys/sysctl.h>

int init_calc_cpu()
{
    return 0;
}

int calc_cpu_total()
{
    static u_int64_t last_cp_time[CPUSTATES] = { 0, 0, 0, 0, 0 };
    u_int64_t curr_cp_time[CPUSTATES];
    u_int64_t total_time = 0, idle_time = 0;
    int mib[2];
    int i;
    size_t ssize;
    const int IDLE_TIME = 4;
    const int NICE_TIME = 1;

    ssize = sizeof(curr_cp_time);
    mib[0] = CTL_KERN;
    mib[1] = KERN_CP_TIME;
    if (sysctl(mib, 2, curr_cp_time, &ssize, NULL, 0)) {
	fprintf(stderr, "wmcube: unable to read CP_TIME from sysctl()\n");
	exit(0);
    }
    if (!use_nice)
	curr_cp_time[NICE_TIME] = 0;

    /* NetBSD gives 5 CPUSTATES - 
     * User, Nice, System, Interrupt, Idle 
     */
    idle_time = curr_cp_time[IDLE_TIME] - last_cp_time[IDLE_TIME];
    for (i = 0; i < CPUSTATES; i++) {
	total_time += (curr_cp_time[i] - last_cp_time[i]);
	last_cp_time[i] = curr_cp_time[i];
    }

    /* Calculate the % CPU usage as the User+Nice+System+Interrupt/Total
     * for the interval
     */
    return (100 * (int) (total_time - idle_time) / total_time);

}

#else				/* END NETBSD */

/*
 * This is a stub which will compile for platforms other than LINUX or SOLARIS.
 * Use these to start your port to a new platform.
 */
int init_calc_cpu()
{
    return 0;
}

int calc_cpu_total()
{
    return 0;
}

#endif				/* OS SPECIFIC CODE */
