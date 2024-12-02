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

// Define maximum width for ceiling_height
#define MAX_WIDTH 1000
int ceiling_height[MAX_WIDTH];

// BST operations
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

// Insert node (sort by width)
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

TreeNode* findLargestFit(TreeNode* root, int targetWidth){
    if(root == NULL )return NULL;

    TreeNode* result = NULL;

    //if current width is already larger than targetwidth, search the left subtree
    if(root->rect.width > targetWidth){
        result = findLargestFit(root->left, targetWidth);
    }
    else{
        // If current node fits, search right subtree
        result = root;
        TreeNode* rightResult = findLargestFit(root->right, targetWidth);
        if(rightResult != NULL && rightResult->rect.width > result->rect.width){
            result = rightResult;
        }
    }
    return result;
}

// Delete specified node from BST
TreeNode* deleteNode(TreeNode* root, Rectangle rect) {
    if (root == NULL) return NULL;
    
    // Search for node to delete
    if (rect.width < root->rect.width) {
        root->left = deleteNode(root->left, rect);
    } else if (rect.width > root->rect.width) {
        root->right = deleteNode(root->right, rect);
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

// ceiling_height related functions
void initializeceiling_height(int width) {
    for (int i = 0; i < width; i++) {
        ceiling_height[i] = 0;
    }
}

void printceiling_height(int containerWidth) {
    printf("Current ceiling_height: ");
    for (int i = 0; i < containerWidth; i++) {
        printf("%d ", ceiling_height[i]);
    }
    printf("\n");
}

// Find lowest point in ceiling_height and its maximum width
int findMaxWidth(int containerWidth, int* startIndex) {
    // First find the minimum height
    int minHeight = ceiling_height[0];
    for (int i = 0; i < containerWidth; i++) {
        if (ceiling_height[i] < minHeight) {
            minHeight = ceiling_height[i];
        }
    }
    
    // Then find widest area at minimum height
    int maxSpace = 0;
    int currentStart = 0;
    int currentWidth = 0;
    
    for (int i = 0; i < containerWidth; i++) {
        if (ceiling_height[i] == minHeight) {
            if (currentWidth == 0) {
                currentStart = i;
            }
            currentWidth++;
        } else {
            if (currentWidth > maxSpace) {
                maxSpace = currentWidth;
                *startIndex = currentStart;
            }
            currentWidth = 0;
        }
    }
    
    // Handle last segment
    if (currentWidth > maxSpace) {
        maxSpace = currentWidth;
        *startIndex = currentStart;
    }
    
    // If no continuous area found, it's a single point
    if (maxSpace == 0) {
        for (int i = 0; i < containerWidth; i++) {
            if (ceiling_height[i] == minHeight) {
                *startIndex = i;
                return 1;  // Return width of 1
            }
        }
    }
    
    return maxSpace;
}

void placeRectangle(int startIndex, int rectWidth, int rectHeight) {
    for (int i = startIndex; i < startIndex + rectWidth && i < MAX_WIDTH; i++) {
        ceiling_height[i] += rectHeight;
    }
}

int calculateHeight(int containerWidth) {
    int maxHeight = 0;
    for (int i = 0; i < containerWidth; i++) {
        if (ceiling_height[i] > maxHeight) {
            maxHeight = ceiling_height[i];
        }
    }
    return maxHeight;
}

// Improved ceiling_height adjustment strategy
void adjustceiling_height(int startIndex, int width, int containerWidth) {
    // Find the minimum height from the neighboring regions
    // For left side, check the height at startIndex - 1, if it's within bounds
    int leftHeight = (startIndex > 0) ? ceiling_height[startIndex - 1] : INT_MAX;

    // For right side, check the height at startIndex + width, if it's within bounds
    int rightHeight = (startIndex + width < containerWidth) ? ceiling_height[startIndex + width] : INT_MAX;

    // The target height is the minimum of the two neighboring heights
    int targetHeight = (leftHeight < rightHeight) ? leftHeight : rightHeight;

    // Now adjust the ceiling_height in the current region
    for (int i = startIndex; i < startIndex + width && i < containerWidth; i++) {
        // If the current section's height is less than the target, adjust it to the target
        if (ceiling_height[i] < targetHeight) {
            ceiling_height[i] = targetHeight;
        }
    }
}



// Main algorithm: ceiling_height Packing using BST
int ceiling_heightPacking(Rectangle *rectangles, int n, int containerWidth) {
    initializeceiling_height(containerWidth);
    
    int area_sum = 0; //calculate the sum of all areas
    // Build BST
    TreeNode* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, rectangles[i]);
        area_sum += rectangles[i].width * rectangles[i].height;
    }
    
    int remainingRects = n;
    
    while (remainingRects > 0) {
        int startIndex = 0;
        int maxWidth = findMaxWidth(containerWidth, &startIndex);
        printf("Current maxWidth: %d\n", maxWidth);
        
        // Find suitable rectangle
        TreeNode* bestFit = findLargestFit(root, maxWidth);
        
        if (bestFit != NULL) {
            Rectangle bestRect = bestFit->rect;
            printf("Placing rectangle with width: %d, height: %d at index %d\n\n",
                   bestRect.width, bestRect.height, startIndex);
                   
            // Place rectangle
            placeRectangle(startIndex, bestRect.width, bestRect.height);
            
            // Remove used rectangle from BST
            root = deleteNode(root, bestRect);
            
            remainingRects--;
        } else {
            printf("\nNo suitable rectangle found. Adjusting ceiling_height at index %d :\n", startIndex);
            adjustceiling_height(startIndex, maxWidth, containerWidth);
        }
        
        printceiling_height(containerWidth);
    }
    
    // Clean up BST
    cleanupBST(root);
    
    if (remainingRects > 0) {
        printf("Warning: Could not place all rectangles efficiently\n");
    }

    float optimal_height = area_sum / (float)containerWidth;
    int  current_height = calculateHeight(containerWidth);
    float radio = current_height / optimal_height * 100;
    // printf("The sum of all areas is: %d\n", area_sum);

    printf("\n--------------------conclusion---------------------\n");
    printf("- Approximation Ratio for ceiling_height packing: %.4f%%\n", radio);
    
    return current_height;
}
