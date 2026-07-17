#include <stdio.h>
#include <math.h>

/**
 * Simulates the distributed log processing time metrics.
 * Compounding work: 2 * T(n/2) + log2(n)
 */
double simulateLogProcessing(double n) {
    // Base Case: 1 log record left, constant processing work
    if (n <= 1.0) {
        return 1.0;
    }

    // Logarithmic work done during the synchronization phase
    double mergeWork = log2(n);

    // Divide work into two parallelizable streams
    double subProblemWork = 2.0 * simulateLogProcessing(n / 2.0);

    return subProblemWork + mergeWork;
}

int main() {
    // Test dataset sizes representing expanding log files
    double logVolumes[] = {16.0, 256.0, 4096.0, 65536.0, 1048576.0};
    int numTests = sizeof(logVolumes) / sizeof(logVolumes[0]);

    printf("--- Distributed Log Processing Platform Simulation ---\n\n");
    printf("%-20s | %-25s | %-20s\n", "Log Records (n)", "Total Execution Units", "Units / n Ratio");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < numTests; i++) {
        double n = logVolumes[i];
        double totalWork = simulateLogProcessing(n);

        // Ratio demonstrates that the execution converges cleanly onto a linear constant
        double ratio = totalWork / n;

        printf("%-20.0f | %-25.2f | %-20.4f\n", n, totalWork, ratio);
    }

    return 0;
}
