#include <stdio.h>
#include "array_utils.h"
#include "sorting.h"

#define SUCCESS_MESSAGE "(SUCCESS) %s\n"
#define FAILURE_MESSAGE "(FAILURE) %s\n"

enum algo {
	INSERTION,
	SELECTION,
	BUBBLE,
	QUICK,
	MERGE
};

struct test_stats {
	int num_tests;
	int num_tests_passed;
};

static void run_algo(int (*arr)[], int arr_length, enum algo a) {
	if (a == INSERTION) {
		insertion_sort(arr, arr_length);
	} else if (a == SELECTION) {
		selection_sort(arr, arr_length);
	} else if (a == BUBBLE) {
		bubble_sort(arr, arr_length);
	} else if (a == QUICK) {
		quick_sort(arr, 0, arr_length);
	} else if (a == MERGE) {
		int tmp[arr_length];
		merge_sort(arr, &tmp, 0, arr_length);
	}
}

static int test(enum algo a, int arr_length) {
	int arr[arr_length];
	int arr2[arr_length];
	fill_range(&arr, 0, arr_length);
	arr_copy(&arr2, &arr, 0, arr_length);
	arr_randomize(&arr2, arr_length);

	run_algo(&arr2, arr_length, a);

	for (int i = 0; i < arr_length; i++) {
		if (arr[i] != arr2[i]) {
			return 0;
		}
	}

	return 1;
}

static void run_test(enum algo a, int arr_length, char *test_name, struct test_stats *stats) {
	++stats->num_tests;
	if (!test(INSERTION, arr_length)) {
		printf(FAILURE_MESSAGE, test_name);
	} else {
		++stats->num_tests_passed;
		printf(SUCCESS_MESSAGE, test_name);
	}
}

int main(void) {
	struct test_stats stats = {0,0};

	run_test(INSERTION, 100, "Insertion Sort", &stats);
	run_test(SELECTION, 100, "Selection Sort", &stats);
	run_test(BUBBLE, 100, "Bubble Sort", &stats);
	run_test(QUICK, 100, "Quick Sort", &stats);
	run_test(MERGE, 100, "Merge Sort", &stats);
	printf("Passed: ( %d / %d )\n", stats.num_tests_passed, stats.num_tests);

	return 0;
}
