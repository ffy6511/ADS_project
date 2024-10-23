#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>

# include "../include/fibheap.h"
# include "../include/leftistheap.h"
# include "../include/minheap.h"


/*****************************************************************************************
 * 这是图和算法实现的头文件，我建议详细看这一部分
 * 
 * 我们在这里把数据结构都include进来，但是具体数据结构是按照传入测试/算法函数的结构名确定的
 * 
 * 数据结构的选择，我们选取了三种代表性的
 * 斐波那契堆 二项队列有的斐波那契堆都有，二项队列没有的斐波那契堆也有，所以选择斐波那契堆，避免代码过长
 * 左式堆 左式堆与斜堆类似，都实现可能导致代码过长
 * 普通最小堆，这是一个数组版本的，在FDS学的，我们用这个和高级数据结构做对比
 * 
 *****************************************************************************************/


class Graph { //这是整个图的类，除了数据结构的实现，其他所有东西都在里面
private:
    // 邻接节点结构体 用于储存图的节点所连的边（我们采用了链表来实现）
    struct AdjListNode {
        int vertex; //第二节点
        int weight; //权重
        AdjListNode* next;//链表指针
        AdjListNode(int v, int w) : vertex(v), weight(w), next(nullptr) {} //初始化函数
    };


    int numVertices;                      // 图中的顶点数量
    int numEdges;                         // 图中的边数量

    std::vector<AdjListNode*> adjList;    // 邻接表
    std::mutex graphMutex;                // 保护图的互斥锁，用于多线程环境 

    // 私有成员函数：多线程处理一部分文件
    void processFilePart(const std::string& filename, std::streampos start, std::streampos end, int tqdm);

    // 私有成员函数：从文件中读取图
    void loadGraphFromFileMultithreaded(const std::string& filename, int numThreads);

public:

    // 构造函数：接受文件名并从文件中读取图（多线程）
    //构造函数里面就套了一个loadGraphFromFileMultithreaded函数，用于图的读取，也就是说这玩意是用来生成图的
    Graph(const std::string& filename, int numThreads);

    // 析构函数 释放空间
    ~Graph();

    // 显示图的邻接列表 不建议大图调用，经过测试，给定的数据集的最小的也会显示很多内容难以识别
    void displayGraph() const;

    //算法实现 之所以直接暴露在外面是因为你也可以选择直接拿这个算法当借口来测试，没有那麽多IO操作，简单直接，此处不设置示例
    //参数包括起始位置，终点，堆类型
    std::pair<std::vector<int>, int> dijkstra(int startVertex, int endVertex,  const std::string& heapType);

    //测试方法
    //这一部分在main里有详细讲
    double test(int queryNumber, const std::string& filename, const std::string& heapType, int num, int random, int a, int b);


    //两个封装起来的变量，表示顶点数目和边数目
    int getNumVertices() const {
        return numVertices;
    }

    int getNumEdges() const {
        return numEdges;
    }
};

#endif //GRAPH_H
