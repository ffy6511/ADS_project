#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Constants */
#define NUM_FILES 5         // number of test files to process
#define FILE_PREFIX "input/"  // prefix for input file paths
#define FILE_SUFFIX ".txt"    // suffix for input file paths

/* global variables */
int N;      // number of items and coupons
float money;

// record relevant item's min_cost by available coupon
typedef struct item {
    int price_id;    // index of the item price
    int coupon_id;   // index of available coupon
    float min_cost;  // current least cost
} item;

// min_heap of items
typedef struct min_heap {
    item* items;     // array of items in heap
    int size;        // current size of heap
} min_heap;

/* function declaration */
void swap(item* a, item* b);                         // swap a and b
void heapify(min_heap* heap, int i);                 // Maintain the properties of the heap
float delete_min(min_heap* heap, float* prices, float* coupons);  // Get and update the smallest element
void initialize(min_heap* heap, float* prices, float* coupons);   // Initialize the small heap

/* main function */
int main() {
    int count = 0;
    // iterate through different input sizes (1000, 2000, 3000, etc.)
    for (int i = 1000; count < NUM_FILES; i += 1000, count += 1) {
        char file_path[200];
        
        // start timing
        clock_t start = clock();

        // run 100 iterations for each file size to get average performance
        for (int j = 0; j < 100; j++) {
            sprintf(file_path, "%sinput_%d.txt", FILE_PREFIX, i);
            FILE* input_file = fopen(file_path, "r");

            // check if file exists
            if (input_file == NULL) {
                printf("Error opening file: %s\n", file_path);
                continue;
            }

            // read input parameters
            fscanf(input_file, "%d %f", &N, &money);

            /* read inputs of prices and coupons */
            float* prices = (float*)malloc(sizeof(float) * N);
            float* coupons = (float*)malloc(sizeof(float) * N);

            for (int j = 0; j < N; j++) {
                fscanf(input_file, "%f", &prices[j]);
            }
            for (int j = 0; j < N; j++) {
                fscanf(input_file, "%f", &coupons[j]);
            }

            fclose(input_file);

            // Sort coupons in descending order using bubble sort
            for (int j = 0; j < N - 1; j++) {
                for (int k = 0; k < N - j - 1; k++) {
                    if (coupons[k] < coupons[k + 1]) {
                        float temp = coupons[k];
                        coupons[k] = coupons[k + 1];
                        coupons[k + 1] = temp;
                    }
                }
            }

            min_heap heap;
            initialize(&heap, prices, coupons);

            // calculate maximum items that can be bought
            int count_item = 0;
            while (heap.size > 0) {
                float min_cost = delete_min(&heap, prices, coupons);
                if (money - min_cost >= 0) {
                    count_item++;
                    money -= min_cost;
                } else {
                    break;
                }
            }

            // reset heap for next iteration
            heap.size = N;
            for (int k = 0; k < N; k++) {
                heap.items[k].coupon_id = 0;
                heap.items[k].min_cost = prices[k] - coupons[0];
            }
            money = 1000000;

            // free allocated memory
            free(prices);
            free(coupons);
            free(heap.items);
        }

        // calculate and print execution time
        clock_t end = clock();
        double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
        printf("File: %s, Time taken: %.6f seconds\n", file_path, time_taken);
    }
    return 0;
}

/* Function implementation */
// Swap two elements in the heap
void swap(item* a, item* b) {
    item temp = *a;
    *a = *b;
    *b = temp;
}

// Adjust downward from position i to maintain the nature of the small top heap
void heapify(min_heap* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;    // left child
    int right = 2 * i + 2;   // right child

    // find the min among i, left, right
    if (left < heap->size && heap->items[left].min_cost < heap->items[smallest].min_cost)
        smallest = left;
    if (right < heap->size && heap->items[right].min_cost < heap->items[smallest].min_cost)
        smallest = right;

    // If the minimum value is not i, swap and recursively adjust
    if (smallest != i) {
        swap(&heap->items[i], &heap->items[smallest]);
        heapify(heap, smallest);
    }
}

// Take out the smallest element and update the heap
float delete_min(min_heap* heap, float* prices, float* coupons) {
    if (heap->size == 0) return -1;  // If the heap is empty, -1 is returned
    float min_cost = heap->items[0].min_cost;  // Save minimum spend

    // Update the top element of the heap to the next state (use the next coupon)
    heap->items[0].coupon_id++;
    if (heap->items[0].coupon_id < N) {
        // calculate the new min_cost
        heap->items[0].min_cost = prices[heap->items[0].price_id] - coupons[heap->items[0].coupon_id];
    } else {
        // The coupon is used up, remove the item from the heap
      heap->items[0].min_cost = prices[heap->items[0].price_id];
    }

    heapify(heap, 0);  // Maintain heap properties
    return min_cost;
}

// Initialize the small top heap
void initialize(min_heap* heap, float* prices, float* coupons) {
    heap->size = N;
    heap->items = (item*)malloc(sizeof(item) * N);

    // Initialize the initial state of each item (using the maximum coupon)
    for (int i = 0; i < N; i++) {
        heap->items[i].price_id = i;        // just copy the price id
        heap->items[i].coupon_id = 0;       // initial using the biggest coupon
        heap->items[i].min_cost = prices[i] - coupons[0];
    }

    // Build a heap from the bottom up
    for (int i = N / 2 - 1; i >= 0; i--) {
        heapify(heap, i);
    }
}