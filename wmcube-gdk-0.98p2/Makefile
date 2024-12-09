CC = gcc
OS = -DLINUX

LIBDIR = -L/usr/local/lib -L/usr/X11R6/lib
INCDIR = -I/usr/local/include -I/usr/local/lib/glib/include

# PRO = -g -pg -DPRO

# Edit OSLIBS as appropriate to include OS specific libraries.

OSLIBS =
#LIBS   = -lgdk $(OSLIBS)
LIBS = `gtk-config --libs | sed "s/-lgtk//g"`
CFLAGS = -Wall `gtk-config --cflags` ${EXTRA} -ansi -O3 $(PRO) $(OS)

OBJS =	wmcube.o

.c.o:
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $*.o

wmcube: $(OBJS)
	$(CC) -o wmcube $^ $(CFLAGS) $(LIBDIR) $(LIBS)


all:: wmcube

clean::
	for i in $(OBJS) ; do \
		rm -f $$i ; \
	done
	rm -f wmcube
	rm -f *~

install:: wmcube
	cp -f wmcube /usr/local/bin/
	chmod 755 /usr/local/bin/wmcube
	chown root:root /usr/local/bin/wmcube
	@echo "wmCube installation finished..."
