#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define Element_type float

///////////////////////////////////////////////////////////
// Part 1: Rectangle Structure and BST Operations
///////////////////////////////////////////////////////////

typedef struct {
    Element_type width;
    Element_type height;
} Rectangle;

// BST node structure
typedef struct TreeNode {
    Rectangle rect;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

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

// Find the largest rectangle that fits within the given width
TreeNode* findLargestFit(TreeNode* root, Element_type targetWidth) {
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