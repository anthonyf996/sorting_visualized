CC := gcc -Wall
CFLAGS :=
CPPFLAGS :=
LDCFLAGS :=
CAIRO_LIBS=`pkg-config --libs --cflags cairo cairo-xlib`
GTK3_LIBS=`pkg-config --libs --cflags gtk+-3.0`
OBJS :=
TARGETS := sorting_visualized sorting_test

all:	sorting_visualized

sorting_visualized:	sorting_visualized.c sorting.c sorting.h array_utils.c array_utils.h
		$(CC) $(CFLAGS) $(CPPFLAGS) -o sorting_visualized sorting_visualized.c sorting.c array_utils.c $(CAIRO_LIBS) $(GTK3_LIBS)

test:	sorting_test

sorting_test:	sorting_test.c	sorting.c sorting.h array_utils.c array_utils.h
		$(CC) $(CFLAGS) $(CPPFLAGS) -o sorting_test sorting_test.c sorting.c array_utils.c

.PHONY: clean
clean:
	-rm -f $(TARGETS) $(OBJS)
