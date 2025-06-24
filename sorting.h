#ifndef _SORTING_H_
#define _SORTING_H_

void insertion_sort(int (*arr)[], int arr_length);
void selection_sort(int (*arr)[], int arr_length);
void bubble_sort(int (*arr)[], int arr_length);
int quick_sort_partition(int (*arr)[], int left, int right);
void quick_sort(int (*arr)[], int left, int right);
void merge(int (*arr)[], int (*tmp)[], int start, int end);
void merge_sort(int (*arr)[], int (*tmp)[], int start, int end);

#endif
