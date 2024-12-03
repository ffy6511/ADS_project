#include "adjustment.h"
#include <stdio.h>
#include <stdlib.h>

// 函数声明
// BST functions
TreeNode* createNode(Rectangle rect);
TreeNode* insert(TreeNode* root, Rectangle rect);
TreeNode* findLargestFit(TreeNode* root, int targetWidth);
TreeNode* deleteNode(TreeNode* root, Rectangle rect);
void cleanupBST(TreeNode* root);

// Partition and heap functions
Partition* createPartition(int start_index, int width, int height);
MinHeap* initialize_partition_heap(int containerWidth);
Partition* extractMin(MinHeap *heap);
void insertPartition(MinHeap *heap, Partition *part);
void adjust_partition(MinHeap *heap, Partition *part);

// Main packing algorithm
int partitionPacking(Rectangle *rectangles, int n, int containerWidth);

int main() {
    int containerWidth = 10; //max width of the container

    //initialize rectangles
    Rectangle rectangles[] = {
        {4, 3}, {3, 2}, {6, 5}, {5, 3}, {7, 1}
    };
    int n = sizeof(rectangles) / sizeof(rectangles[0]);

   // Call the partition packing function
    int finalHeight = partitionPacking(rectangles, n, containerWidth);
    printf("\nFinal container height: %d\n", finalHeight);
    return 0;
}