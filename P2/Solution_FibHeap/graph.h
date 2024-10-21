// graph.h

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTEX 2000000

// 邻接节点结构体
typedef struct adj_listnode {
    int vertex;               // 邻接顶点编号
    int weight;               // 边的权重
    struct adj_listnode* next; // 指向下一个邻接节点的指针
} adj_listnode;

// 图结构体
typedef struct graph {
    int num_vertex;           // 图中的顶点数量
    adj_listnode** adj_list;  // 邻接列表数组
} graph;

// 函数声明
graph* createGraph(int numVertices);
void addEdge(graph* graph, int src, int dest, int weight);

#endif // GRAPH_H
