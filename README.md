# DKU-Algorithm
22년도 알고리즘 수업

32183376 이원규 과제2 제출입니다.
소스코드는 sortAlgo.c 입니다.
```c
/*
*	DKU Algorithm HW#2
*	    Student id : 32183376
*	    Student name : Wonkyu-Lee
*       Date: 2022.04.18
*   Exchagesort, Mergesort, Quicksort, Heapsort, Radix sort
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
void exchageSort(int *arr,int n) {
	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (arr[i] > arr[j]) {
				swap(arr + i, arr + j);
			}
		}
	}
}
void merge(int *arr,int low, int mid, int high) {
	int* temp = (int*)malloc(sizeof(int) * (high + 1));
	int i = low, j = mid + 1;
	int idx=0;
	while (i <= mid && j <= high) {
		if (arr[i] <= arr[j]) {
			temp[idx++] = arr[i++];
		}
		else {
			temp[idx++] = arr[j++];
		}
	}
		while (i <= mid) temp[idx++] = arr[i++];
		while (j <= high) temp[idx++] = arr[j++];
		idx--;
		while (idx >= 0) {
			arr[low + idx] = temp[idx];
			idx--;
		}
	
}
void mergeSort(int *arr,int low, int high) {
	int mid;
	if (low < high) {
		mid = (low + high) / 2;
		mergeSort(arr, low, mid);
		mergeSort(arr, mid + 1, high);
		merge(arr, low, mid, high);
	}
}
void quickSort(int *arr, int l, int r) {
	if (l >= r) return;
	int pivot = l; //pivot 왼쪽 고정
	int left = pivot + 1, right = r;
	
	while (left < right) {
		while (left <= r && arr[left] <= arr[pivot])left++;
		while (right > l && arr[right] >= arr[pivot])right--;
		if (left > right) {
			swap(arr + right, arr + pivot);
		}
		else {
			swap(arr + left, arr + right);
		}
	}

	quickSort(arr, l, right - 1);
	quickSort(arr, right + 1, r);
}

void heapify(int*arr, int n, int i) {
	int parent = i;
	int leftChild = i * 2 + 1;
	int rightChild = i * 2 + 2;

	if (leftChild < n && arr[parent] < arr[leftChild]) {
		parent = leftChild;
	}
	if (rightChild < n && arr[parent] < arr[rightChild]) {
		parent = rightChild;
	}
	if (i != parent) {
		swap(arr + parent, arr + i);
		heapify(arr, n, parent);
	}

}
void heapSort(int* arr,int n) {
	for (int i = (n / 2) - 1; i >= 0; i--) heapify(arr, n, i);
	for (int i = n - 1; i > 0; i--) {
		swap(arr, arr + i);
		heapify(arr, i, 0);
	}
}
void radixSort(int *arr, int n) {
	int result[10000];
	int exp = 1, maxValue = 0;
	for (int i = 0; i < n; i++) {
		if (arr[i] > maxValue)maxValue = arr[i];
	}
	while (maxValue / exp > 0) {
		int bucket[10] = { 0 };
		for (int i = 0; i < n; i++)bucket[arr[i] / exp % 10]++;
		for (int i = 1; i < 10; i++)bucket[i] += bucket[i - 1];
		for (int i = n - 1; i >= 0; i--) {
			result[--bucket[arr[i] / exp % 10]] = arr[i];
		}
		for (int i = 0; i < n; i++)arr[i] = result[i];
		exp *= 10;
	}

}
int main() {

	int n,input;
	double start, end;
	long seed = time(NULL);
	srand(seed);
	printf("배열의크기입력:");
	scanf_s("%d", &n);
	int* arr = (int*)malloc(n * sizeof(int));
	if (arr) {
		*arr = '\0';
		for (int i = 0; i < n; i++) {
			arr[i] = (rand() % n)+1;
		}
	}
	start = (double)clock() / CLOCKS_PER_SEC;
	
	exchageSort(arr, n);
	//mergeSort(arr, 0, n - 1);
	//quickSort(arr, 0, n - 1);
	//heapSort(arr, n);
	//radixSort(arr, n);
	
	end = (double)clock() / CLOCKS_PER_SEC;
	printf("%lf", (end - start));
	free(arr);
	return 0;
}
```
