#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 100

typedef struct {
    int weight;
    int value;
} Item;

int knapsack(int W, Item items[], int n) {
    int dp[MAX_ITEMS + 1][MAX_ITEMS + 1];
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (items[i - 1].weight <= w) {
                int include_item = items[i - 1].value + dp[i - 1][w - items[i - 1].weight];
                int exclude_item = dp[i - 1][w];
                dp[i][w] = (include_item > exclude_item) ? include_item : exclude_item;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    return dp[n][W];
}

int main() {
    int W = 50;
    Item items[] = {
        {10, 60},
        {20, 100},
        {30, 120}
    };
    int n = sizeof(items) / sizeof(items[0]);
    printf("Maximum value in Knapsack = %d\n", knapsack(W, items, n));
    return 0;
}
