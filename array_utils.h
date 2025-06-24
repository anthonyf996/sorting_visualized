#ifndef _ARRAY_UTILS_H
#define _ARRAY_UTILS_H

void fill_range(int (*arr)[], int start, int end);
void arr_swap(int (*arr)[], int index1, int index2);
void arr_randomize(int (*arr)[], int arr_length);
void arr_copy(int (*arr2)[], int (*arr)[], int start, int end);

#endif
