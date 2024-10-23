#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <stdexcept>


/************************************************************************************************************
 * 这是斐波那契堆的内部实现
 * 在本次，我们编写的数据结构都是模板类 未实例化时不指定类型名称 
 * 因此无法按照传统的 .h 放名称 .cpp 放主体的方式实现
 * 为了结构完整性加了cpp文件，但它们全是空的
 * 所以在实现上会有注释“用**结构作为堆元素”
 * 
 * 我们在编写 .h 时，把传统的 .cpp 包含的内容分隔开，便于阅读
 * 
 * 包含操作增删查减小键值等
 * 
 ***********************************************************************************************************/



template <typename T> //以T作为模板
class FibonacciHeap {
public:
    FibonacciHeap();
    ~FibonacciHeap();

    struct Node {
        T key;        // 存储的值（键）
        int val;      // 顶点的值
        int degree;   // 度数，表示有几个儿子
        Node* parent; // 父亲
        Node* child;  // 任意一个孩子
        Node* left;   // 左兄弟
        Node* right;  // 右兄弟
        bool mark;    // 是否标记
        // 构造函数 接受模板类型的k和int类型的v，v表示节点编号（在迪杰特拉斯算法中的编号
        Node(T k, int v) : key(k), val(v), degree(0), parent(nullptr), child(nullptr), left(this), right(this), mark(false) {}
    };

    Node* minNode;
    int heapSize;

    bool isEmpty() const; //是否空
    void insert(T value, int vertex); //插入
    typename FibonacciHeap<T>::Node* getMin() const; // 修改为返回顶点的值
    void deleteMin(); //删最小
    void decreaseKey(Node* node, T newValue); //减小键值
    void deleteNode(Node* node); //删（在算法实现中没有用到）
    int size() const; // 大小

private:
    void link(Node* y, Node* x);  //链接，一个成为另一个孩子
    void consolidate(); //合并
    void cut(Node* x, Node* y); //剪切 合并的反操作
    void cascadingCut(Node* y);  //剪切 链接的反操作
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//下面是实现



//构造函数
template <typename T>
FibonacciHeap<T>::FibonacciHeap() : minNode(nullptr), heapSize(0) {}

//析构函数
template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    if (minNode) {
        // 释放所有节点
        std::vector<Node*> nodes;
        Node* current = minNode;
        do {
            nodes.push_back(current);
            current = current->right;
        } while (current != minNode);

        for (Node* node : nodes) {
            delete node;
        }
    }
}

template <typename T>
bool FibonacciHeap<T>::isEmpty() const {
    // 检查堆是否为空，如果最小节点为 nullptr，则堆为空
    return minNode == nullptr;
}

template <typename T>
void FibonacciHeap<T>::insert(T value, int vertex) {
    // 插入一个新的节点到斐波那契堆中，节点包含一个值和对应的顶点编号
    Node* node = new Node(value, vertex); // 创建新节点
    if (!minNode) { // 如果最小节点为空，设置当前节点为最小节点
        minNode = node;
    }
    else { // 如果堆非空，将新节点插入到根列表中
        node->left = minNode; // 新节点的左指针指向最小节点
        node->right = minNode->right; // 新节点的右指针指向最小节点的右节点
        minNode->right->left = node; // 更新最小节点右侧节点的左指针
        minNode->right = node; // 更新最小节点的右指针
        if (value < minNode->key) { // 如果新插入的值比当前最小值还小，更新最小节点
            minNode = node;
        }
    }
    heapSize++; // 堆大小增加
}

template <typename T>
typename FibonacciHeap<T>::Node* FibonacciHeap<T>::getMin() const {
    // 返回最小节点，如果堆为空则抛出异常
    if (!minNode) throw std::runtime_error("空堆！");
    return minNode; // 返回最小节点的指针
}

template <typename T>
void FibonacciHeap<T>::deleteMin() {
    // 删除堆中最小的节点
    if (!minNode) return; // 如果堆为空，直接返回

    Node* oldMin = minNode; // 保存最小节点
    if (oldMin->child) { // 如果最小节点有孩子
        Node* child = oldMin->child;
        do { // 遍历孩子链表，将孩子的 parent 指针置空
            child->parent = nullptr;
            child = child->right;
        } while (child != oldMin->child);

        // 合并孩子节点到根列表
        Node* minLeft = minNode->left;
        Node* childLeft = oldMin->child->left;

        minLeft->right = oldMin->child;
        oldMin->child->left = minLeft;

        childLeft->right = minNode->right;
        minNode->right->left = childLeft;
    }

    // 从根列表中移除最小节点
    minNode->left->right = minNode->right;
    minNode->right->left = minNode->left;

    if (minNode == minNode->right) {
        // 如果堆中只有一个节点，删除后堆为空
        minNode = nullptr;
    }
    else {
        // 否则，将最小节点设置为根列表中的下一个节点，并调用 consolidate 函数
        minNode = minNode->right;
        consolidate();
    }

    delete oldMin; // 删除旧的最小节点
    heapSize--; // 堆大小减少
}

template <typename T>
void FibonacciHeap<T>::decreaseKey(Node* node, T newValue) {
    // 减少节点的键值，如果新的值大于当前值，抛出异常
    if (newValue > node->key) {
        throw std::invalid_argument("太大！");
    }
    node->key = newValue; // 更新节点的值
    Node* parent = node->parent;

    // 如果节点有父节点，且当前值小于父节点的值，切断节点与父节点的连接
    if (parent && node->key < parent->key) {
        cut(node, parent);
        cascadingCut(parent); // 执行级联剪切
    }

    // 如果当前值比最小值还小，更新最小节点
    if (minNode && node->key < minNode->key) {
        minNode = node;
    }
}

template <typename T>
void FibonacciHeap<T>::link(Node* y, Node* x) {
    // 将节点 y 链接到节点 x 上，y 将作为 x 的孩子
    y->left->right = y->right; // 从根列表中移除 y
    y->right->left = y->left;

    y->parent = x; // 设置 y 的父节点为 x
    if (!x->child) { // 如果 x 没有孩子，y 成为 x 的唯一孩子
        x->child = y;
        y->left = y->right = y;
    }
    else { // 如果 x 有孩子，将 y 插入到孩子链表中
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++; // 更新 x 的度数
    y->mark = false; // 标记 y 为未被剪切
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    // 将根列表中的节点合并，以确保每个度数的节点只有一个
    int D = static_cast<int>(std::log2(heapSize)) + 1; // 计算所需的数组大小
    std::vector<Node*> A(D, nullptr); // 创建一个数组，用于存储各个度数的节点

    std::vector<Node*> rootList; // 创建根列表数组
    Node* current = minNode;
    if (current) { // 如果根列表不为空，遍历根列表中的所有节点
        do {
            rootList.push_back(current);
            current = current->right;
        } while (current != minNode);
    }

    // 遍历根列表，合并具有相同度数的节点
    for (Node* w : rootList) {
        Node* x = w;
        int d = x->degree;
        while (A[d]) { // 如果当前度数存在节点，合并
            Node* y = A[d];
            if (x->key > y->key) std::swap(x, y); // 保证 x 的键值更小
            link(y, x); // 将 y 链接到 x 上
            A[d] = nullptr; // 清空该位置
            d++; // 增加度数
        }
        A[d] = x; // 将节点放入数组中
    }

    minNode = nullptr; // 清空最小节点
    for (Node* w : A) { // 重新构建根列表
        if (w) {
            if (!minNode) {
                minNode = w;
                w->left = w->right = w;
            }
            else {
                w->left = minNode;
                w->right = minNode->right;
                minNode->right->left = w;
                minNode->right = w;
                if (w->key < minNode->key) {
                    minNode = w;
                }
            }
        }
    }
}

template <typename T>
void FibonacciHeap<T>::cut(Node* x, Node* y) {
    // 从 y 的孩子中移除 x，并将 x 添加到根列表
    if (x->right == x) { // 如果 x 是 y 的唯一孩子
        y->child = nullptr;
    }
    else { // 否则将 x 从孩子列表中移除
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x) {
            y->child = x->right;
        }
    }
    y->degree--; // 减少 y 的度数

    // 将 x 插入到根列表
    x->left = minNode;
    x->right = minNode->right;
    minNode->right->left = x;
    minNode->right = x;

    x->parent = nullptr; // 设置 x 的父节点为空
    x->mark = false; // 标记 x 为未被剪切
}

template <typename T>
void FibonacciHeap<T>::cascadingCut(Node* y) {
    // 级联剪切，如果父节点 y 已被标记，则递归剪切
    Node* z = y->parent;
    if (z) {
        if (!y->mark) { // 如果 y 没有被标记，则标记它
            y->mark = true;
        }
        else { // 如果 y 已被标记，剪切 y，并继续递归剪切其父节点 z
            cut(y, z);
            cascadingCut(z);
        }
    }
}

template <typename T>
void FibonacciHeap<T>::deleteNode(Node* node) {
    // 删除给定的节点，首先将其键值减少到负无穷，然后删除最小节点
    decreaseKey(node, std::numeric_limits<T>::min()); // 将键值减少到负无穷
    deleteMin(); // 删除最小节点
}

template <typename T>
int FibonacciHeap<T>::size() const {
    // 返回堆的大小
    return heapSize;
}

#endif // FIBONACCI_HEAP_H
