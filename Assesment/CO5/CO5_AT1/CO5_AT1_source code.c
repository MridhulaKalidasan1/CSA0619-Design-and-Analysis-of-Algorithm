#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_V 30

typedef struct {
    int num_vertices;
    int adj[MAX_V][MAX_V];
    int degree[MAX_V];
} Graph;

int min_fvs_size = MAX_V + 1;
bool best_fvs[MAX_V];
bool current_fvs[MAX_V];

// Check if graph contains a cycle using DFS
bool dfs_cycle(Graph *g, int v, bool visited[], int parent) {
    visited[v] = true;
    for (int u = 0; u < g->num_vertices; u++) {
        if (g->adj[v][u] && !current_fvs[u]) {
            if (!visited[u]) {
                if (dfs_cycle(g, u, visited, v))
                    return true;
            } else if (u != parent) {
                return true; // Cycle found
            }
        }
    }
    return false;
}

bool has_cycle(Graph *g) {
    bool visited[MAX_V] = {false};
    for (int i = 0; i < g->num_vertices; i++) {
        if (!visited[i] && !current_fvs[i]) {
            if (dfs_cycle(g, i, visited, -1))
                return true;
        }
    }
    return false;
}

// Backtracking solver for FVS
void solve_fvs(Graph *g, int v_idx, int current_size) {
    // Prune if current path exceeds or matches best found size
    if (current_size >= min_fvs_size) {
        return;
    }

    // Base Case: If graph is now acyclic, record optimal solution
    if (!has_cycle(g)) {
        if (current_size < min_fvs_size) {
            min_fvs_size = current_size;
            memcpy(best_fvs, current_fvs, sizeof(current_fvs));
        }
        return;
    }

    if (v_idx >= g->num_vertices) {
        return;
    }

    // Pruning: Skip vertices with degree <= 1 (they cannot be part of any cycle)
    int effective_degree = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        if (g->adj[v_idx][i] && !current_fvs[i]) {
            effective_degree++;
        }
    }

    if (effective_degree <= 1) {
        // Skip this vertex and move to next
        solve_fvs(g, v_idx + 1, current_size);
        return;
    }

    // Branch 1: Include v_idx in Feedback Vertex Set (Remove vertex)
    current_fvs[v_idx] = true;
    solve_fvs(g, v_idx + 1, current_size + 1);
    current_fvs[v_idx] = false; // Backtrack

    // Branch 2: Do NOT include v_idx in Feedback Vertex Set
    solve_fvs(g, v_idx + 1, current_size);
}

int main() {
    Graph g;
    g.num_vertices = 5;
    memset(g.adj, 0, sizeof(g.adj));

    // Graph Construction: Complete graph K4 with 1 additional edge
    // Edges: (0-1), (1-2), (2-0), (2-3), (3-0), (3-4)
    int edges[][2] = {
        {0, 1}, {1, 2}, {2, 0},
        {2, 3}, {3, 0}, {3, 4}
    };
    int num_edges = sizeof(edges) / sizeof(edges[0]);

    for (int i = 0; i < num_edges; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        g.adj[u][v] = 1;
        g.adj[v][u] = 1;
    }

    memset(current_fvs, 0, sizeof(current_fvs));
    memset(best_fvs, 0, sizeof(best_fvs));

    solve_fvs(&g, 0, 0);

    printf("Minimum Feedback Vertex Set Size: %d\n", min_fvs_size);
    printf("Vertices to remove: ");
    for (int i = 0; i < g.num_vertices; i++) {
        if (best_fvs[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
