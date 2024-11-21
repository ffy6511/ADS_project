#include <stdio.h>
#include <stdlib.h>

int main() {
    // Read in the number of items/coupons (N) and the available money (money)
    int N;
    float money;
    scanf("%d %f", &N, &money);

    // Allocate memory for the prices and coupons arrays
    float* prices = (float*)malloc(sizeof(float) * N);
    float* coupons = (float*)malloc(sizeof(float) * N);

    // Read in the prices and coupons
    for (int i = 0; i < N; i++) {
        scanf("%f", &prices[i]);
    }
    for (int i = 0; i < N; i++) {
        scanf("%f", &coupons[i]);
    }

    // Calculate all possible costs and store them in the cost_queue array
    float cost_queue[N*N];
    int queue_size = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cost_queue[queue_size++] = prices[i] - coupons[j];
        }
    }

    // Sort the cost_queue array in ascending order using bubble sort
    for (int i = 0; i < queue_size - 1; i++) {
        for (int j = 0; j < queue_size - i - 1; j++) {
            if (cost_queue[j] > cost_queue[j+1]) {
                float temp = cost_queue[j];
                cost_queue[j] = cost_queue[j+1];
                cost_queue[j+1] = temp;
            }
        }
    }

    // Iterate through the sorted costs, purchasing items as long as the remaining money is sufficient
    int count_item = 0;
    for (int i = 0; i < queue_size; i++) {
        if (money - cost_queue[i] >= 0) {
            count_item++;
            money -= cost_queue[i];
        }
    }

    // Print the number of items purchased and the remaining money
    printf("%d %.0f\n", count_item, money);

    // Free the dynamically allocated memory
    free(prices);
    free(coupons);
    return 0;
}