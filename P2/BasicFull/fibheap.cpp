//这是一个模板类，不能按照.h .cpp 方式写
//必须把所有的内容全放在.h文件里


/*

#include "fibheap.h"

template <typename T>
FibonacciHeap<T>::Node::Node(T val) : key(val), degree(0), parent(nullptr), child(nullptr), left(this), right(this), mark(false) {}

template <typename T>
FibonacciHeap<T>::FibonacciHeap() : minNode(nullptr), heapSize(0) {}

template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    // pass TODO:析构函数
}

template <typename T>
bool FibonacciHeap<T>::isEmpty() const {
    return minNode == nullptr;
}

template <typename T>
void FibonacciHeap<T>::insert(T value) {
    Node* node = new Node(value);
    if (!minNode) {
        minNode = node;
    }
    else {
        node->left = minNode;
        node->right = minNode->right;
        minNode->right->left = node;
        minNode->right = node;
        if (value < minNode->key) {
            minNode = node;
        }
    }
    heapSize++;
}

template <typename T>
T FibonacciHeap<T>::getMin() const {
    if (!minNode) throw std::runtime_error("Heap is empty.");
    return minNode->key;
}


template <typename T>
void FibonacciHeap<T>::link(Node* y, Node* x) {

    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->left = y->right = y;
    }
    else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = false;
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    int D = static_cast<int>(std::log2(heapSize)) + 1;
    std::vector<Node*> A(D, nullptr);

    std::vector<Node*> rootList;
    Node* current = minNode;
    if (current) {
        do {
            rootList.push_back(current);
            current = current->right;
        } while (current != minNode);
    }

    for (Node* w : rootList) {
        Node* x = w;
        int d = x->degree;
        while (A[d]) {
            Node* y = A[d];
            if (x->key > y->key) std::swap(x, y);
            link(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }

    minNode = nullptr;
    for (Node* w : A) {
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
void FibonacciHeap<T>::deleteMin() {
    if (!minNode) return;

    Node* oldMin = minNode;
    if (oldMin->child) {
        Node* child = oldMin->child;
        do {
            child->parent = nullptr;
            child = child->right;
        } while (child != oldMin->child);

        Node* minLeft = minNode->left;
        Node* childLeft = oldMin->child->left;

        minLeft->right = oldMin->child;
        oldMin->child->left = minLeft;

        childLeft->right = minNode->right;
        minNode->right->left = childLeft;
    }

    minNode->left->right = minNode->right;
    minNode->right->left = minNode->left;

    if (minNode == minNode->right) {
        minNode = nullptr;
    }
    else {
        minNode = minNode->right;
        consolidate();
    }

    delete oldMin;
    heapSize--;
}

template <typename T>
void FibonacciHeap<T>::decreaseKey(typename FibonacciHeap<T>::Node* node, T newValue) {
    if (newValue > node->key) {
        throw std::invalid_argument("大了");
    }
    node->key = newValue;
    Node* parent = node->parent;

    if (parent && node->key < parent->key) {
        cut(node, parent);
        cascadingCut(parent);
    }

    if (node->key < minNode->key) {
        minNode = node;
    }
}

template <typename T>
void FibonacciHeap<T>::cut(Node* x, Node* y) {
    if (x->right == x) {
        y->child = nullptr;
    }
    else {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x) {
            y->child = x->right;
        }
    }
    y->degree--;

    x->left = minNode;
    x->right = minNode->right;
    minNode->right->left = x;
    minNode->right = x;

    x->parent = nullptr;
    x->mark = false;
}

template <typename T>
void FibonacciHeap<T>::cascadingCut(Node* y) {
    Node* z = y->parent;
    if (z) {
        if (!y->mark) {
            y->mark = true;
        }
        else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

template <typename T>
void FibonacciHeap<T>::deleteNode(Node* node) {
    decreaseKey(node, std::numeric_limits<T>::min());
    deleteMin();
}

template <typename T>
int FibonacciHeap<T>::size() const {
    return heapSize;
}
*/