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

// Partition structure definition
typedef struct partition {
    int start_index;
    int width;
    int height;
} Partition;

// Heap structure for partitions
typedef struct Heap {
    Partition **arr; // Array of pointers to partitions
    int size;        // Current number of elements in heap
    int capacity;    // Maximum capacity of heap
} Heap;

// Function to create a new TreeNode
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

// Insert node into BST (sorted by width)
TreeNode* insertBST(TreeNode* root, Rectangle rect) {
    if (root == NULL) {
        return createNode(rect);
    }
    
    if (rect.width < root->rect.width) {
        root->left = insertBST(root->left, rect);
    } else {
        root->right = insertBST(root->right, rect);
    }
    return root;
}

// Find the largest rectangle that fits into targetWidth
TreeNode* findLargestFit(TreeNode* root, int targetWidth){
    if(root == NULL )return NULL;

    TreeNode* result = NULL;

    // If current width is larger than targetWidth, search the left subtree
    if(root->rect.width > targetWidth){
        result = findLargestFit(root->left, targetWidth);
    }
    else{
        // If current node fits, search right subtree for a larger fit
        result = root;
        TreeNode* rightResult = findLargestFit(root->right, targetWidth);
        if(rightResult != NULL && rightResult->rect.width > result->rect.width){
            result = rightResult;
        }
    }
    return result;
}

// Delete specified node from BST
TreeNode* deleteNodeBST(TreeNode* root, Rectangle rect) {
    if (root == NULL) return NULL;
    
    // Search for node to delete
    if (rect.width < root->rect.width) {
        root->left = deleteNodeBST(root->left, rect);
    } else if (rect.width > root->rect.width) {
        root->right = deleteNodeBST(root->right, rect);
    } else {
        // Node found
        
        // Case 1: Leaf node
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        
        // Case 2: Single child
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
        
        // Case 3: Two children
        // Find smallest node in right subtree
        TreeNode* successor = root->right;
        TreeNode* successorParent = root;
        
        while (successor->left != NULL) {
            successorParent = successor;
            successor = successor->left;
        }
        
        if (successorParent != root) {
            successorParent->left = successor->right;
        } else {
            successorParent->right = successor->right;
        }
        
        // Copy successor value to current node
        root->rect = successor->rect;
        free(successor);
    }
    return root;
}

// Clean up entire BST
void cleanupBST(TreeNode* root) {
    if (root != NULL) {
        cleanupBST(root->left);
        cleanupBST(root->right);
        free(root);
    }
}

// Function to create a new Partition
Partition* createPartition(int start_index, int width, int height) {
    Partition* p = (Partition*)malloc(sizeof(Partition));
    if (p == NULL) {
        fprintf(stderr, "Memory allocation failed for Partition!\n");
        exit(1);
    }
    p->start_index = start_index;
    p->width = width;
    p->height = height;
    return p;
}

// Function to create a new Heap
Heap* createHeap(int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if (heap == NULL) {
        fprintf(stderr, "Memory allocation failed for Heap!\n");
        exit(1);
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->arr = (Partition**)malloc(sizeof(Partition*) * capacity);
    if (heap->arr == NULL) {
        fprintf(stderr, "Memory allocation failed for Heap array!\n");
        exit(1);
    }
    return heap;
}

// Swap two partitions in the heap
void swapPartitions(Partition** a, Partition** b) {
    Partition* temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify up to maintain min-heap property based on height
void heapifyUp(Heap* heap, int index) {
    if (index && heap->arr[(index - 1) / 2]->height > heap->arr[index]->height) {
        swapPartitions(&heap->arr[index], &heap->arr[(index - 1) / 2]);
        heapifyUp(heap, (index - 1) / 2);
    }
}

// Heapify down to maintain min-heap property based on height
void heapifyDown(Heap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->arr[left]->height < heap->arr[smallest]->height)
        smallest = left;

    if (right < heap->size && heap->arr[right]->height < heap->arr[smallest]->height)
        smallest = right;

    if (smallest != index) {
        swapPartitions(&heap->arr[index], &heap->arr[smallest]);
        heapifyDown(heap, smallest);
    }
}

// Insert a partition into the heap
void insertHeap(Heap* heap, Partition* p) {
    if (heap->size == heap->capacity) {
        // Resize heap array if needed
        heap->capacity *= 2;
        heap->arr = (Partition**)realloc(heap->arr, sizeof(Partition*) * heap->capacity);
        if (heap->arr == NULL) {
            fprintf(stderr, "Memory reallocation failed for Heap array!\n");
            exit(1);
        }
    }
    heap->arr[heap->size] = p;
    heapifyUp(heap, heap->size);
    heap->size++;
}

// Extract the partition with the minimum height
Partition* extractMinHeap(Heap* heap) {
    if (heap->size <= 0)
        return NULL;
    if (heap->size == 1) {
        heap->size--;
        return heap->arr[0];
    }

    // Store the minimum value, and remove it from heap
    Partition* root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);

    return root;
}

// Clean up the heap
void cleanupHeap(Heap* heap) {
    for(int i = 0; i < heap->size; i++) {
        free(heap->arr[i]);
    }
    free(heap->arr);
    free(heap);
}

// Function to initialize the heap with the initial partition
Heap* initialize_partition_heap(int containerWidth) {
    Heap* heap = createHeap(10); // Initial capacity can be adjusted
    Partition* initial = createPartition(0, containerWidth, 0);
    insertHeap(heap, initial);
    return heap;
}

// Function to find and place the best fitting rectangle
bool findAndPlaceRectangle(Heap* heap, TreeNode** root, Rectangle* bestRect, int* startIndex) {
    if (heap->size == 0)
        return false;

    // Extract the partition with the minimum height
    Partition* current = extractMinHeap(heap);
    int target_width = current->width;
    *startIndex = current->start_index;

    // Find the largest rectangle that fits into target_width
    TreeNode* bestFit = findLargestFit(*root, target_width);
    
    if (bestFit != NULL) {
        // Found a suitable rectangle
        *bestRect = bestFit->rect;

        // Place the rectangle: update partitions
        // Case 1: Rectangle width == target_width
        if (bestFit->rect.width == target_width) {
            // No split needed, partition is fully occupied
            // Merge with adjacent partitions if necessary (not implemented here)
        }
        // Case 2: Rectangle width < target_width
        else {
            // Split the partition into two
            int remaining_width = target_width - bestFit->rect.width;
            Partition* newPartition = createPartition(current->start_index + bestFit->rect.width, remaining_width, current->height);
            insertHeap(heap, newPartition);
            current->width = bestFit->rect.width;
        }

        // Place the rectangle: update the current partition's height
        current->height += bestFit->rect.height;
        // Re-insert the updated partition back into the heap
        insertHeap(heap, current);

        // Remove the placed rectangle from BST
        *root = deleteNodeBST(*root, bestFit->rect);

        return true;
    } else {
        // No suitable rectangle found, adjust the partition's height
        // For simplicity, increase the height and re-insert
        current->height += 1; // Adjust the height by 1 unit
        insertHeap(heap, current);
        return false;
    }
}

// Function to calculate the maximum height from all partitions
int calculateHeightFromHeap(Heap* heap) {
    int maxHeight = 0;
    for(int i = 0; i < heap->size; i++) {
        if(heap->arr[i]->height > maxHeight) {
            maxHeight = heap->arr[i]->height;
        }
    }
    return maxHeight;
}

// Main algorithm: ceiling_height Packing using BST and Partition Heap
int ceiling_heightPacking(Rectangle *rectangles, int n, int containerWidth) {
    // Initialize BST with all rectangles
    TreeNode* root = NULL;
    int area_sum = 0; // Calculate the sum of all areas
    for (int i = 0; i < n; i++) {
        root = insertBST(root, rectangles[i]);
        area_sum += rectangles[i].width * rectangles[i].height;
    }

    // Initialize the partition heap
    Heap* heap = initialize_partition_heap(containerWidth);

    int remainingRects = n;

    while (remainingRects > 0) {
        Rectangle bestRect;
        int startIndex = 0;
        bool placed = findAndPlaceRectangle(heap, &root, &bestRect, &startIndex);
        
        if (placed) {
            printf("Placing rectangle with width: %d, height: %d at index %d\n",
                   bestRect.width, bestRect.height, startIndex);
            remainingRects--;
        } else {
            // If no rectangle was placed, adjust the partition's height
            printf("No suitable rectangle found. Adjusting partition height.\n");
            // In this simplified version, we have already increased the partition's height
            // You can implement more sophisticated height adjustments if needed
        }

        // Optional: Print current partitions and their heights
        /*
        printf("Current Partitions:\n");
        for(int i = 0; i < heap->size; i++) {
            printf("Start: %d, Width: %d, Height: %d\n", 
                   heap->arr[i]->start_index, 
                   heap->arr[i]->width, 
                   heap->arr[i]->height);
        }
        printf("\n");
        */
    }

    // Calculate the final height
    int current_height = calculateHeightFromHeap(heap);

    // Clean up
    cleanupHeap(heap);
    cleanupBST(root);

    // Calculate approximation ratio
    float optimal_height = area_sum / (float)containerWidth;
    float ratio = (current_height / optimal_height) * 100.0f;

    printf("\n--------------------conclusion---------------------\n");
    printf("- Approximation Ratio for ceiling_height packing: %.4f%%\n", ratio);
    
    return current_height;
}

// Example usage
int main() {
    int n, W;
    printf("请输入矩形的数量 n 和容器的宽度 W（用空格分隔）：");
    scanf("%d %d", &n, &W);

    Rectangle* rectangles = (Rectangle*)malloc(sizeof(Rectangle) * n);
    if(rectangles == NULL){
        fprintf(stderr, "Memory allocation failed for rectangles!\n");
        exit(1);
    }

    printf("请输入每个矩形的宽度和高度（每个矩形一行，用空格分隔）：\n");
    for(int i = 0; i < n; i++) {
        printf("矩形 %d: ", i+1);
        scanf("%d %d", &rectangles[i].width, &rectangles[i].height);
    }

    int finalHeight = ceiling_heightPacking(rectangles, n, W);
    printf("最终容器高度: %d\n", finalHeight);

    free(rectangles);
    return 0;
}
