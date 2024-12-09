# For Debian packaging
DESTDIR =

# where to install this program
PREFIX = $(DESTDIR)/usr/local

# no user serviceable parts below this line
# optimization cflags
CFLAGS = -O3 -Wall `gtk-config --cflags` ${EXTRA}
# profiling cflags
# CFLAGS=-ansi -pedantic -Wall -pg -O3 `gtk-config --cflags` ${EXTRA} -DPRO
# test coverage cflags
# CFLAGS=-fprofile-arcs -ftest-coverage -Wall -ansi -pedantic -g `gtk-config --cflags` ${EXTRA} -DPRO


CC = gcc
SHELL = sh
OBJS = fishmon.o
LIBS = `gtk-config --libs | sed "s/-lgtk//g"`
INSTALL = -m 755

all: wmfishtime

wmfishtime: $(OBJS)
	$(CC) $(CFLAGS) -o wmfishtime $(OBJS) $(LIBS)

clean:
	rm -rf wmfishtime *.o *.bb* *.gcov gmon.* *.da *~ .xvpics

# You're going to have to be root to do this!
install:
	install $(INSTALL) wmfishtime $(PREFIX)/bin
	install $(INSTALL) wmfishtime.1 $(PREFIX)/man/man1
