// fibheap.c
#include "fibheap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define maxDegree  45

// 创建一个新的斐波那契堆节点
Fib_Position Create_Node(Elementtype x, int vertex) {
    Fib_Position node = (Fib_Position)malloc(sizeof(struct FibNode));
    if (node == NULL) {
        printf("内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    node->element = x;
    node->degree = 0;
    node->marked = 0;
    node->parent = node->children = NULL;
    node->left_sibling = node->right_sibling = node;
    node->vertex = vertex;
    return node;
}

// 打印节点信息
void Print_Node(Fib_Position p) {
    if (p != NULL) {
        printf("顶点: %d, 距离: %d\n", p->vertex, p->element);
    }
}

// 创建一个新的斐波那契堆
FibHeap Fibheap_Make_Heap() {
    FibHeap heap = (FibHeap)malloc(sizeof(struct Fib));
    if (heap == NULL) {
        printf("内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    heap->root = NULL;
    heap->size = 0;
    return heap;
}

// 将节点插入到根链表中
void Insert_To_Root_List(FibHeap heap, Fib_Position node) {
    if (heap->root == NULL) {
        heap->root = node;
    }
    else {
        node->left_sibling = heap->root->left_sibling;
        node->right_sibling = heap->root;
        heap->root->left_sibling->right_sibling = node;
        heap->root->left_sibling = node;
        if (node->element < heap->root->element) {
            heap->root = node;
        }
    }
}

// 插入操作
FibHeap Fibheap_Insert(FibHeap heap, Elementtype x, int vertex) {
    Fib_Position node = Create_Node(x, vertex);
    Insert_To_Root_List(heap, node);
    heap->size++;
    return heap;
}

// 合并两个斐波那契堆
FibHeap Fibheap_Merge(FibHeap h1, FibHeap h2) {
    if (h1->root == NULL) return h2;
    if (h2->root == NULL) return h1;

    // 合并根链表
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
    // 从根链表中移除 child
    child->left_sibling->right_sibling = child->right_sibling;
    child->right_sibling->left_sibling = child->left_sibling;

    // 将 child 加入到 parent 的子链表中
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

// 合并具有相同度数的树
void Consolidate(FibHeap heap) {
    Fib_Position array[maxDegree];
    for (int i = 0; i < maxDegree; i++)
        array[i] = NULL;

    // 遍历根链表
    Fib_Position start = heap->root;
    if (start == NULL)
        return;

    Fib_Position x = start;
    do {
        Fib_Position next = x->right_sibling;
        int degree = x->degree;
        while (array[degree] != NULL) {
            Fib_Position y = array[degree];
            if (x->element > y->element) {
                // 交换 x 和 y
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

    // 重建根链表并找到新的最小节点
    heap->root = NULL;
    for (int i = 0; i < maxDegree; i++) {
        if (array[i] != NULL) {
            if (heap->root == NULL) {
                heap->root = array[i];
                array[i]->left_sibling = array[i]->right_sibling = array[i];
            }
            else {
                Insert_To_Root_List(heap, array[i]);
            }
            if (array[i]->element < heap->root->element) {
                heap->root = array[i];
            }
        }
    }
}

// 删除最小节点
FibHeap Fibheap_Delete_Min(FibHeap heap) {
    Fib_Position min = heap->root;
    if (min == NULL)
        return heap;

    // 将最小节点的所有子节点加入根链表
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

    // 从根链表中移除最小节点
    if (min->left_sibling == min) {
        heap->root = NULL;
    }
    else {
        heap->root = min->right_sibling;
        min->left_sibling->right_sibling = min->right_sibling;
        min->right_sibling->left_sibling = min->left_sibling;
        Consolidate(heap);
    }

    heap->size--;
    free(min);
    return heap;
}

// 减少节点的关键字值
void Fibheap_Decrease_Key(FibHeap heap, Fib_Position x, Elementtype new_val) {
    if (new_val > x->element) {
        printf("错误：新键值大于当前键值\n");
        return;
    }

    x->element = new_val;
    Fib_Position y = x->parent;

    if (y != NULL && x->element < y->element) {
        // 将 x 从 y 的子链表中切断
        if (x->right_sibling == x) {
            y->children = NULL;
        }
        else {
            x->left_sibling->right_sibling = x->right_sibling;
            x->right_sibling->left_sibling = x->left_sibling;
            if (y->children == x)
                y->children = x->right_sibling;
        }
        y->degree--;

        // 将 x 加入根链表
        Insert_To_Root_List(heap, x);
        x->parent = NULL;
        x->marked = 0;

        // 级联剪切
        Fib_Position z = y;
        while (z->parent != NULL) {
            if (z->marked == 0) {
                z->marked = 1;
                break;
            }
            else {
                Fib_Position parent_z = z->parent;
                // 将 z 从 parent_z 的子链表中切断
                if (z->right_sibling == z) {
                    parent_z->children = NULL;
                }
                else {
                    z->left_sibling->right_sibling = z->right_sibling;
                    z->right_sibling->left_sibling = z->left_sibling;
                    if (parent_z->children == z)
                        parent_z->children = z->right_sibling;
                }
                parent_z->degree--;

                // 将 z 加入根链表
                Insert_To_Root_List(heap, z);
                z->parent = NULL;
                z->marked = 0;

                z = parent_z;
            }
        }

        // 更新根链表中的最小节点
        if (x->element < heap->root->element) {
            heap->root = x;
        }
    }

    // 如果 x 是新的最小节点，则更新堆的根
    if (x->element < heap->root->element) {
        heap->root = x;
    }
}

// 根据顶点编号查找节点（递归）
Fib_Position Fibheap_Find_Node(Fib_Position node, int vertex) {
    if (node == NULL)
        return NULL;

    Fib_Position found = NULL;
    Fib_Position start = node;
    do {
        if (node->vertex == vertex)
            return node;

        // 递归查找子节点
        if (node->children != NULL) {
            found = Fibheap_Find_Node(node->children, vertex);
            if (found != NULL)
                return found;
        }

        node = node->right_sibling;
    } while (node != start);

    return NULL;
}

// 在堆中查找指定顶点的节点
Fib_Position Fibheap_Find(FibHeap heap, int vertex) {
    if (heap == NULL || heap->root == NULL)
        return NULL;

    return Fibheap_Find_Node(heap->root, vertex);
}

// 实现 Dijkstra 算法
int query(graph* graph, int src, int dest) {
    int num_vertices = graph->num_vertex;

    // 分配并初始化 distance 和 known 数组
    int* dist = (int*)malloc(num_vertices * sizeof(int));
    int* known = (int*)malloc(num_vertices * sizeof(int));

    if (dist == NULL || known == NULL) {
        printf("内存分配失败！\n");
        if (dist) free(dist);
        if (known) free(known);
        return INT_MAX;
    }

    for (int i = 0; i < num_vertices; i++) {
        dist[i] = INT_MAX;
        known[i] = 0;
    }

    // 创建斐波那契堆
    FibHeap heap = Fibheap_Make_Heap();
    if (heap == NULL) {
        printf("堆创建失败！\n");
        free(dist);
        free(known);
        return INT_MAX;
    }

    // 初始化源节点
    dist[src] = 0;
    Fibheap_Insert(heap, dist[src], src);

    // 创建一个数组来跟踪每个顶点在堆中的位置
    Fib_Position* position = (Fib_Position*)malloc(num_vertices * sizeof(Fib_Position));
    if (position == NULL) {
        printf("内存分配失败！\n");
        free(dist);
        free(known);
        free(heap);
        return INT_MAX;
    }
    for (int i = 0; i < num_vertices; i++) {
        position[i] = NULL;
    }
    position[src] = heap->root;

    while (heap->size > 0) {
        // 提取最小节点
        Fib_Position min_node = heap->root;
        int u = min_node->vertex;
        Fibheap_Delete_Min(heap);
        known[u] = 1;

        // 遍历 u 的所有邻接点
        adj_listnode* neighbor = graph->adj_list[u];
        while (neighbor != NULL) {
            int v = neighbor->vertex;
            int weight = neighbor->weight;

            if (!known[v] && dist[u] != INT_MAX && (dist[u] + weight) < dist[v]) {
                dist[v] = dist[u] + weight;
                if (position[v] == NULL) {
                    Fibheap_Insert(heap, dist[v], v);
                    position[v] = heap->root; // 更新位置
                }
                else {
                    Fibheap_Decrease_Key(heap, position[v], dist[v]);
                }
            }

            neighbor = neighbor->next;
        }

        // 释放 min_node 节点
    }

    int distance = dist[dest];

    // 释放所有分配的内存
    free(dist);
    free(known);
    free(position);
    free(heap);

    printf("从顶点 %d 到顶点 %d 的最短距离是 %d\n", src, dest, distance);
    return distance;
}
