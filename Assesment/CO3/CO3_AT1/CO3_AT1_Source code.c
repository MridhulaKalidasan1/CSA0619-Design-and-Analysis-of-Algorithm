#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Linked List Node Structure ---
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// --- Array Merge Sort Functions ---
void merge_array(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    if (!L || !R) {
        fprintf(stderr, "Memory allocation failed in array merge.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort_array(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_array(arr, l, m);
        merge_sort_array(arr, m + 1, r);
        merge_array(arr, l, m, r);
    }
}

// --- Linked List Merge Sort Functions (Iterative Merge to avoid Stack Overflow) ---
Node* merge_list_iterative(Node* left, Node* right) {
    Node dummy;
    Node* tail = &dummy;
    dummy.next = NULL;

    while (left != NULL && right != NULL) {
        if (left->data <= right->data) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    tail->next = (left != NULL) ? left : right;
    return dummy.next;
}

void split_list(Node* source, Node** front, Node** back) {
    Node* slow = source;
    Node* fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

void merge_sort_list(Node** head_ref) {
    Node* head = *head_ref;
    if (!head || !head->next) return;

    Node* a;
    Node* b;

    split_list(head, &a, &b);
    merge_sort_list(&a);
    merge_sort_list(&b);

    *head_ref = merge_list_iterative(a, b);
}

// --- Helper Utilities ---
void push_node(Node** head_ref, int new_data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed for node creation.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = new_data;
    new_node->next = *head_ref;
    *head_ref = new_node;
}

void free_list(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    int N = 100000;

    // Allocate array on heap to avoid stack allocation limits
    int* arr = (int*)malloc(N * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed for benchmark array.\n");
        return 1;
    }

    Node* head = NULL;

    // Seed RNG for identical benchmark datasets
    srand(42);
    for (int i = 0; i < N; i++) {
        int val = rand() % 1000000;
        arr[i] = val;
        push_node(&head, val);
    }

    printf("Starting benchmark for N = %d elements...\n\n", N);

    // Benchmark Array Merge Sort
    clock_t start_arr = clock();
    merge_sort_array(arr, 0, N - 1);
    clock_t end_arr = clock();
    double time_arr = ((double)(end_arr - start_arr)) / CLOCKS_PER_SEC;

    // Benchmark Linked List Merge Sort
    clock_t start_list = clock();
    merge_sort_list(&head);
    clock_t end_list = clock();
    double time_list = ((double)(end_list - start_list)) / CLOCKS_PER_SEC;

    // Display Benchmark Metrics
    printf("===================================================\n");
    printf("                BENCHMARK RESULTS                  \n");
    printf("===================================================\n");
    printf("Array Merge Sort Execution Time       : %.4f seconds\n", time_arr);
    printf("Linked List Merge Sort Execution Time : %.4f seconds\n\n", time_list);

    printf("===================================================\n");
    printf("            MEMORY OVERHEAD ANALYSIS               \n");
    printf("===================================================\n");
    printf("Array Base Memory Allocation          : %.2f KB\n", (N * sizeof(int)) / 1024.0);
    printf("Array Peak Dynamic Buffer Memory      : %.2f KB\n", (N * sizeof(int)) / 1024.0);
    printf("Linked List Base Allocation           : %.2f KB\n", (N * sizeof(Node)) / 1024.0);
    printf("Linked List Auxiliary Buffer Memory   : 0.00 KB (In-place pointer updates)\n");

    // Cleanup resources
    free(arr);
    free_list(head);

    return 0;
}
