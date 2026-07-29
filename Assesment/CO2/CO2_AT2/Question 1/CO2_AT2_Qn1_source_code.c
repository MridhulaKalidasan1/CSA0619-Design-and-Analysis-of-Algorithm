#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point p1;
    Point p2;
} Edge;

// Computes the cross product of vectors (p2 - p1) and (p3 - p1)
double cross_product(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

// Brute-Force Convex Hull Execution
void brute_force_convex_hull(Point points[], int n) {
    if (n < 3) {
        printf("Convex hull requires at least 3 points.\n");
        return;
    }

    Edge hull_edges[n * n];
    int edge_count = 0;

    // Outer loops: Iterate over every pair of points
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            bool all_positive = true;
            bool all_negative = true;

            // Inner loop: Check positions of all other points relative to line (points[i], points[j])
            for (int k = 0; k < n; k++) {
                if (k == i || k == j) continue;

                double val = cross_product(points[i], points[j], points[k]);

                if (val < 0) all_positive = false;
                if (val > 0) all_negative = false;
            }

            // If all points lie on one side (or are collinear)
            if (all_positive || all_negative) {
                hull_edges[edge_count].p1 = points[i];
                hull_edges[edge_count].p2 = points[j];
                edge_count++;
            }
        }
    }

    // Display boundary results
    printf("--- Convex Hull Edges Found (%d edges) ---\n", edge_count);
    for (int i = 0; i < edge_count; i++) {
        printf("Edge %d: (%.2f, %.2f) -> (%.2f, %.2f)\n",
               i + 1,
               hull_edges[i].p1.x, hull_edges[i].p1.y,
               hull_edges[i].p2.x, hull_edges[i].p2.y);
    }
}

int main() {
    // Sample satellite sensor coordinates (GIS grid projection)
    Point points[] = {
        {0.0, 3.0}, {1.0, 1.0}, {2.0, 2.0}, {4.0, 4.0},
        {0.0, 0.0}, {1.0, 2.0}, {3.0, 1.0}, {3.0, 3.0}
    };
    int n = sizeof(points) / sizeof(points[0]);

    printf("Processing %d geographic points...\n", n);
    brute_force_convex_hull(points, n);

    return 0;
}
