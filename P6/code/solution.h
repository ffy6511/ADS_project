#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

// Define maximum width for Skyline
#define MAX_WIDTH 1000
int skyline[MAX_WIDTH];

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

// Find largest rectangle that fits within target width
TreeNode* findLargestFit(TreeNode* root, int targetWidth) {
    TreeNode* result = NULL;
    
    // Inorder traversal to find the largest width that doesn't exceed target
    if (root != NULL) {
        // First search in left subtree
        result = findLargestFit(root->left, targetWidth);
        
        // If current node fits and is larger than previous result
        if (root->rect.width <= targetWidth && 
            (result == NULL || root->rect.width > result->rect.width)) {
            result = root;
        }
        
        // If current node fits, search right subtree
        if (root->rect.width <= targetWidth) {
            TreeNode* rightResult = findLargestFit(root->right, targetWidth);
            if (rightResult != NULL && 
                rightResult->rect.width > result->rect.width) {
                result = rightResult;
            }
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

// Skyline related functions
void initializeSkyline(int width) {
    for (int i = 0; i < width; i++) {
        skyline[i] = 0;
    }
}

void printSkyline(int containerWidth) {
    printf("Current Skyline: ");
    for (int i = 0; i < containerWidth; i++) {
        printf("%d ", skyline[i]);
    }
    printf("\n");
}

// Find lowest point in Skyline and its maximum width
int findMaxWidth(int containerWidth, int* startIndex) {
    // First find the minimum height
    int minHeight = skyline[0];
    for (int i = 0; i < containerWidth; i++) {
        if (skyline[i] < minHeight) {
            minHeight = skyline[i];
        }
    }
    
    // Then find widest area at minimum height
    int maxSpace = 0;
    int currentStart = 0;
    int currentWidth = 0;
    
    for (int i = 0; i < containerWidth; i++) {
        if (skyline[i] == minHeight) {
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
            if (skyline[i] == minHeight) {
                *startIndex = i;
                return 1;  // Return width of 1
            }
        }
    }
    
    return maxSpace;
}

void placeRectangle(int startIndex, int rectWidth, int rectHeight) {
    for (int i = startIndex; i < startIndex + rectWidth && i < MAX_WIDTH; i++) {
        skyline[i] += rectHeight;
    }
}

int calculateHeight(int containerWidth) {
    int maxHeight = 0;
    for (int i = 0; i < containerWidth; i++) {
        if (skyline[i] > maxHeight) {
            maxHeight = skyline[i];
        }
    }
    return maxHeight;
}

// Improved Skyline adjustment strategy
void adjustSkyline(int startIndex, int width, int containerWidth) {
    // Find minimum and maximum heights in current area
    int minHeight = skyline[startIndex];
    int maxHeight = minHeight;
    for (int i = startIndex; i < startIndex + width && i < containerWidth; i++) {
        if (skyline[i] < minHeight) minHeight = skyline[i];
        if (skyline[i] > maxHeight) maxHeight = skyline[i];
    }
    
    // If height difference exists, level up lower parts
    if (minHeight < maxHeight) {
        for (int i = startIndex; i < startIndex + width && i < containerWidth; i++) {
            if (skyline[i] < maxHeight) {
                skyline[i] = maxHeight;
            }
        }
    } else {
        // If height is uniform, increment entire section
        int newHeight = minHeight + 1;
        for (int i = startIndex; i < startIndex + width && i < containerWidth; i++) {
            skyline[i] = newHeight;
        }
    }
}

// Main algorithm: Skyline Packing using BST
int skylinePacking(Rectangle *rectangles, int n, int containerWidth) {
    initializeSkyline(containerWidth);
    
    // Build BST
    TreeNode* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, rectangles[i]);
    }
    
    int remainingRects = n;
    int stuckCounter = 0;
    const int MAX_STUCK_ATTEMPTS = 5;  // Maximum attempts when stuck
    
    while (remainingRects > 0 && stuckCounter < MAX_STUCK_ATTEMPTS) {
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
            stuckCounter = 0;
        } else {
            printf("No suitable rectangle found. Adjusting skyline at index %d\n", startIndex);
            adjustSkyline(startIndex, maxWidth, containerWidth);
            stuckCounter++;
        }
        
        printSkyline(containerWidth);
    }
    
    // Clean up BST
    cleanupBST(root);
    
    if (remainingRects > 0) {
        printf("Warning: Could not place all rectangles efficiently\n");
    }
    
    return calculateHeight(containerWidth);
}