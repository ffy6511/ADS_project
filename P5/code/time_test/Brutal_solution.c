#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_FILES 5
#define FILE_PREFIX "input/"
#define FILE_SUFFIX ".txt"

int main() {
    int count = 0;
    // Loop over multiple files, assuming naming convention "input_x.txt"
    for (int i = 100; count < NUM_FILES; i += 100, count++) {
        char file_path[200];
        sprintf(file_path, "%sinput_%d.txt", FILE_PREFIX, i);
        FILE* input_file = fopen(file_path, "r");
        
        if (input_file == NULL) {
            printf("Error opening file: %s\n", file_path);
            continue;
        }

        int N;
        float money;
        fscanf(input_file, "%d %f", &N, &money);

        float* prices = (float*)malloc(sizeof(float) * N);
        float* coupons = (float*)malloc(sizeof(float) * N);

        // Read prices and coupons from the file
        for (int i = 0; i < N; i++) {
            fscanf(input_file, "%f", &prices[i]);
        }
        for (int i = 0; i < N; i++) {
            fscanf(input_file, "%f", &coupons[i]);
        }

        fclose(input_file);

        // Initialize cost_queue array to store all possible costs (price - coupon)
        float* cost_queue = (float*)malloc(sizeof(float) * N * N);
        int queue_size = 0;

        // Calculate all possible costs
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cost_queue[queue_size++] = prices[i] - coupons[j];
            }
        }

        // Measure the time it takes to sort the cost_queue
        clock_t start = clock();

        // Bubble sort the cost_queue array in ascending order
        for (int i = 0; i < queue_size - 1; i++) {
            for (int j = 0; j < queue_size - i - 1; j++) {
                if (cost_queue[j] > cost_queue[j + 1]) {
                    float temp = cost_queue[j];
                    cost_queue[j] = cost_queue[j + 1];
                    cost_queue[j + 1] = temp;
                }
            }
        }

        // Now we try to purchase items as long as we have enough money
        int count_item = 0;
        for (int i = 0; i < queue_size; i++) {
            if (money - cost_queue[i] >= 0) {
                count_item++;
                money -= cost_queue[i];
            }
        }

        clock_t end = clock();
        double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

        // Output the results
        printf("File: %s, Items Purchased: %d, Remaining Money: %.0f, Time Taken: %.6f seconds\n",
               file_path, count_item, money, time_taken);

        // Free dynamically allocated memory
        free(prices);
        free(coupons);
        free(cost_queue);
    }

    return 0;
}
