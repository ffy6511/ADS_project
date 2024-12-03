#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Rectangle structure definition
typedef struct {
    int width;
    int height;
} Rectangle;

// BST node structure
typedef struct TreeNode {
    Rectangle rect;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Partition structure
typedef struct partition {
    int start_index; // Starting x-coordinate
    int width;       // Width of the partition
    int height;      // Current height
    struct partition *left;  // Pointer to left neighbor
    struct partition *right; // Pointer to right neighbor
} Partition;

// Min-heap structure for partitions
typedef struct {
    Partition **heap;  // Array of partitions
    int size;          // Current size of the heap
    int capacity;      // Maximum capacity
} MinHeap;

// Create a new BST node
TreeNode* createNode(Rectangle rect) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    node->rect = rect;
    node->left = node->right = NULL;
    return node;
}

// Insert a rectangle into the BST (sorted by width)
TreeNode* insert(TreeNode* root, Rectangle rect) {
    if (root == NULL) {
        return createNode(rect);
    }
    
    if (rect.width < root->rect.width) {
        root->left = insert(root->left, rect);
    } else {
        root->right = insert(root->right, rect);
    }
    return root;
}

// Find the largest rectangle that fits within the given width
TreeNode* findLargestFit(TreeNode* root, int targetWidth) {
    if (root == NULL) return NULL;

    TreeNode* result = NULL;

    if (root->rect.width > targetWidth) {
        result = findLargestFit(root->left, targetWidth);
    } else {
        result = root;
        TreeNode* rightResult = findLargestFit(root->right, targetWidth);
        if (rightResult != NULL && rightResult->rect.width > result->rect.width) {
            result = rightResult;
        }
    }
    return result;
}

// Delete a specified rectangle from the BST
TreeNode* deleteNode(TreeNode* root, Rectangle rect) {
    if (root == NULL) return NULL;

    if (rect.width < root->rect.width) {
        root->left = deleteNode(root->left, rect);
    } else if (rect.width > root->rect.width) {
        root->right = deleteNode(root->right, rect);
    } else {
        // Node found
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest in the right subtree)
        TreeNode* successor = root->right;
        while (successor->left != NULL) {
            successor = successor->left;
        }
        root->rect = successor->rect;
        root->right = deleteNode(root->right, successor->rect);
    }
    return root;
}

// Clean up the entire BST
void cleanupBST(TreeNode* root) {
    if (root != NULL) {
        cleanupBST(root->left);
        cleanupBST(root->right);
        free(root);
    }
}

// Create a new partition
Partition* createPartition(int start_index, int width, int height) {
    Partition *part = (Partition*)malloc(sizeof(Partition));
    part->start_index = start_index;
    part->width = width;
    part->height = height;
    part->left = NULL;
    part->right = NULL;
    return part;
}

// Initialize the min-heap for partitions
MinHeap* initialize_partition_heap(int containerWidth) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->heap = (Partition**)malloc(sizeof(Partition*) * 1000);
    heap->size = 0;
    heap->capacity = 1000;

    // Create initial partition covering the entire width
    Partition *initial = createPartition(0, containerWidth, 0);
    heap->heap[0] = initial;
    heap->size = 1;
    return heap;
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
        Partition *temp = heap->heap[index];
        heap->heap[index] = heap->heap[smallest];
        heap->heap[smallest] = temp;
        heapifyDown(heap, smallest);
    }
}

// Extract the partition with the smallest height
Partition* extractMin(MinHeap *heap) {
    if (heap->size == 0) return NULL;
    Partition *min = heap->heap[0];
    printf("\nExtracted partition with minimum height: start_index=%d, width=%d, height=%d\n",
        min->start_index, min->width, min->height);
    heap->heap[0] = heap->heap[--heap->size];
    heapifyDown(heap, 0);
    return min;
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


    heap->heap[heap->size] = part;

    int i = heap->size;
    while (i > 0 && heap->heap[i]->height < heap->heap[(i - 1) / 2]->height) {
        Partition *temp = heap->heap[i];
        heap->heap[i] = heap->heap[(i - 1) / 2];
        heap->heap[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
    heap->size += 1;
}

// Adjust the height of a partition and merge neighbors if needed
void adjust_partition(MinHeap *heap, Partition *part) {
    int newHeight = INT_MAX;
    if (part->left != NULL) newHeight = part->left->height;
    if (part->right != NULL && part->right->height < newHeight) newHeight = part->right->height;

    printf("Adjusting partition at index %d. New height set to %d.\n", part->start_index, newHeight);

    part->height = newHeight;

    // Merge with left
    if (part->left != NULL && part->left->height == newHeight) {
        printf("Merging with left neighbor at index %d.\n", part->left->start_index);
        part->start_index = part->left->start_index;
        part->width += part->left->width;
        part->left = part->left->left;
    }

    // Merge with right
    if (part->right != NULL && part->right->height == newHeight) {
        printf("Merging with right neighbor at index %d.\n", part->right->start_index);
        part->width += part->right->width;
        part->right = part->right->right;
    }

    // Reinsert updated partition into the heap
    insertPartition(heap, part);
    printf("Partition after adjustment: start_index=%d, width=%d, height=%d\n", 
        part->start_index, part->width, part->height);
}

// Main packing algorithm using partitions and BST
int partitionPacking(Rectangle *rectangles, int n, int containerWidth) {
    MinHeap *heap = initialize_partition_heap(containerWidth);

    int area_sum = 0;  // Calculate the sum of all areas
    TreeNode* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, rectangles[i]);
        area_sum += rectangles[i].width * rectangles[i].height;
    }

    int remainingRects = n;

    while (remainingRects > 0) {
        Partition *part = extractMin(heap);
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
                Partition *newPart = createPartition(part->start_index + bestRect.width, part->width - bestRect.width, part->height);
                insertPartition(heap, newPart);
                 printf("New partition created: start_index=%d, width=%d, height=%d\n", 
                       newPart->start_index, newPart->width, newPart->height);
            }

            part->height += bestRect.height;

            insertPartition(heap, part);

            root = deleteNode(root, bestRect);
            remainingRects--;
            printf("Remaining rectangles to place: %d\n", remainingRects);
            for(int i = 0; i < heap->size; i++)
                printf("partitin %d 's height is %d\n",i,heap->heap[i]->height);
        } else {
            printf("No suitable rectangle found for partition at index %d. Adjusting partition...\n", part->start_index);
            adjust_partition(heap, part);
            printf("Partition adjusted: start_index=%d, width=%d, height=%d\n", 
                   part->start_index, part->width, part->height);
        }
    }

    int finalHeight = 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->heap[i]->height > finalHeight) {
            finalHeight = heap->heap[i]->height;
        }
    }
    printf("\n-------------------- Final Output --------------------\n");
    printf("Final container height: %d\n", finalHeight);
    printf("Total area of rectangles: %d\n", area_sum);
    printf("Approximation ratio: %.4f%%\n", (float)finalHeight / (area_sum / (float)containerWidth) * 100);


    cleanupBST(root);
    return finalHeight;  // Final height of the container
}
