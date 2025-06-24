#include <time.h>
#include <unistd.h>
#include "array_utils.h"

void insertion_sort(int (*arr)[], int arr_length) {
	for (int sorted_max_index = 0; sorted_max_index < arr_length - 1; sorted_max_index++) {
		for (int i = sorted_max_index, unsorted_min_index = i + 1; i >= 0; i--, unsorted_min_index--) {
			if ((*arr)[unsorted_min_index] > (*arr)[i]) {
				break;
			}
			arr_swap(arr, i, unsorted_min_index);
		}
		//highlight_index = sorted_max_index;
		usleep(5000);
	}
}

void selection_sort(int (*arr)[], int arr_length) {
	for (int sorted_max_index = 0, min_index = 0; sorted_max_index < arr_length; sorted_max_index++, min_index = sorted_max_index) {
		for (int unsorted_index = sorted_max_index + 1; unsorted_index < arr_length; unsorted_index++) {
			if ((*arr)[unsorted_index] < (*arr)[min_index]) {
				min_index = unsorted_index;
			}
		}
		arr_swap(arr, sorted_max_index, min_index);
		//highlight_index = sorted_max_index;
		usleep(5000);
	}
}

void bubble_sort(int (*arr)[], int arr_length) {
	for (int sorted_max_index = 0; sorted_max_index < arr_length; sorted_max_index++) {
		for (int unsorted_index = arr_length - 1; unsorted_index > sorted_max_index; unsorted_index--) {
			if ((*arr)[unsorted_index - 1] > (*arr)[unsorted_index]) {
				arr_swap(arr, unsorted_index - 1, unsorted_index);
			}
		}
		//highlight_index = sorted_max_index;
		usleep(5000);
	}
}

int quick_sort_partition(int (*arr)[], int left, int right) {
	int pivot = (*arr)[right];
	//int pivot = (rand() % (right + 1 - left)) + left;
	//int pivot = (int)floor((right + left) / 2);
	//highlight_index = pivot;
	int i = left - 1;

	for (int j = left; j <= right; j++) {
		if ((*arr)[j] < pivot) {
			i++;
			arr_swap(arr, i, j);
		}
	}
	arr_swap(arr, i + 1, right);
	usleep(5000);
	return i + 1;
}

void quick_sort(int (*arr)[], int left, int right) {
	if (left >= right)
		return;
	int index = quick_sort_partition(arr, left, right);
	quick_sort(arr, left, index-1);
	quick_sort(arr, index, right);
}

void merge(int (*arr)[], int (*tmp)[], int start, int end) {
	int leftEnd = (int)((end + start) / 2);
	int rightStart = leftEnd + 1;

	int left = start;
	int right = rightStart;
	int index = start;

	while (left <= leftEnd && right <= end) {
		if ((*arr)[left] <= (*arr)[right]) {
			(*tmp)[index] = (*arr)[left];
			++left;
		} else {
			(*tmp)[index] = (*arr)[right];
			++right;
		}
		//highlight_index = index;
		++index;
	}

	while (left <= leftEnd) {
		(*tmp)[index] = (*arr)[left];
		++left;
		//highlight_index = index;
		++index;
	}

	while (right <= end) {
		(*tmp)[index] = (*arr)[right];
		++right;
		//highlight_index = index;
		++index;
	}

	for (int i = start; i <= end; i++) {
		(*arr)[i] = (*tmp)[i];
	}
	usleep(5000);
}

void merge_sort(int (*arr)[], int (*tmp)[], int start, int end) {
	if ((end - start + 1) < 2) {
		return;
	}

	int mid = (int)((end + start) / 2);

	merge_sort(arr, tmp, start, mid);
	merge_sort(arr, tmp, mid + 1, end);
	merge(arr, tmp, start, end);
}
