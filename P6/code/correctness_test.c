#include "solution.h"
#include <stdio.h>
#include <stdlib.h>

// 函数声明
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

// 全局变量
int skyline[MAX_WIDTH];

int main() {
    int containerWidth = 10; //max width of the container

    //initialize rectangles
    Rectangle rectangles[] = {
        {4, 3}, {3, 2}, {6, 5}, {5, 3}, {7, 1}
    };
    int n = sizeof(rectangles) / sizeof(rectangles[0]);

    int finalHeight = skylinePacking(rectangles, n, containerWidth); //call the function to pack the rectangles
    printf("\nFinal container height: %d\n", finalHeight);
    return 0;
}