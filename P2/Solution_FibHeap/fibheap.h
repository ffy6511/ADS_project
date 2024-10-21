// fibheap.h
#ifndef FIBHEAP
#define FIBHEAP

#include "graph.h" // 包含 graph.h 以使用图结构

typedef int Elementtype;

typedef struct FibNode* Fib_Position;
typedef struct Fib* FibHeap;

struct Fib {
    Fib_Position root;      // 指向根链表中的最小节点
    int size;               // 堆中节点的数量
};

struct FibNode {
    Elementtype element;           // 节点的关键字（距离值）
    Fib_Position left_sibling;    // 左兄弟
    Fib_Position right_sibling;   // 右兄弟
    Fib_Position parent;          // 父节点
    Fib_Position children;        // 子节点
    int degree;                   // 度数
    int marked;                   // 标记，用于剪切操作
    int vertex;                   // 对应的顶点编号
};

// 函数声明
Fib_Position Create_Node(Elementtype x, int vertex);
void Print_Node(Fib_Position p);
FibHeap Fibheap_Make_Heap();
FibHeap Fibheap_Insert(FibHeap heap, Elementtype x, int vertex);
FibHeap Fibheap_Delete_Min(FibHeap heap);
FibHeap Fibheap_Merge(FibHeap h1, FibHeap h2);
void Fibheap_Decrease_Key(FibHeap heap, Fib_Position x, Elementtype new_val);
Fib_Position Fibheap_Find(FibHeap heap, int vertex);
int query(graph* graph, int src, int dest);

#endif
