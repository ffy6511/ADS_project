#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int N, D;
    printf("Enter the number of items/coupons:\n");
    scanf("%d", &N);
    D = 5*N;

    float max_price = D/3;
    float min_price = D/10;
    float max_coupon = min_price - 1;

    float* prices = (float*)malloc(sizeof(float) * N);
    float* coupons = (float*)malloc(sizeof(float) * N);

    // Generate random prices and coupons, ensuring prices are greater than the max coupon
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        prices[i] = (rand() / (float)RAND_MAX) * (max_price - min_price) + min_price;
        coupons[i] = (rand() / (float)RAND_MAX) * max_coupon;
    }
    FILE* output_file = fopen("input_500.txt","w");

    fprintf(output_file, "%d %d\n", N, D);
    for (int i = 0; i < N; i++) {
        fprintf(output_file, "%.0f ", prices[i]);
    }
    fprintf(output_file, "\n");
    for (int i = 0; i < N; i++) {
        fprintf(output_file, "%.0f ", coupons[i]);
    }
    fprintf(output_file, "\n");

    fclose(output_file);
    free(prices);
    free(coupons);
    return 0;
}