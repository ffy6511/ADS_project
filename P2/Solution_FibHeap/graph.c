// graph.c

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

// 创建一个新的图
graph* createGraph(int numVertices) {
    graph* g = (graph*)malloc(sizeof(graph));
    if (g == NULL) {
        printf("Failed to allocate memory for graph\n");
        exit(EXIT_FAILURE);
    }
    g->num_vertex = numVertices;
    g->adj_list = (adj_listnode**)malloc(numVertices * sizeof(adj_listnode*));
    if (g->adj_list == NULL) {
        printf("Failed to allocate memory for adjacency list\n");
        free(g);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numVertices; i++) {
        g->adj_list[i] = NULL;
    }
    return g;
}

// 向图中添加一条边
void addEdge(graph* graph, int src, int dest, int weight) {
    if (src >= graph->num_vertex || dest >= graph->num_vertex || src < 0 || dest < 0) {
        printf("Vertex out of bounds: src=%d, dest=%d\n", src, dest);
        return;
    }
    adj_listnode* new_node = (adj_listnode*)malloc(sizeof(adj_listnode));
    if (new_node == NULL) {
        printf("Failed to allocate memory for the new_node!\n");
        exit(EXIT_FAILURE); // 立即退出，避免继续运行导致更严重的问题
    }
    new_node->vertex = dest;
    new_node->weight = weight;
    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;
}
