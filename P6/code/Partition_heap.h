#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "Rectangle_BST.h"

///////////////////////////////////////////////////////////
// Part 2: Partition Structure and Min-Heap Operations
///////////////////////////////////////////////////////////

typedef struct Partition {
    int start_index; // Starting x-coordinate
    int width;       // Width of the partition
    int height;      // Current height
    struct Partition *left;  // Pointer to left neighbor
    struct Partition *right; // Pointer to right neighbor
    int heap_index;           // Current index in the heap
} Partition;

// Min-heap structure for partitions
typedef struct {
    Partition **heap;  // Array of partitions
    int size;          // Current size of the heap
    int capacity;      // Maximum capacity
} MinHeap;

// Create a new partition
Partition* createPartition(int start_index, int width, int height) {
    Partition *part = (Partition*)malloc(sizeof(Partition));
    if (part == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    
    part->start_index = start_index;
    part->width = width;
    part->height = height;
    part->left = NULL;
    part->right = NULL;
    part->heap_index = -1; // Initial value
    return part;
}

// Initialize the min-heap for partitions
MinHeap* initialize_partition_heap(int containerWidth) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (heap == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    heap->heap = (Partition**)malloc(sizeof(Partition*) * 1000); // Initial capacity
    if (heap->heap == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    heap->size = 0;
    heap->capacity = 1000;

    // Create initial partition covering the entire width
    Partition *initial = createPartition(0, containerWidth, 0);
    initial->heap_index = heap->size;
    heap->heap[heap->size++] = initial;
    return heap;
}

// Heapify up to maintain min-heap property
void heapifyUp(MinHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->heap[index]->height < heap->heap[parent]->height) {
            // Swap partitions
            Partition *temp = heap->heap[index];
            heap->heap[index] = heap->heap[parent];
            heap->heap[parent] = temp;

            // Update heap_index
            heap->heap[index]->heap_index = index;
            heap->heap[parent]->heap_index = parent;

            index = parent;
        } else {
            break;
        }
    }
}

// Heapify down to maintain min-heap property
void heapifyDown(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->heap[left]->height < heap->heap[smallest]->height)
        smallest = left;

    if (right < heap->size && heap->heap[right]->height < heap->heap[smallest]->height)
        smallest = right;

    if (smallest != index) {
        // Swap partitions
        Partition *temp = heap->heap[index];
        heap->heap[index] = heap->heap[smallest];
        heap->heap[smallest] = temp;

        // Update heap_index
        heap->heap[index]->heap_index = index;
        heap->heap[smallest]->heap_index = smallest;

        heapifyDown(heap, smallest);
    }
}

// Insert a new partition into the min-heap
void insertPartition(MinHeap *heap, Partition *part) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        Partition **temp = (Partition**)realloc(heap->heap, sizeof(Partition*) * heap->capacity);
        if (temp == NULL) {
            fprintf(stderr, "Memory allocation failed during realloc!\n");
            exit(1);
        }
        heap->heap = temp;
    }

    int i = heap->size;
    heap->heap[i] = part;
    part->heap_index = i;
    heap->size += 1;
    heapifyUp(heap, i);
}

// Extract the partition with the smallest height and remove it from the heap
Partition* extractMinPartition(MinHeap *heap) {
    if (heap->size == 0) return NULL;
    Partition *min = heap->heap[0];

    heap->heap[0] = heap->heap[heap->size - 1];
    heap->heap[0]->heap_index = 0;
    heap->size--;
    heapifyDown(heap, 0);

    printf("\nExtracted partition: start_index=%d, width=%d, height=%d\n",
        min->start_index, min->width, min->height);
    return min;
}

// Remove a specified partition from the heap
void removePartition(MinHeap *heap, Partition *part) {
    int index = part->heap_index;
    if (index >= heap->size || index < 0) return; // Invalid index

    // Replace with the last partition
    heap->heap[index] = heap->heap[heap->size - 1];
    heap->heap[index]->heap_index = index;
    heap->size--;

    // Adjust heap to maintain min-heap property
    heapifyDown(heap, index);
    heapifyUp(heap, index);
}

// Combine the current partition with its left and/or right neighbors if heights match
void combinePartitions(MinHeap *heap, Partition *part) {
    bool merged = false;

    // Check and merge with left neighbor
    if (part->left != NULL && part->left->height == part->height) {
        printf("Merging with left neighbor at index %d.\n", part->left->start_index);
        
        // Update current partition's start_index and width
        part->start_index = part->left->start_index;
        part->width += part->left->width;

        // Remove the left neighbor from the heap
        removePartition(heap, part->left);

        // Update the left pointer
        part->left = part->left->left;
        if (part->left != NULL) {
            part->left->right = part;
        }
        merged = true;
    }

    // Check and merge with right neighbor
    if (part->right != NULL && part->right->height == part->height) {
        printf("Merging with right neighbor at index %d.\n", part->right->start_index);
        
        // Update current partition's width
        part->width += part->right->width;

        // Remove the right neighbor from the heap
        removePartition(heap, part->right);

        // Update the right pointer
        part->right = part->right->right;
        if (part->right != NULL) {
            part->right->left = part;
        }
        merged = true;
    }

    if (merged) {
        // Reinsert the updated partition into the heap to maintain heap properties
        heapifyUp(heap, part->heap_index);
        heapifyDown(heap, part->heap_index);
        printf("Partition after merging: start_index=%d, width=%d, height=%d\n", 
               part->start_index, part->width, part->height);
    }
}


///////////////////////////////////////////////////////////
// Part 3: Main Packing Algorithm and Integration
///////////////////////////////////////////////////////////

// Clean up the entire heap
void cleanupHeap(MinHeap *heap) {
    for(int i = 0; i < heap->size; i++) {
        free(heap->heap[i]);
    }
    free(heap->heap);
    free(heap);
}

// Main packing algorithm using partitions and BST
int partitionPacking(Rectangle *rectangles, int n, int containerWidth) {
    MinHeap *heap = initialize_partition_heap(containerWidth);

    int area_sum = 0;  // Calculate the sum of all areas
    TreeNode* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insertBST(root, rectangles[i]);
        area_sum += rectangles[i].width * rectangles[i].height;
    }

    int remainingRects = n;

    while (remainingRects > 0) {
        Partition *part = extractMinPartition(heap);
        if (part == NULL) break;

        printf("\nSelected partition: start_index=%d, width=%d, height=%d\n", 
               part->start_index, part->width, part->height);

        TreeNode* bestFit = findLargestFit(root, part->width);

        if (bestFit != NULL) {
            Rectangle bestRect = bestFit->rect;

            printf("Placing rectangle: width=%d, height=%d in partition at index %d\n", 
                   bestRect.width, bestRect.height, part->start_index);

            // Place the rectangle
            if (bestRect.width < part->width) {
                // 创建右侧的新分区
                Partition *newPart = createPartition(part->start_index + bestRect.width, part->width - bestRect.width, part->height);
                // 这个分区不需要检查与右侧是否需要合并，因为高度不同
                newPart->left = part;
                newPart->right = part->right;

                if (part->right != NULL) {
                    part->right->left = newPart;
                }

                part->right = newPart;

                insertPartition(heap, newPart); 
                printf("New partition created: start_index=%d, width=%d, height=%d\n", 
                    newPart->start_index, newPart->width, newPart->height);
            }

            // 更新当前分区的高度
            part->height += bestRect.height;
            part->width = bestRect.width;

            // 将更新后的分区重新插入堆中
            insertPartition(heap, part);

            // 从 BST 中删除已放置的矩形
            root = deleteNode(root, bestRect);
            remainingRects--;

            printf("Remaining rectangles to place: %d\n", remainingRects);
            
        } else {
            printf("No suitable rectangle found for partition at index %d. Adjusting partition...\n", part->start_index);
            int slow_height = INT_MAX;
            if(!part->left)
                slow_height = part->right->height;
            else if(!part->right){
                slow_height = part->left->height;
            }
            else{
                slow_height = part->left->height < part->right->height ?
                    part->left->height : part->right->height;
            }
            part->height = slow_height;
            printf("Partition adjusted: start_index=%d,, height=%d\n", 
                part->start_index, part->height);
            insertPartition(heap, part);
        }

        // ：无论是否完美插入，都尝试合并分区
        combinePartitions(heap, part);
        for(int i = 0; i < heap->size; i++)
            printf("\npartition %d's height is %d\n", i, heap->heap[i]->height);
    }

    // 计算最终容器高度
    int finalHeight = 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->heap[i]->height > finalHeight) {
            finalHeight = heap->heap[i]->height;
        }
    }
    
    printf("\n-------------------- Conclusion --------------------\n");
    // printf("Final container height: %d\n", finalHeight);
    // printf("Total area of rectangles: %d\n", area_sum);
    printf("Approximation ratio: %.4f%%\n", (float)finalHeight / (area_sum / (float)containerWidth) * 100);

    cleanupBST(root);
    cleanupHeap(heap); // 清理堆内存

    return finalHeight;  // Final height of the container
}
