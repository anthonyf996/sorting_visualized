CC := gcc -Wall
CFLAGS :=
CPPFLAGS :=
LDCFLAGS :=
CAIRO_LIBS=`pkg-config --libs --cflags cairo cairo-xlib`
GTK3_LIBS=`pkg-config --libs --cflags gtk+-3.0`
OBJS :=
TARGETS := sort_demo sorting_test

all:	sort_demo

sort_demo:	sort_demo.c sorting.c sorting.h array_utils.c array_utils.h
		$(CC) $(CFLAGS) $(CPPFLAGS) -o sort_demo sort_demo.c sorting.c array_utils.c $(CAIRO_LIBS) $(GTK3_LIBS)

test:	sorting_test

sorting_test:	sorting_test.c	sorting.c sorting.h array_utils.c array_utils.h
		$(CC) $(CFLAGS) $(CPPFLAGS) -o sorting_test sorting_test.c sorting.c array_utils.c

.PHONY: clean
clean:
	-rm -f $(TARGETS) $(OBJS)
