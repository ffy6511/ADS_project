#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H

#include <iostream>
#include <limits>
#include <stdexcept>

// 使用模板类实现通用的左倾堆，可以存储任意类型的数据
template <typename T>
class LeftistHeap {
private:
    // 定义堆节点的内部结构体
    struct Node {
        T key;         // 节点存储的键值
        int npl;       // 左式堆的零路径长 (null path length)
        Node* left;    // 指向左子树的指针
        Node* right;   // 指向右子树的指针

        // 使用初始化列表构造节点
        explicit Node(T value) : key(value), npl(0), left(nullptr), right(nullptr) {} //避免隐式转换
    };

    Node* root;  // 堆的根节点

    // 合并两个堆的私有递归方法
    Node* merge(Node* firstHeap, Node* secondHeap) {
        if (!firstHeap) return secondHeap;
        if (!secondHeap) return firstHeap;

        // 确保 firstHeap 的根节点的键小于 secondHeap 的根节点的键
        if (firstHeap->key > secondHeap->key) {
            std::swap(firstHeap, secondHeap);
        }

        // 递归合并 firstHeap 的右子树和 secondHeap
        firstHeap->right = merge(firstHeap->right, secondHeap);

        // 保持左式堆的性质，确保左子树比右子树更高
        if (!firstHeap->left || (firstHeap->right && firstHeap->left->npl < firstHeap->right->npl)) {
            std::swap(firstHeap->left, firstHeap->right);
        }

        // 更新零路径长 (NPL)
        firstHeap->npl = (firstHeap->right ? firstHeap->right->npl + 1 : 0);
        return firstHeap;
    }

public:
    // 构造函数，初始化根节点为空
    LeftistHeap() : root(nullptr) {}

    // 析构函数，销毁堆并释放内存
    ~LeftistHeap() {
        clear(root);
    }

    // 递归清空堆的方法
    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);  // 清空左子树
            clear(node->right); // 清空右子树
            delete node;        // 删除当前节点
        }
    }

    // 插入一个新元素到堆中
    void insert(T value) {
        Node* newNode = new (std::nothrow) Node(value);  // 动态分配一个新节点，使用 nothrow 防止内存分配失败
        if (newNode == nullptr) {
            throw std::bad_alloc();  // 如果分配失败，抛出 bad_alloc 异常
        }

        root = merge(root, newNode);      // 合并新节点与当前堆
    }

    // 删除堆中的最小元素并返回其值
    T deleteMin() {
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");  // 如果堆为空，抛出异常
        }

        T minValue = root->key;  // 记录最小值
        Node* oldRoot = root;    // 保存当前根节点
        root = merge(root->left, root->right);  // 合并左子树和右子树形成新的根节点
        delete oldRoot;          // 删除旧的根节点
        return minValue;         // 返回最小值
    }

    // 获取堆中的最小元素，不删除它
    T getMin() const {
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");  // 如果堆为空，抛出异常
        }
        return root->key;  // 返回根节点的键值
    }

    // 检查堆是否为空
    bool isEmpty() const {
        return root == nullptr;  // 如果根节点为空，堆就是空的
    }

    // 递归中序遍历，用于打印堆中的所有元素
    void inorder(Node* node) const {
        if (node != nullptr) {
            inorder(node->left);         // 递归遍历左子树
            std::cout << node->key << " ";       // 打印当前节点的键值
            inorder(node->right);        // 递归遍历右子树
        }
    }

    // 迭代中序遍历，用于打印堆中的所有元素
    // void inorder() const {
    //     if (root == nullptr) {
    //         return;
    //     }

    //     std::stack<Node*> nodeStack;
    //     Node* currentNode = root;

    //     while (!nodeStack.empty() || currentNode != nullptr) {
    //         // 访问左子树节点
    //         while (currentNode != nullptr) {
    //             nodeStack.push(currentNode);
    //             currentNode = currentNode->left;
    //         }

    //         // 访问节点并处理右子树
    //         currentNode = nodeStack.top();
    //         nodeStack.pop();
    //         std::cout << currentNode->key << " ";

    //         currentNode = currentNode->right;
    //     }
    //     std::cout << std::endl;
    // }


    // 显示堆中的所有元素
    void display() const {
        inorder(root);  // 从根节点开始中序遍历
        std::cout << std::endl;  // 输出换行符
    }
};

#endif // LEFTIST_HEAP_H

