#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility> // for std::pair

template <typename T>
class LeftistHeap {
private:
    struct Node {
        T key; // 这里可以是一个 std::pair<int, int>，第一个 int 是顶点，第二个 int 是键值
        int npl;
        Node* left;
        Node* right;

        Node(T value) : key(value), npl(0), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // 合并两个左式堆
    Node* merge(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        // 确保 h1 的根节点的键小于 h2 的根节点的键
        if (h1->key.second > h2->key.second) { // 假设 key 是 std::pair<int, int>
            std::swap(h1, h2);
        }

        // 合并 h1 的右子树和 h2
        h1->right = merge(h1->right, h2);

        // 确保左子树比右子树更高
        if (!h1->left || (h1->right && h1->left->npl < h1->right->npl)) {
            std::swap(h1->left, h1->right);
        }

        // 更新 NPL
        h1->npl = (h1->right ? h1->right->npl + 1 : 0);
        return h1;
    }

public:
    LeftistHeap() : root(nullptr) {}

    ~LeftistHeap() {
        clear(root);
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // 插入一个新的元素，假设元素是 std::pair<int, int>
    void insert(T value) {
        Node* newNode = new Node(value);
        root = merge(root, newNode);
    }

    // 删除最小元素并返回顶点
    std::pair<int, int> deleteMin() {
        if (!root) {
            throw std::runtime_error("Heap is empty");
        }

        std::pair<int, int> minValue = root->key; // 假设 key 是 std::pair<int, int>
        Node* oldRoot = root;
        root = merge(root->left, root->right);
        delete oldRoot;
        return minValue; // 返回具有最小键值的顶点
    }

    // 获取最小元素的顶点
    std::pair<int, int> getMin() const {
        if (!root) {
            throw std::runtime_error("Heap is empty");
        }
        return root->key; // 返回具有最小键值的顶点
    }

    // 检查堆是否为空
    bool isEmpty() const {
        return root == nullptr;
    }

    // 显示堆中的元素（中序遍历）
    void inorder(Node* node) const {
        if (node) {
            inorder(node->left);
            std::cout << "(" << node->key.first << ", " << node->key.second << ") "; // 显示顶点和键值
            inorder(node->right);
        }
    }

    void display() const {
        inorder(root);
        std::cout << std::endl;
    }
};

#endif // LEFTIST_HEAP_H
