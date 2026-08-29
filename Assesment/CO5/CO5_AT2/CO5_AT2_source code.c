#include <stdio.h>
#include <stdbool.h>

#define MAX_V 20

// Global variables to track optimal solution
int min_size;
int best_set[MAX_V];

// Check if current_set forms a valid dominating set
bool is_dominating_set(int current_set[], int current_size, int adj[MAX_V][MAX_V], int N) {
    bool covered[MAX_V] = {false};

    // Mark vertices in set and their neighbors as covered
    for (int i = 0; i < current_size; i++) {
        int u = current_set[i];
        covered[u] = true;
        for (int v = 0; v < N; v++) {
            if (adj[u][v] == 1) {
                covered[v] = true;
            }
        }
    }

    // Verify all vertices are covered
    for (int i = 0; i < N; i++) {
        if (!covered[i]) {
            return false;
        }
    }
    return true;
}

// Backtracking function for subset generation with pruning
void backtrack(int index, int current_set[], int current_size, int adj[MAX_V][MAX_V], int N) {
    // Pruning 1: Bounding based on current subset size
    if (current_size >= min_size) {
        return;
    }

    // Base case: Evaluated all vertices
    if (index == N) {
        if (is_dominating_set(current_set, current_size, adj, N)) {
            min_size = current_size;
            for (int i = 0; i < current_size; i++) {
                best_set[i] = current_set[i];
            }
        }
        return;
    }

    // Branch 1: INCLUDE vertex index
    current_set[current_size] = index;
    backtrack(index + 1, current_set, current_size + 1, adj, N);

    // Branch 2: EXCLUDE vertex index
    backtrack(index + 1, current_set, current_size, adj, N);
}

int main() {
    int N = 5;

    // Example Graph (Path Graph: 0 - 1 - 2 - 3 - 4)
    int adj[MAX_V][MAX_V] = {
        {0, 1, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1},
        {0, 0, 0, 1, 0}
    };

    min_size = N + 1;
    int current_set[MAX_V];

    backtrack(0, current_set, 0, adj, N);

    printf("Minimum Dominating Set Size: %d\n", min_size);
    printf("Vertices in Minimum Dominating Set: { ");
    for (int i = 0; i < min_size; i++) {
        printf("%d ", best_set[i]);
    }
    printf("}\n");

    return 0;
}
