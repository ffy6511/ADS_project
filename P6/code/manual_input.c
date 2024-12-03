#include "Partition_heap.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int containerWidth; //maximum width of the container
    int n; //number of rectangles

    printf("Enter container width: ");
    scanf("%d", &containerWidth);

    printf("Enter number of rectangles: ");
    scanf("%d", &n);

    Rectangle* rectangles = (Rectangle*)malloc(n * sizeof(Rectangle));
    if (rectangles == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }

    // Read input rectangles
    for (int i = 0; i < n; i++) {
        printf("Enter width and height for rectangle %d (format: width height): ", i + 1);
        scanf("%d %d", &rectangles[i].width, &rectangles[i].height);
        
        // Check for invalid input
        if (rectangles[i].width <= 0 || rectangles[i].height <= 0 || 
            rectangles[i].width > containerWidth) {
            printf("Error: Invalid rectangle size! Width must be > 0 and <= container width, height must be > 0\n");
            free(rectangles);
            return 1;
        }
    }

    int finalHeight = partitionPacking(rectangles, n, containerWidth); // Pack rectangles into container
    printf("Final height dealing with %d rectangles by Partition packing algorithm:%d\n", n,finalHeight);

    free(rectangles); // Free memory
    return 0;
}