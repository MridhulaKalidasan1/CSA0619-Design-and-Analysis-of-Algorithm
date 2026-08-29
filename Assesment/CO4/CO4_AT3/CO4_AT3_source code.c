#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_STATIONS 4
#define V 4

// --- 1. ASSEMBLY LINE SCHEDULING (DP) ---
int min(int a, int b) { return (a < b) ? a : b; }

void assemblyLineScheduling(int a[][NUM_STATIONS], int t[][NUM_STATIONS - 1], int e[], int x[]) {
    int T1[NUM_STATIONS], T2[NUM_STATIONS];

    T1[0] = e[0] + a[0][0];
    T2[0] = e[1] + a[1][0];

    for (int j = 1; j < NUM_STATIONS; j++) {
        T1[j] = min(T1[j - 1] + a[0][j], T2[j - 1] + t[1][j - 1] + a[0][j]);
        T2[j] = min(T2[j - 1] + a[1][j], T1[j - 1] + t[0][j - 1] + a[1][j]);
    }

    int final_cost = min(T1[NUM_STATIONS - 1] + x[0], T2[NUM_STATIONS - 1] + x[1]);
    printf("Assembly Line Scheduling (DP) Min Cost: %d\n", final_cost);
}

// --- 2. MINIMUM SPANNING TREE (KRUSKAL'S GREEDY) ---
struct Edge {
    int src, dest, weight;
};

struct Subset {
    int parent, rank;
};

int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void Union(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int compareEdges(const void* a, const void* b) {
    return ((struct Edge*)a)->weight - ((struct Edge*)b)->weight;
}

void kruskalMST(struct Edge edges[], int E) {
    qsort(edges, E, sizeof(edges[0]), compareEdges);
    struct Subset* subsets = (struct Subset*)malloc(V * sizeof(struct Subset));

    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    int mst_weight = 0, e = 0, i = 0;
    while (e < V - 1 && i < E) {
        struct Edge next_edge = edges[i++];
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y) {
            mst_weight += next_edge.weight;
            Union(subsets, x, y);
            e++;
        }
    }
    printf("Kruskal's MST (Greedy) Min Cost: %d\n", mst_weight);
    free(subsets);
}

int main() {
    // Assembly Line Data
    int a[2][NUM_STATIONS] = {{4, 5, 3, 2}, {2, 10, 1, 4}};
    int t[2][NUM_STATIONS - 1] = {{0, 7, 4}, {0, 1, 2}};
    int e[] = {10, 12};
    int x[] = {18, 7};

    assemblyLineScheduling(a, t, e, x);

    // Graph Data for Kruskal
    struct Edge edges[] = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5}, {1, 3, 15}, {2, 3, 4}
    };
    int E = sizeof(edges) / sizeof(edges[0]);

    kruskalMST(edges, E);

    return 0;
}
