#include <stdlib.h>

void fill_range(int (*arr)[], int start, int end) {
	for (int i = start; i < end; i++) {
		(*arr)[i] = i;
	}
}

void arr_swap(int (*arr)[], int index1, int index2) {
	int temp = (*arr)[index1];
	(*arr)[index1] = (*arr)[index2];
	(*arr)[index2] = temp;
}

void arr_randomize(int (*arr)[], int arr_length) {
	for (int i = 0; i < arr_length; i++) {
		arr_swap(arr, i, rand() % arr_length);
	}
}

void arr_copy(int (*arr2)[], int (*arr)[], int start, int end) {
	for (int i = start; i < end; i++) {
		(*arr2)[i] = (*arr)[i];
	}
}
