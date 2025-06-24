#include <gtk/gtk.h>
#include "array_utils.h"
#include "sorting.h"

#define SUCCESS 0
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define DRAW_AREA_WIDTH 1280
#define DRAW_AREA_HEIGHT 720
#define SLEEP_TIME_ACTION_THREAD 2000
#define SLEEP_TIME_DRAW_THREAD 2000
#define SLEEP_TIME_INVALIDATE_CALLBACK 1000 / 60
#define ARR_LENGTH ((DRAW_AREA_WIDTH) / (3))

static gboolean invalidate_callback(void *ptr);

static pthread_t drawing_thread;
static pthread_mutex_t mutex;
static pthread_t action_thread;
static pthread_mutex_t amutex;
static cairo_surface_t *surface = NULL;
static int surface_width;
static int surface_height;
static int arr[ARR_LENGTH] = {0};
static int highlight_index = 0;

enum action {
	STANDBY,
	QUIT,
	RANDOMIZE,
	INSERTION_SORT,
	SELECTION_SORT,
	BUBBLE_SORT,
	QUICK_SORT,
	MERGE_SORT
};

static enum action curr_action = STANDBY;

void set_action_standby_and_reset() {
	fill_range(&arr, 0, ARR_LENGTH);
}

void set_action_randomize() {
	arr_randomize(&arr, ARR_LENGTH);
}

void set_action_insertion_sort() {
	curr_action = INSERTION_SORT;
}

void set_action_selection_sort() {
	curr_action = SELECTION_SORT;
}

void set_action_bubble_sort() {
	curr_action = BUBBLE_SORT;
}

void set_action_quick_sort() {
	curr_action = QUICK_SORT;
}

void set_action_merge_sort() {
	curr_action = MERGE_SORT;
}

static gboolean invalidate_callback(void *ptr) {
	if (GTK_IS_WIDGET(ptr)) {
		gtk_widget_queue_draw(GTK_WIDGET (ptr));
		return TRUE;
	}

	return FALSE;
}

static gboolean drawing_area_configure_callback(GtkWidget *widget, GdkEventConfigure *event) {
	if (event->type == GDK_CONFIGURE) {
		pthread_mutex_lock(&mutex);

		if (surface != (cairo_surface_t *)NULL) {
			cairo_surface_destroy(surface);
		}

		GtkAllocation allocation;
		gtk_widget_get_allocation(widget, &allocation);
		surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, allocation.width, allocation.height);
		surface_width = allocation.width;
		surface_height = allocation.height;

		pthread_mutex_unlock(&mutex);
	}

	return TRUE;
}

static void drawing_area_draw_callback(GtkWidget *widget, cairo_t *context, void *ptr) {
	pthread_mutex_lock(&mutex);

	if (surface != (cairo_surface_t *)NULL) {
		cairo_set_source_surface(context, surface, 0, 0);
		cairo_paint(context);
	}

	pthread_mutex_unlock(&mutex);
}

static void * thread_action(void *ptr) {
	while (1) {
		usleep(SLEEP_TIME_ACTION_THREAD);
		pthread_mutex_lock(&amutex);
		if (curr_action == INSERTION_SORT) {
			insertion_sort(&arr, ARR_LENGTH);
			highlight_index = 0;
			curr_action = STANDBY;
		} else if (curr_action == SELECTION_SORT) {
			selection_sort(&arr, ARR_LENGTH);
			highlight_index = 0;
			curr_action = STANDBY;
		} else if (curr_action == BUBBLE_SORT) {
			bubble_sort(&arr, ARR_LENGTH);
			highlight_index = 0;
			curr_action = STANDBY;
		} else if (curr_action == QUICK_SORT) {
			quick_sort(&arr, 0, ARR_LENGTH - 1);
			highlight_index = 0;
			curr_action = STANDBY;
		} else if (curr_action == MERGE_SORT) {
			int tmp[ARR_LENGTH];
			merge_sort(&arr, &tmp, 0, ARR_LENGTH - 1);
			highlight_index = 0;
			curr_action = STANDBY;
		}
		pthread_mutex_unlock(&amutex);
	}
	return NULL;
}

static void * thread_draw(void *ptr) {
	while (1) {
		usleep(SLEEP_TIME_DRAW_THREAD);

		if (surface == (cairo_surface_t *)NULL) {
			continue;
		}

		pthread_mutex_lock(&mutex);

		cairo_t *context = cairo_create(surface);

		cairo_set_source_rgb(context, 0, 0, 0);
		cairo_rectangle(context, 0, 0, surface_width, surface_height);
		cairo_fill(context);

		cairo_set_source_rgb(context, 0.1, 0.1, 0.1);
		cairo_paint(context);

		cairo_rectangle(context, 0, 0, surface_width, surface_height);
		cairo_fill(context);

		cairo_set_source_rgb(context, 0.27, 0.68, 1);
		for (int i = 0; i < ARR_LENGTH; i++) {
			if (i == highlight_index) {
				cairo_set_source_rgb(context, 0.12, 1, 0.12);
			} else {
				cairo_set_source_rgb(context, 0.27, 0.68, 1);
			}
			cairo_rectangle(context, 0 + (i * 3), surface_height - (arr[i] * 1.0), 2, (arr[i] * 1.0));
			cairo_fill(context);
		}
		
		cairo_destroy(context);

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	fill_range(&arr, 0, ARR_LENGTH);

	gtk_init(&argc, &argv);

	//GtkWidget *window, *vbox, *toolbar;
	GtkWidget *window, *drawingArea, *vbox, *toolbar;
	GtkToolItem *resetTab, *randomizeTab, *insertionSortTab, *selectionSortTab, *bubbleSortTab, *quickSortTab, *mergeSortTab;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Demo");
	gtk_window_set_default_size(GTK_WINDOW(window), WIN_WIDTH, WIN_HEIGHT);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	drawingArea = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawingArea, DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT);
	g_signal_connect(drawingArea, "configure-event", G_CALLBACK(drawing_area_configure_callback), NULL);
	gtk_container_add(GTK_CONTAINER(vbox), drawingArea);

	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

	resetTab = gtk_tool_button_new(NULL, "Reset");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), resetTab, 0);
	g_signal_connect(G_OBJECT(resetTab), "clicked", G_CALLBACK(set_action_standby_and_reset), resetTab);

	randomizeTab = gtk_tool_button_new(NULL, "Randomize");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), randomizeTab, 3);
	g_signal_connect(G_OBJECT(randomizeTab), "clicked", G_CALLBACK(set_action_randomize), randomizeTab);

	insertionSortTab = gtk_tool_button_new(NULL, "Insertion Sort");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), insertionSortTab, 4);
	g_signal_connect(G_OBJECT(insertionSortTab), "clicked", G_CALLBACK(set_action_insertion_sort), insertionSortTab);

	selectionSortTab = gtk_tool_button_new(NULL, "Selection Sort");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), selectionSortTab, 5);
	g_signal_connect(G_OBJECT(selectionSortTab), "clicked", G_CALLBACK(set_action_selection_sort), selectionSortTab);

	bubbleSortTab = gtk_tool_button_new(NULL, "Bubble Sort");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), bubbleSortTab, 6);
	g_signal_connect(G_OBJECT(bubbleSortTab), "clicked", G_CALLBACK(set_action_bubble_sort), bubbleSortTab);

	quickSortTab = gtk_tool_button_new(NULL, "Quick Sort");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), quickSortTab, 7);
	g_signal_connect(G_OBJECT(quickSortTab), "clicked", G_CALLBACK(set_action_quick_sort), quickSortTab);

	mergeSortTab = gtk_tool_button_new(NULL, "Merge Sort");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), mergeSortTab, 8);
	g_signal_connect(G_OBJECT(mergeSortTab), "clicked", G_CALLBACK(set_action_merge_sort), mergeSortTab);

	gtk_widget_show_all(window);

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&amutex, NULL);
	pthread_create(&drawing_thread, NULL, thread_draw, NULL);
	pthread_create(&action_thread, NULL, thread_action, NULL);

	g_timeout_add(SLEEP_TIME_INVALIDATE_CALLBACK, invalidate_callback, drawingArea);

	g_signal_connect(drawingArea, "draw", G_CALLBACK(drawing_area_draw_callback), NULL);

	gtk_main();

	return SUCCESS;
}
