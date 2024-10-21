// main.c
/*
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "fibheap.h"
#include "graph.h"
#include <time.h>
#include <string.h>

int main() {
    int src, dest, weight, num_edges;
    char* all_files[8] = {
        "USA-road-d.BAY.gr",
        "USA-road-d.CAL.gr",
        "USA-road-d.COL.gr",
        "USA-road-d.FLA.gr",
        "USA-road-d.LKS.gr",
        "USA-road-d.NE.gr",
        "USA-road-d.NW.gr",
        "USA-road-d.NY.gr"
    };
    double time_costs[8];

    for (int i = 0; i < 8; i++) {
        char* file_name = all_files[i];

        // 创建图，假设顶点编号从 0 到 MAX_VERTEX-1
        graph* graph = createGraph(MAX_VERTEX);
        if (graph == NULL) {
            printf("图创建失败！\n");
            continue;
        }

        FILE* file = fopen(file_name, "r");
        if (file == NULL) {
            printf("无法打开文件: %s\n", file_name);
            // 释放图的内存
            free(graph->adj_list);
            free(graph);
            continue;
        }

        // 读取图的信息
        fscanf(file, "%d", &num_edges);
        printf("当前文件: %s\n", file_name);
        printf("边的数量: %d\n", num_edges);

        // 读取每条边的数据，假设每行格式为: a src dest weight
        for (int j = 0; j < num_edges; j++) {
            char edge_prefix;
            if (fscanf(file, " %c %d %d %d", &edge_prefix, &src, &dest, &weight) != 4) {
                printf("读取边数据时出错，行数: %d\n", j + 1);
                break;
            }
            addEdge(graph, src, dest, weight);
        }
        fclose(file);

        // 随机生成查询对并进行 Dijkstra 查询
        int query_number = 1000;
        printf("查询次数: %d\n", query_number);
        clock_t start_time = clock();

        srand(time(NULL)); // 设置随机种子

        for (int q = 0; q < query_number; q++) {
            // 生成随机查询对
            src = rand() % num_edges/3; // 随机源节点
            dest = rand() % num_edges/3; // 随机目标节点

            // 调用 Dijkstra 算法查询最短路径
            int distance = query(graph, src, dest);
            // 可以根据需要存储或使用 distance
        }

        clock_t end_time = clock();
        double time_cost = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("%s: 时间消耗: %f 秒\n\n", file_name, time_cost);
        time_costs[i] = time_cost;

        // 释放图的内存
        for (int v = 0; v < graph->num_vertex; v++) {
            adj_listnode* current = graph->adj_list[v];
            while (current != NULL) {
                adj_listnode* temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(graph->adj_list);
        free(graph);
    }

    // 打印所有文件的时间消耗
    for (int i = 0; i < 8; i++)
        printf("文件 %s 的时间消耗: %f 秒\n", all_files[i], time_costs[i]);

    return 0;
}

*/

// main.c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "fibheap.h"
#include "graph.h"
#include <time.h>
#include <string.h>

int main() {
    int src, dest, weight, num_edges;
    char* all_files[8] = {
        "USA-road-d.BAY.gr",
        "USA-road-d.CAL.gr",
        "USA-road-d.COL.gr",
        "USA-road-d.FLA.gr",
        "USA-road-d.LKS.gr",
        "USA-road-d.NE.gr",
        "USA-road-d.NW.gr",
        "USA-road-d.NY.gr"
    };
    double time_costs[8];

    for (int i = 0; i < 8; i++) {
        char* file_name = all_files[i];

        // 创建图，假设顶点编号从 0 到 MAX_VERTEX-1
        graph* graph = createGraph(MAX_VERTEX);
        if (graph == NULL) {
            printf("图创建失败！\n");
            continue;
        }

        FILE* file = fopen(file_name, "r");
        if (file == NULL) {
            printf("无法打开文件: %s\n", file_name);
            // 释放图的内存
            free(graph->adj_list);
            free(graph);
            continue;
        }

        // 读取图的信息
        fscanf(file, "%d", &num_edges);
        printf("current file: %s\n", file_name);
        printf("number of edges: %d\n", num_edges);

        // 读取每条边的数据，假设每行格式为: a src dest weight
        for (int j = 0; j < num_edges; j++) {
            char edge_prefix;
            if (fscanf(file, "\na %d %d %d", &src, &dest, &weight) != 3) {
                printf("error reading edge data, line: %d\n", j + 1);
                break;
            }
            addEdge(graph, src, dest, weight);
        }
        fclose(file);

        // 随机生成查询对并进行 Dijkstra 查询
        int query_number = 5;
        printf("query times: %d\n", query_number);
        clock_t start_time = clock();

        srand(time(NULL)); // 设置随机种子

        for (int q = 0; q < query_number; q++) {
            // 生成随机查询对
            src = rand() % num_edges / 3; // 随机源节点
            dest = rand() % num_edges / 3; // 随机目标节点

            // 调用 Dijkstra 算法查询最短路径
            int distance = query(graph, src, dest);
            // 可以根据需要存储或使用 distance
        }

        clock_t end_time = clock();
        double time_cost = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("%s: 时间消耗: %f 秒\n\n", file_name, time_cost);
        time_costs[i] = time_cost;

        // 释放图的内存
        for (int v = 0; v < graph->num_vertex; v++) {
            adj_listnode* current = graph->adj_list[v];
            while (current != NULL) {
                adj_listnode* temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(graph->adj_list);
        free(graph);
    }

    // 打印所有文件的时间消耗
    for (int i = 0; i < 8; i++)
        printf("文件 %s 的时间消耗: %f 秒\n", all_files[i], time_costs[i]);

    return 0;
}
