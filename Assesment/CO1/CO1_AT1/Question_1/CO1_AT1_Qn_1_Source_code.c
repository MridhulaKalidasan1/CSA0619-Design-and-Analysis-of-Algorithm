#include <stdio.h>

/**
 * Simulates the decision support evaluation logic recursively.
 * Time Complexity: O(log n)
 * Space Complexity: O(log n) due to the call stack
 */
int evaluateDecisionRecursive(int n, int steps) {
    // Base case: Problem size reduced to 1
    if (n <= 1) {
        return steps;
    }

    // Constant work O(1) evaluation and branching happens here
    // Problem size is halved: n / 2
    return evaluateDecisionRecursive(n / 2, steps + 1);
}

/**
 * Simulates the decision support evaluation logic iteratively.
 * Time Complexity: O(log n)
 * Space Complexity: O(1)
 */
int evaluateDecisionIterative(int n) {
    int steps = 0;
    while (n > 1) {
        n = n / 2; // Reduce problem size by half
        steps++;   // Constant work increment
    }
    return steps;
}

int main() {
    // Array of sample large inputs to showcase scaling efficiency
    int testInputs[] = {16, 1024, 65536, 1048576};
    int numTests = sizeof(testInputs) / sizeof(testInputs[0]);

    printf("--- Intelligent Decision Support Module Evaluation ---\n\n");
    printf("%-15s | %-20s | %-20s\n", "Input Size (n)", "Recursive Steps", "Iterative Steps");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < numTests; i++) {
        int n = testInputs[i];
        int recSteps = evaluateDecisionRecursive(n, 0);
        int iterSteps = evaluateDecisionIterative(n);

        printf("%-15d | %-20d | %-20d\n", n, recSteps, iterSteps);
    }

    return 0;
}
