#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_WIDTH 1000

typedef struct {
    int width;
    int height;
} Rectangle;

typedef struct {
    int x;      // x coordinate
    int height; // current height at this x coordinate
} Position;

// Compare rectangles by height in descending order
int compareRectangles(const void *a, const void *b) {
    Rectangle *rectA = (Rectangle *)a;
    Rectangle *rectB = (Rectangle *)b;
    return rectB->height - rectA->height;
}

// Find the position with minimum height that can fit the current rectangle
int findBestFit(Position *positions, int containerWidth, int rectWidth, int *minX) {
    int minHeight = INT_MAX;
    int bestX = -1;
    
    // Try each possible x position
    for (int x = 0; x <= containerWidth - rectWidth; x++) {
        // Find maximum height in the range [x, x+width-1]
        int maxHeight = 0;
        bool canFit = true;
        
        for (int i = x; i < x + rectWidth; i++) {
            if (positions[i].height > maxHeight) {
                maxHeight = positions[i].height;
            }
        }
        
        // If this position has the lowest height so far, update best position
        if (maxHeight < minHeight) {
            minHeight = maxHeight;
            bestX = x;
        }
    }
    
    *minX = bestX;
    return minHeight;
}

int bestFitPacking(Rectangle *rectangles, int n, int containerWidth) {
    qsort(rectangles, n, sizeof(Rectangle), compareRectangles);
    
    // Initialize positions array to track height at each x coordinate
    Position *positions = (Position *)calloc(containerWidth, sizeof(Position));
    for (int i = 0; i < containerWidth; i++) {
        positions[i].x = i;
        positions[i].height = 0;
    }
    
    float area_sum = 0;
    int totalHeight = 0;
    
    // Place each rectangle
    for (int i = 0; i < n; i++) {
        Rectangle currentRect = rectangles[i];
        int bestX;
        int baseHeight = findBestFit(positions, containerWidth, currentRect.width, &bestX);
        
        if (bestX != -1) {
            // Update heights for the width of the rectangle
            int newHeight = baseHeight + currentRect.height;
            for (int x = bestX; x < bestX + currentRect.width; x++) {
                positions[x].height = newHeight;
            }
            
            // Update total height if necessary
            if (newHeight > totalHeight) {
                totalHeight = newHeight;
            }
        }
        
        area_sum += currentRect.width * currentRect.height;
    }
    
    // Calculate and output metrics
    float optimal_height = area_sum / containerWidth;
    float ratio = totalHeight / optimal_height * 100;
    
    printf("\nOptimal height (area/width): %.2f\n", optimal_height);
    printf("Actual container height: %d\n", totalHeight);
    printf("Total rectangle area: %.2f\n", area_sum);
    printf("Approximation Ratio for Best Fit packing: %.2f%%\n", ratio);
    printf("\n--------------------conclusion---------------------\n");

    
    free(positions);
    return totalHeight;
}

int main() {
    FILE *fp;
    int containerWidth; // Maximum width of the container
    int n; // Number of rectangles

    fp = fopen("../Generate_input/size_1000_short.txt", "r"); // Open input file, please change the file name to your own
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
        // Check if the rectangle size is valid
        if (rectangles[i].width <= 0 || rectangles[i].height <= 0 || 
            rectangles[i].width > containerWidth) {
            printf("Error: Invalid rectangle size! Width must be > 0 and <= container width, height must be > 0\n");
            free(rectangles);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    // Call the Best Fit packing function

    int finalHeight = bestFitPacking(rectangles, n, containerWidth);  
    printf("\nFinal container height with Best Fit packing: %d\n", finalHeight);

    // Free the allocated memory for rectangles
    free(rectangles);
    return 0;
}
