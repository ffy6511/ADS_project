#include <stdio.h>
#include <stdlib.h>
#include "min_heap.h"
#include <time.h>

int main(){
    int src, dest, weight, num_edges, num_vertices;
    char file_name[100] = "USA-road-d.NY.gr";
    graph* graph = createGraph(MAX_VERTEX);
    
    FILE* file = fopen(file_name, "r");
    //读取图信息并构建
    fscanf(file,"%d", &num_edges);
    printf("current file: %s\n", file_name);
    printf("num_edges: %d\n",num_edges);

    // 读取每条边的数据，忽略每行的第一个字符'a'
    for (int i = 0; i < num_edges; i++) {
        char edge_prefix;
        fscanf(file, " %c %d %d %d", &edge_prefix, &src, &dest, &weight);

        addEdge(graph, src, dest, weight);
    }
    fclose(file);
    
    min_heap* heap = createMinHeap(MAX_VERTEX);
    //随机生成一对query,并循环1000次
    int query_number = 1000;
    double time_cost;
    printf("The number of queries is: %d\n", query_number);
    clock_t start_time = clock();   


    while(query_number-- > 0) {
        //生成随机查询对
        src = rand() % MAX_VERTEX; // 随机生成源节点
        dest = rand() % MAX_VERTEX; // 随机生成目标节点
         // 调用算法查询最短路径
        int distance = query(graph, src, dest);
    }
    clock_t end_time = clock();
    time_cost = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("The time cost is: %f s\n", time_cost);
}

// int main(){
//     int src, dest, weight, num_edges, num_vertices;
//     char edge_prefix;
//     graph* graph = createGraph(MAX_VERTEX);
//     printf("please input the number of edges: \n");
//     scanf("%d",&num_edges);
    
//     //读取图信息并构建
//     printf("num_edges: %d\n",num_edges);

//     for(int i = 0; i < num_edges ; i++){
//         scanf("%d %d %d",&src,&dest,&weight);
//         addEdge(graph, src, dest, weight);
//     }
    
//     min_heap* heap = createMinHeap(MAX_VERTEX);
//     //随机生成一对query,并循环1000次
//     int query_number = 1;
//     double time_cost;
//     printf("The number of queries is: %d\n", query_number);
//     clock_t start_time = clock();   


//     while(query_number-- > 0) {
//         printf("please input the source and destination of the query: \n");
//         scanf("%d %d",&src,&dest);
//          // 调用算法查询最短路径
//         int distance = query(graph, src, dest);
//     }
//     clock_t end_time = clock();
//     time_cost = (double)(end_time - start_time) / CLOCKS_PER_SEC;
//     printf("The time cost is: %f s\n", time_cost);
// }