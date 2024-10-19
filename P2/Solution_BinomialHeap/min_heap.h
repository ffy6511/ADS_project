#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define MAX_VERTEX 1000000

typedef struct heap_node{
    int dist;
    int vertex;
} heap_node;

typedef struct min_heap{
    heap_node* nodes;    // 堆节点数组，使用 1-based 索引
    int size;            // 当前堆的大小
    int capacity;        // 堆的容量
    int* position;       // 辅助数组，position[vertex] = 在堆中的位置
} min_heap;

typedef struct adj_listnode{
    int vertex; // 邻接点
    int weight;
    struct adj_listnode* next;
} adj_listnode;

typedef struct graph{
    int num_vertex;
    adj_listnode** adj_list;
} graph;

// 函数声明
graph* createGraph(int numVertices); // 新建图
void addEdge(graph* graph, int src, int dest, int weight); // 添加边
min_heap* createMinHeap(int capacity);      // 新建最小堆
void insertMinHeap(min_heap* heap, int vertex, int distance);   // 插入最小堆
heap_node* extractMin(min_heap* heap);       // 删除最小堆最小元素
void decreaseKey(min_heap* heap, int vertex, int newDistance);     // 修改最小堆元素值
int query(graph* graph, int src, int dest);   // dijkstra算法

// 创建图并初始化
graph* createGraph(int numVertices){
    graph* g = (graph*)malloc(sizeof(graph));
    g->num_vertex = numVertices;
    g->adj_list = (adj_listnode**)malloc(numVertices * sizeof(adj_listnode*));
    for(int i = 0; i < numVertices; i++) {
        g->adj_list[i] = NULL;
    }
    return g;
}

// 添加边到图中
void addEdge(graph* graph, int src, int dest, int weight){
    adj_listnode* new_node = (adj_listnode*)malloc(sizeof(adj_listnode));
    new_node->vertex = dest;
    new_node->weight = weight;
    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;
}

// 创建最小堆并初始化
min_heap* createMinHeap(int capacity){
    min_heap* heap = (min_heap*)malloc(sizeof(min_heap));
    if(heap == NULL){
        printf("Failed to create heap\n");
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;

    // 分配节点数组的内存，使用 1-based 索引
    heap->nodes = (heap_node*)malloc((capacity + 1) * sizeof(heap_node));
    if (heap->nodes == NULL) {
        free(heap);
        printf("Failed to allocate memory for heap nodes\n");
        return NULL;
    }

    // 分配 position 数组的内存，并初始化为 -1，表示不在堆中
    heap->position = (int*)malloc((capacity) * sizeof(int));
    if (heap->position == NULL) {
        free(heap->nodes);
        free(heap);
        printf("Failed to allocate memory for position array\n");
        return NULL;
    }

    for(int i = 0; i <= capacity; i++){ // 0 位置未使用
        heap->nodes[i].dist = INT_MAX;
        heap->nodes[i].vertex = i;
        if(i < capacity) {
            heap->position[i] = -1; // 初始化为不在堆中
        }
    }

    return heap;
}

// 插入最小堆
void insertMinHeap(min_heap* heap, int vertex, int distance){
    if(heap->size >= heap->capacity){
        printf("Heap is full\n");
        return;
    }

    // 插入堆中，使用 1-based 索引
    heap->size += 1;
    int position = heap->size;
    heap->nodes[position].vertex = vertex;
    heap->nodes[position].dist = distance;

    // 更新 position 数组
    heap->position[vertex] = position;

    // 上浮操作以维持堆性质
    while(position > 1 && heap->nodes[position].dist < heap->nodes[position/2].dist){
        // 交换位置
        heap_node temp = heap->nodes[position];
        heap->nodes[position] = heap->nodes[position/2];
        heap->nodes[position/2] = temp;

        // 更新 position 数组
        heap->position[heap->nodes[position].vertex] = position;
        heap->position[heap->nodes[position/2].vertex] = position / 2;

        position /= 2;
    }
}

// 提取最小元素
heap_node* extractMin(min_heap* heap){
    if(heap->size == 0){
        printf("The heap is empty\n");
        return NULL;
    }

    // 获取最小节点
    heap_node* min_node = (heap_node*)malloc(sizeof(heap_node));
    if(min_node == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }
    *min_node = heap->nodes[1];

    // 将最后一个节点移动到根位置
    heap_node last_node = heap->nodes[heap->size];
    heap->size -= 1;

    int curr_position = 1;
    int child;

    while(curr_position * 2 <= heap->size){
        child = curr_position * 2;
        // 找出左右子节点中较小的
        if(child < heap->size && heap->nodes[child + 1].dist < heap->nodes[child].dist){
            child += 1;
        }

        // 如果最后一个节点的距离大于子节点的距离，则下沉
        if(last_node.dist > heap->nodes[child].dist){
            heap->nodes[curr_position] = heap->nodes[child];
            // 更新 position 数组
            heap->position[heap->nodes[child].vertex] = curr_position;
            curr_position = child;
        }
        else{
            break;
        }
    }

    // 将最后一个节点放到正确的位置
    heap->nodes[curr_position] = last_node;
    heap->position[last_node.vertex] = curr_position;

    // 清除提取节点的位置
    heap->position[min_node->vertex] = -1;

    return min_node;
}

// 修改最小堆中的节点距离
void decreaseKey(min_heap* heap, int vertex, int newDistance){
    // 检查输入是否合法
    if(vertex < 0 || vertex >= heap->capacity || newDistance > heap->nodes[heap->position[vertex]].dist){
        printf("Invalid input for decreaseKey!\n");
        return;
    }

    // 获取顶点在堆中的位置
    int pos = heap->position[vertex];

    // 更新距离
    heap->nodes[pos].dist = newDistance;

    // 上浮操作以维持堆性质
    while(pos > 1 && heap->nodes[pos].dist < heap->nodes[pos/2].dist){
        // 交换节点
        heap_node temp = heap->nodes[pos];
        heap->nodes[pos] = heap->nodes[pos/2];
        heap->nodes[pos/2] = temp;

        // 更新 position 数组
        heap->position[heap->nodes[pos].vertex] = pos;
        heap->position[heap->nodes[pos/2].vertex] = pos / 2;

        pos /= 2;
    }
}

// Dijkstra 算法实现
int query(graph* graph, int src, int dest){
    int num_vertices = graph->num_vertex;

    // 分配并初始化 distance 和 known 数组
    int* dist = (int*)malloc(num_vertices * sizeof(int));
    int* known = (int*)malloc(num_vertices * sizeof(int));

    if(dist == NULL || known == NULL){
        printf("Failed to allocate memory for distance or known arrays\n");
        return INT_MAX;
    }

    for(int i = 0; i < num_vertices; i++){
        dist[i] = INT_MAX;
        known[i] = 0;
    }

    // 创建并初始化堆
    min_heap* heap = createMinHeap(num_vertices);
    if(heap == NULL){
        printf("Failed to create heap\n");
        free(dist);
        free(known);
        return INT_MAX;
    }

    // 初始化源节点
    dist[src] = 0;
    insertMinHeap(heap, src, dist[src]);

    while(heap->size > 0){
        heap_node* curr_node = extractMin(heap);
        if(curr_node == NULL){
            break;
        }

        int u = curr_node->vertex;
        free(curr_node);

        if(known[u]){
            continue;
        }

        known[u] = 1;

        // 遍历所有邻接点
        adj_listnode* neighbor = graph->adj_list[u];
        while(neighbor != NULL){
            int v = neighbor->vertex;
            int weight = neighbor->weight;

            if(!known[v]){
                if(dist[u] != INT_MAX && (dist[u] + weight) < dist[v]){
                    dist[v] = dist[u] + weight;
                    
                    //如果还没有插入到堆中
                    if(heap->position[v] == -1){
                        insertMinHeap(heap, v, dist[v]);
                    }
                    else{
                        decreaseKey(heap, v, dist[v]);
                    }
                }
            }

            neighbor = neighbor->next;
        }
    }

    int distance = dist[dest];

    free(dist);
    free(known);
    free(heap->nodes);
    free(heap->position);
    free(heap);

    printf("The distance from %d to %d is %d\n", src, dest, distance);
    return distance;
}


