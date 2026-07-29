#include <stdio.h>

// Function to perform Selection Sort in C
void selectionSort(int arr[], int n) {
    int i, j, min_idx, temp;

    // Loop through the array
    for (i = 0; i < n - 1; i++) {
        // Assume the current position holds the minimum element
        min_idx = i;

        // Find the smallest element in the remaining unsorted array
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // Optimization: Swap only if a smaller element was found
        if (min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

// Helper function to print the array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[] = {64, 25, 12, 22, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    printArray(arr, n);

    selectionSort(arr, n);

    printf("Sorted array: ");
    printArray(arr, n);

    return 0;
}
