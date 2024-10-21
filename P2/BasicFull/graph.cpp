#include "graph.h"
//#include "fibheap.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

// 构造函数：接受文件名并从文件中读取图（多线程）
Graph::Graph(const std::string& filename, int numThreads) : numVertices(0), numEdges(0) {
    loadGraphFromFileMultithreaded(filename, numThreads);
}

// 析构函数：释放动态分配的内存
Graph::~Graph() {
    for (int i = 0; i < numVertices; ++i) {
        AdjListNode* node = adjList[i];
        while (node != nullptr) {
            AdjListNode* temp = node;
            node = node->next;
            delete temp;
        }
    }
}

// 私有成员函数：多线程处理一部分文件
void Graph::processFilePart(const std::string& filename, std::streampos start, std::streampos end) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    infile.seekg(start);  // 从文件的指定位置开始读取
    std::string line;
    while (infile.tellg() < end && std::getline(infile, line)) {
        if (line[0] == 'a') {
            int src, dest, weight;
            std::istringstream iss(line);
            char a;
            iss >> a >> src >> dest >> weight;

            src -= 1;  // 转为从0开始的索引
            dest -= 1;

            // 添加边，使用互斥锁保证线程安全
            {
                std::lock_guard<std::mutex> lock(graphMutex);
                AdjListNode* newNode = new AdjListNode(dest, weight);
                newNode->next = adjList[src];
                adjList[src] = newNode;

                // 如果是无向图，添加反向边
                /*
                newNode = new AdjListNode(src, weight);
                newNode->next = adjList[dest];
                adjList[dest] = newNode;
                */
            }
        }
    }

    infile.close();
}

//GPT，让GPT改的多线程，我自己写的是单线程函数（
// 私有成员函数：从文件中读取图（多线程）
void Graph::loadGraphFromFileMultithreaded(const std::string& filename, int numThreads) {
    std::ifstream infile(filename, std::ios::ate);  // 打开文件并定位到文件末尾
    if (!infile.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    // 获取文件大小
    std::streampos fileSize = infile.tellg();
    infile.seekg(0, std::ios::beg);  // 重置到文件开头

    // 读取文件头部信息，找到顶点和边的数量
    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] == 'p') {
            std::istringstream iss(line);
            std::string type;
            iss >> type >> type >> numVertices >> numEdges;
            break;
        }
    }

    // 初始化邻接列表
    adjList.resize(numVertices, nullptr);

    // 计算每个线程处理的文件部分大小
    std::streamoff partSize = static_cast<std::streamoff>(fileSize) / numThreads;

    // 创建线程并分配工作
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        std::streampos start = static_cast<std::streampos>(i * partSize);
        std::streampos end = (i == numThreads - 1) ? fileSize : static_cast<std::streampos>((i + 1) * partSize);
        threads.emplace_back(&Graph::processFilePart, this, filename, start, end);
    }

    // 等待所有线程完成
    for (auto& th : threads) {
        th.join();
    }

    infile.close();
}

// 显示图的邻接列表
void Graph::displayGraph() const {
    for (int i = 0; i < numVertices; ++i) {
        AdjListNode* node = adjList[i];
        std::cout << "Vertex " << i << ":";
        while (node != nullptr) {
            std::cout << " -> " << node->vertex << "(weight: " << node->weight << ")";
            node = node->next;
        }
        std::cout << std::endl;
    }
}


//TODO：改进结构，可以在类中添加一个输入来完成数据结构的选择，我现在是只有斐波那契堆。

std::pair<std::vector<int>, int> Graph::dijkstra(int startVertex, int endVertex) {
    // 初始化距离、前驱和优先级队列


    std::vector<int> distances(numVertices, std::numeric_limits<int>::max());
    std::vector<int> predecessor(numVertices, -1); // 前驱数组
    distances[startVertex] = 0;


    FibonacciHeap<int> fibHeap;
    std::vector<FibonacciHeap<int>::Node*> nodes(numVertices, nullptr);

    for (int i = 0; i < numVertices; ++i) {
        nodes[i] = new FibonacciHeap<int>::Node(distances[i]);
        fibHeap.insert(distances[i]);
    }

    while (!fibHeap.isEmpty()) {
        int u = fibHeap.getMin(); // 当前距离最小的顶点
        fibHeap.deleteMin();

        if (u == endVertex) break; // 目标顶点已处理完毕，终止循环

        AdjListNode* current = adjList[u];
        while (current != nullptr) {
            int v = current->vertex;
            int weight = current->weight;

            if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                predecessor[v] = u; // 记录前驱节点
                fibHeap.decreaseKey(nodes[v], distances[v]);
            }
            current = current->next;
        }
    }

    // 构建从startVertex到endVertex的路径
    std::vector<int> path;
    for (int at = endVertex; at != -1; at = predecessor[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end()); // 反转路径，因为我们是从终点回溯到起点

    // 如果path的第一个顶点不是startVertex，说明没有有效路径
    if (path[0] != startVertex) {
        path.clear(); // 没有找到有效路径
    }

    // 返回路径和终点的最短距离
    return { path, distances[endVertex] };
}

