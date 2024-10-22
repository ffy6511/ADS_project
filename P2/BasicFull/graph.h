#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>

#include "fibheap.h"

class Graph {
private:
    // 邻接节点结构体
    struct AdjListNode {
        int vertex;
        int weight;
        AdjListNode* next;
        AdjListNode(int v, int w) : vertex(v), weight(w), next(nullptr) {}
    };


    int numVertices;                      // 图中的顶点数量
    int numEdges;                         // 图中的边数量
    std::vector<AdjListNode*> adjList;    // 邻接表
    std::mutex graphMutex;                // 保护图的互斥锁，用于多线程环境
    //GPT：GPT给我加的

    // 私有成员函数：多线程处理一部分文件
    void processFilePart(const std::string& filename, std::streampos start, std::streampos end);

    // 私有成员函数：从文件中读取图
    void loadGraphFromFileMultithreaded(const std::string& filename, int numThreads);

public:
    // 构造函数：接受文件名并从文件中读取图（多线程）
    Graph(const std::string& filename, int numThreads);

    // 析构函数
    ~Graph();

    // 显示图的邻接列表
    void displayGraph() const;

    //返回图的顶点数
    int getNumVertices() const {
        return numVertices;
    }

    std::pair<std::vector<int>, int> dijkstra(int startVertex, int endVertex);

};

#endif //GRAPH_H
