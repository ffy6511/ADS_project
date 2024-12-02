#include "solution.h"
#include <stdio.h>
#include <stdlib.h>

// Function declarations
TreeNode* createNode(Rectangle rect);
TreeNode* insert(TreeNode* root, Rectangle rect);
TreeNode* findLargestFit(TreeNode* root, int targetWidth);
TreeNode* deleteNode(TreeNode* root, Rectangle rect);
void cleanupBST(TreeNode* root);
void initializeceiling_height(int width);
void printceiling_height(int containerWidth);
int findMaxWidth(int containerWidth, int* startIndex);
void placeRectangle(int startIndex, int rectWidth, int rectHeight);
int calculateHeight(int containerWidth);
void adjustceiling_height(int startIndex, int width, int containerWidth);
int ceiling_heightPacking(Rectangle *rectangles, int n, int containerWidth);

// Global variable
int ceiling_height[MAX_WIDTH];

int main() {
    FILE *fp;
    int containerWidth; // Maximum width of the container
    int n; // Number of rectangles

    fp = fopen("Generate_input/size_10.txt", "r"); // Open input file, please change the file name to your own
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

    int finalHeight = ceiling_heightPacking(rectangles, n, containerWidth);  // Call the ceiling_height packing function
    
    printf("- Final height dealing with %d rectangles by ceiling_height packing algorithm: %d\n", n, finalHeight);

    free(rectangles);
    return 0;
}