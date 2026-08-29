#include <stdio.h>
#include <string.h>

// Helper function to find the minimum of three integers
int min(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    if (b <= a && b <= c) return b;
    return c;
}

int minEditDistance(char* s1, char* s2) {
    int m = strlen(s1);
    int n = strlen(s2);

    // Create a DP table of size (m+1) x (n+1)
    int dp[m + 1][n + 1];

    // Fill base cases
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) {
                dp[i][j] = j; // Min operations = j insertions
            } else if (j == 0) {
                dp[i][j] = i; // Min operations = i deletions
            } else if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // No edit needed
            } else {
                dp[i][j] = 1 + min(
                    dp[i][j - 1],   // Insert
                    dp[i - 1][j],   // Delete
                    dp[i - 1][j - 1] // Substitute
                );
            }
        }
    }

    return dp[m][n];
}

int main() {
    char s1[] = "sunday";
    char s2[] = "saturday";

    int result = minEditDistance(s1, s2);
    printf("Min Operations = %d\n", result);

    return 0;
}
