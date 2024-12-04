#include "Partition_heap.h"
#include <stdio.h>
#include <stdlib.h>
#define Element_type float

int main() {
    Element_type containerWidth = 10; //max width of the container

    //initialize rectangles
    Rectangle rectangles[] = {
        {4, 3}, {3, 2}, {6, 5}, {5, 3}, {7, 1}
    };
    int n = sizeof(rectangles) / sizeof(rectangles[0]);

   // Call the partition packing function
    float finalHeight = partitionPacking(rectangles, n, containerWidth);
    printf("\nFinal container height: %.2f\n", finalHeight);
    return 0;
}