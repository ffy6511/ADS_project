#include "solution.h"
#include <stdio.h>
#include <stdlib.h>

// Function declarations
TreeNode* createNode(Rectangle rect);
TreeNode* insert(TreeNode* root, Rectangle rect);
TreeNode* findLargestFit(TreeNode* root, int targetWidth);
TreeNode* deleteNode(TreeNode* root, Rectangle rect);
void cleanupBST(TreeNode* root);
void initializeSkyline(int width);
void printSkyline(int containerWidth);
int findMaxWidth(int containerWidth, int* startIndex);
void placeRectangle(int startIndex, int rectWidth, int rectHeight);
int calculateHeight(int containerWidth);
void adjustSkyline(int startIndex, int width, int containerWidth);
int skylinePacking(Rectangle *rectangles, int n, int containerWidth);

// Global variable
int skyline[MAX_WIDTH];

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

    int finalHeight = skylinePacking(rectangles, n, containerWidth); // Pack rectangles into container
    printf("\nFinal container height: %d\n", finalHeight);

    free(rectangles); // Free memory
    return 0;
}