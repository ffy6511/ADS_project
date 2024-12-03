#include "Partition_heap.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int containerWidth; // Maximum width of the container
    int n; // Number of rectangles

    fp = fopen("Generate_input/size_100_short.txt", "r"); // Open input file, please change the file name to your own
    if (fp == NULL) {
        printf("Error: Cannot open input file\n");
        return 1;
    }

    fscanf(fp, "%d", &containerWidth);
    fscanf(fp, "%d", &n);

    Rectangle* rectangles = (Rectangle*)malloc(n * sizeof(Rectangle));
    if (rectangles == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d %d", &rectangles[i].width, &rectangles[i].height);
        
        //check if the rectangle size is valid
        if (rectangles[i].width <= 0 || rectangles[i].height <= 0 || 
            rectangles[i].width > containerWidth) {
            printf("Error: Invalid rectangle size! Width must be > 0 and <= container width, height must be > 0\n");
            free(rectangles);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    int finalHeight = partitionPacking(rectangles, n, containerWidth);  // Call the packing function
    
    printf("- Final height dealing with %d rectangles by Partition_heap packing algorithm: %d\n", n, finalHeight);

    free(rectangles);
    return 0;
}