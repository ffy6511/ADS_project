#include "fibheap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Fib_Position Create_Node(Elementtype x) {
    Fib_Position node = (Fib_Position)malloc(sizeof(struct FibNode));
    node->element = x;
    node->degree = 0;
    node->marked = 0;
    node->parent = node->children = NULL;
    node->left_sibling = node->right_sibling = node;
    return node;
}

// 创建斐波那契堆
FibHeap Fibheap_Make_Heap() {
    FibHeap heap = (FibHeap)malloc(sizeof(struct Fib));
    heap->root = NULL;
    heap->size = 0;
    heap->rootdegree = 0;
    return heap;
}

// 将节点插入到双向循环链表中
void Insert_To_Root_List(FibHeap heap, Fib_Position node) {
    if (heap->root == NULL) {
        heap->root = node;
    }
    else {
        node->left_sibling = heap->root->left_sibling;
        node->right_sibling = heap->root;
        heap->root->left_sibling->right_sibling = node;
        heap->root->left_sibling = node;
    }
}

// 插入操作
FibHeap Fibheap_Insert(FibHeap heap, Elementtype x) {
    Fib_Position node = Create_Node(x);
    Insert_To_Root_List(heap, node);
    if (heap->root == NULL || x < heap->root->element) {
        heap->root = node;
    }
    heap->size++;
    return heap;
}

// 合并两个堆
FibHeap Fibheap_Merge(FibHeap h1, FibHeap h2) {
    if (!h1->root) return h2;
    if (!h2->root) return h1;

    // 合并根列表
    Fib_Position temp = h1->root->right_sibling;
    h1->root->right_sibling = h2->root->right_sibling;
    h2->root->right_sibling->left_sibling = h1->root;
    h2->root->right_sibling = temp;
    temp->left_sibling = h2->root;

    if (h2->root->element < h1->root->element)
        h1->root = h2->root;

    h1->size += h2->size;
    free(h2); // h2 不再需要
    return h1;
}

// 将某个节点加为另一个节点的子节点
void Link(Fib_Position parent, Fib_Position child) {
    child->left_sibling->right_sibling = child->right_sibling;
    child->right_sibling->left_sibling = child->left_sibling;

    if (parent->children == NULL) {
        parent->children = child;
        child->left_sibling = child->right_sibling = child;
    }
    else {
        child->left_sibling = parent->children;
        child->right_sibling = parent->children->right_sibling;
        parent->children->right_sibling->left_sibling = child;
        parent->children->right_sibling = child;
    }
    child->parent = parent;
    parent->degree++;
    child->marked = 0;
}

// 合并根节点相同度数的树
void Consolidate(FibHeap heap) {
    Fib_Position array[maxDegree] = { NULL };
    Fib_Position x = heap->root;
    Fib_Position start = x;

    do {
        Fib_Position next = x->right_sibling;
        int degree = x->degree;
        while (array[degree] != NULL) {
            Fib_Position y = array[degree];
            if (x->element > y->element) {
                Fib_Position temp = x;
                x = y;
                y = temp;
            }
            Link(x, y);
            array[degree] = NULL;
            degree++;
        }
        array[degree] = x;
        x = next;
    } while (x != start);

    heap->root = NULL;
    for (int i = 0; i < maxDegree; i++) {
        if (array[i] != NULL) {
            if (heap->root == NULL) {
                heap->root = array[i];
            }
            else {
                Insert_To_Root_List(heap, array[i]);
                if (array[i]->element < heap->root->element) {
                    heap->root = array[i];
                }
            }
        }
    }
}

// 删除最小节点
FibHeap Fibheap_Delete_Min(FibHeap heap) {
    Fib_Position min = heap->root;
    if (min != NULL) {
        Fib_Position child = min->children;
        if (child != NULL) {
            Fib_Position start = child;
            do {
                Fib_Position next = child->right_sibling;
                Insert_To_Root_List(heap, child);
                child->parent = NULL;
                child = next;
            } while (child != start);
        }

        min->left_sibling->right_sibling = min->right_sibling;
        min->right_sibling->left_sibling = min->left_sibling;
        if (min == min->right_sibling) {
            heap->root = NULL;
        }
        else {
            heap->root = min->right_sibling;
            Consolidate(heap);
        }

        heap->size--;
        free(min);
    }
    return heap;
}

// 减少节点的值
void Fibheap_Decrease_Key(FibHeap heap, Fib_Position p, Elementtype x) {
    if (x > p->element) {
        printf("Error: new key is greater than current key\n");
        return;
    }

    p->element = x;
    Fib_Position parent = p->parent;

    if (parent != NULL && p->element < parent->element) {
        // 将 p 从父节点中分离
        while (parent != NULL && p->element < parent->element) {
            // 级联切断
            Insert_To_Root_List(heap, p);
            parent->degree--;
            p->marked = 0;
            p = parent;
            parent = p->parent;
        }
    }

    if (p->element < heap->root->element) {
        heap->root = p;
    }
}

void Print_Node(Fib_Position p) {
    printf("%d", p->element);
}

Fib_Position Fibheap_Find_Node(Fib_Position node, Elementtype x) {
    if (node == NULL) {
        return NULL;
    }

    Fib_Position found = NULL;
    Fib_Position start = node;  // 记录链表的起始位置
    do {
        if (node->element == x) {
            return node;  // 找到目标节点
        }

        // 递归查找子节点
        if (node->children != NULL) {
            found = Fibheap_Find_Node(node->children, x);
            if (found != NULL) {
                return found;  // 如果在子树中找到，直接返回
            }
        }

        node = node->right_sibling;  // 移动到下一个兄弟节点
    } while (node != start);  // 遍历整个循环链表

    return NULL;  // 如果没找到，返回NULL
}

// 在堆中查找指定元素
Fib_Position Fibheap_Find(FibHeap heap, Elementtype x) {
    if (heap == NULL || heap->root == NULL) {
        return NULL;  // 如果堆为空，直接返回 NULL
    }

    // 从根节点开始查找
    return Fibheap_Find_Node(heap->root, x);
}