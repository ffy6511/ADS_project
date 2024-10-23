#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>


/************************************************************************************************************
 * 这是左式堆的内部实现
 * 在本次，我们编写的数据结构都是模板类 未实例化时不指定类型名称 
 * 因此无法按照传统的 .h 放名称 .cpp 放主体的方式实现
 * 为了结构完整性加了cpp文件，但它们全是空的
 * 所以在实现上会有注释“用**结构作为堆元素”
 * 
 * 包含操作增删查减小键值等
 * 
 ***********************************************************************************************************/

template <typename T>
class LeftistHeap {
private:
    // 节点结构体，表示堆中的每个节点
    struct Node {
        T key; // 键值对，key 是 std::pair<int, int>，第一个 int 是顶点，第二个 int 是对应的键值
        int npl; // 零路径长度 (NPL)
        Node* left; // 指向左子树的指针
        Node* right; // 指向右子树的指针

        // 构造函数，初始化节点的键值，并设置左右子节点为空，NPL 为 0
        Node(T value) : key(value), npl(0), left(nullptr), right(nullptr) {}
    };

    Node* root; // 左式堆的根节点

    // 合并两个左式堆，返回合并后的堆根节点
    Node* merge(Node* h1, Node* h2) {
        // 如果 h1 为空，则直接返回 h2
        if (!h1) return h2;
        // 如果 h2 为空，则直接返回 h1
        if (!h2) return h1;

        // 确保 h1 的根节点的键值小于 h2 的根节点的键值
        if (h1->key.second > h2->key.second) { // 假设 key 是 std::pair<int, int>，比较第二个值（键）
            std::swap(h1, h2); // 如果 h1 的键值较大，交换 h1 和 h2
        }

        // 递归地合并 h1 的右子树和 h2，结果作为新的右子树
        h1->right = merge(h1->right, h2);

        // 确保左子树的 NPL 不小于右子树，否则交换左右子树
        if (!h1->left || (h1->right && h1->left->npl < h1->right->npl)) {
            std::swap(h1->left, h1->right);
        }

        // 更新 NPL 值（右子树的 NPL 加 1，如果右子树为空，则 NPL 为 0）
        h1->npl = (h1->right ? h1->right->npl + 1 : 0);
        return h1; // 返回合并后的根节点
    }

public:
    // 构造函数，初始化根节点为空
    LeftistHeap() : root(nullptr) {}

    // 析构函数，释放堆中的所有节点
    ~LeftistHeap() {
        clear(root); // 递归清除所有节点
    }

    // 递归清除以 node 为根的子树
    void clear(Node* node) {
        if (node) {
            clear(node->left); // 清除左子树
            clear(node->right); // 清除右子树
            delete node; // 删除当前节点
        }
    }

    // 插入一个新的元素，假设元素是 std::pair<int, int>，其中第一个值是顶点，第二个是键值
    void insert(T value) {
        Node* newNode = new Node(value); // 创建一个新节点
        root = merge(root, newNode); // 合并新节点和当前堆
    }

    // 删除最小元素并返回其键值对
    std::pair<int, int> deleteMin() {
        if (!root) { // 如果堆为空，抛出异常
            throw std::runtime_error("空的！");
        }

        std::pair<int, int> minValue = root->key; // 保存根节点的键值对，即最小值
        Node* oldRoot = root; // 保存当前根节点
        root = merge(root->left, root->right); // 合并根节点的左右子树
        delete oldRoot; // 删除旧的根节点
        return minValue; // 返回最小键值的顶点和键值
    }

    // 获取堆中最小元素的键值对（不删除）
    std::pair<int, int> getMin() const {
        if (!root) { // 如果堆为空，抛出异常
            throw std::runtime_error("空的！");
        }
        return root->key; // 返回根节点的键值对，即最小值
    }

    // 检查堆是否为空
    bool isEmpty() const {
        return root == nullptr; // 如果根节点为空，堆为空
    }

    // 递归中序遍历并显示堆中的元素
    void inorder(Node* node) const {
        if (node) {
            inorder(node->left); // 递归遍历左子树
            std::cout << "(" << node->key.first << ", " << node->key.second << ") "; // 显示顶点和键值
            inorder(node->right); // 递归遍历右子树
        }
    }

    // 显示整个堆的元素（中序遍历）
    void display() const {
        inorder(root); // 从根节点开始中序遍历
        std::cout << std::endl;
    }
};


#endif // LEFTIST_HEAP_H
