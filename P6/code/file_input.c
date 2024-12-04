#include "Partition_heap.h"
#include <stdio.h>
#include <stdlib.h>

#define Element_type float // Change Element_type to int if you want to use integer input

#ifdef Element_type
    #if Element_type == float
        #define ELEMENT_TYPE_FLOAT
    #else
        #define ELEMENT_TYPE_INT
    #endif
#endif

int main() {
    FILE *fp;
    Element_type containerWidth; // Maximum width of the container
    int n; // Number of rectangles

    fp = fopen("Generate_input/size_100_short.txt", "r"); // Open input file, please change the file name to your own
    if (fp == NULL) {
        printf("Error: Cannot open input file\n");
        return 1;
    }

    // Read the container width and number of rectangles from the input file
    #ifdef ELEMENT_TYPE_FLOAT
        fscanf(fp, "%f", &containerWidth);
    #else
        fscanf(fp, "%d", &containerWidth);
    #endif

    fscanf(fp, "%d", &n);

    Rectangle* rectangles = (Rectangle*)malloc(n * sizeof(Rectangle));
    if (rectangles == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
       #ifdef ELEMENT_TYPE_FLOAT
        fscanf(fp, "%f %f", &rectangles[i].width, &rectangles[i].height);
        #else
            fscanf(fp, "%d %d", &rectangles[i].width, &rectangles[i].height);
        #endif

        
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

    float finalHeight = partitionPacking(rectangles, n, containerWidth);  // Call the packing function
    
    printf("- Final height dealing with %d rectangles by Partition_heap packing algorithm: %.2f\n", n, finalHeight);

    free(rectangles);
    return 0;
}