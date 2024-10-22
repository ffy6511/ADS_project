#include "graph.h"
//#include "fibheap.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <stdio.h>

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
void Graph::processFilePart(const std::string& filename, std::streampos start, std::streampos end, int tqdm) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    infile.seekg(start);  // 从文件的指定位置开始读取
    std::string line;
    while (infile.tellg() < end && std::getline(infile, line)) {
        //if (!tqdm) {
        //本来想搞个进度条
        //}
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

        /*被废弃的进度条
        {
            std::lock_guard<std::mutex> lock(progressMutex);
            bytesRead += line.size() + 1;  // 更新读取的字节数
            float progress = static_cast<float>(bytesRead) / totalSize;
            displayProgress(progress);  // 显示进度条
        }
        */
    }

    infile.close();
}

//GPT，让GPT改的多线程，我自己写的是单线程函数（
// 私有成员函数：从文件中读取图（多线程）
// 私有成员函数：从文件中读取图（多线程，按行分割）
//原来按照字节截断，
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

    // 找到每个线程应该开始的位置，按行划分
    std::vector<std::streampos> threadStarts;
    threadStarts.push_back(infile.tellg());  // 第一个线程从当前文件位置开始

    std::streamoff partSize = static_cast<std::streamoff>(fileSize) / numThreads;
    for (int i = 1; i < numThreads; ++i) {
        infile.seekg(i * partSize);  // 定位到每个部分的大致开始位置
        // 跳到下一行，避免截断
        std::string dummyLine;
        std::getline(infile, dummyLine);
        threadStarts.push_back(infile.tellg());  // 每个线程的起始点
    }

    // 最后一个线程处理到文件末尾
    threadStarts.push_back(fileSize);

    // 创建线程并分配工作
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        std::streampos start = threadStarts[i];
        std::streampos end = threadStarts[i + 1];
        threads.emplace_back(&Graph::processFilePart, this, filename, start, end, i);
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

std::pair<std::vector<int>, int> Graph::dijkstra(int startVertex, int endVertex, const std::string& heapType) {
    // 初始化距离、前驱和优先级队列

    std::vector<int> distances(numVertices, std::numeric_limits<int>::max());
    std::vector<int> predecessor(numVertices, -1);  // 前驱数组
    distances[startVertex] = 0;

    // 使用斐波那契堆
    if (heapType == "fib") {
        FibonacciHeap<int> fibHeap;
        std::vector<FibonacciHeap<int>::Node*> nodes(numVertices, nullptr);

        for (int i = 0; i < numVertices; ++i) {
            nodes[i] = new FibonacciHeap<int>::Node(distances[i], i);
            fibHeap.insert(distances[i], i);
        }

        while (!fibHeap.isEmpty()) {
            int u = fibHeap.getMin()->val;  // 当前距离最小的顶点
            fibHeap.deleteMin();

            //std::cout << u << std::endl;

            if (u == endVertex) break;  // 目标顶点已处理完毕，终止循环

            //野指针访问
            if (u >= adjList.size()) {
                throw std::runtime_error("Invalid vertex");  
            }

            AdjListNode* current = adjList[u];

            while (current != nullptr) {
                int v = current->vertex;
                int weight = current->weight;

                //printf("haha%d %d \n", v, weight);

                if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessor[v] = u;  // 记录前驱节点
                    fibHeap.decreaseKey(nodes[v], distances[v]);
                }
                current = current->next;
            }
        } 
    }

    // 使用左式堆
    else if (heapType == "left") {
        LeftistHeap<std::pair<int, int>> leftistHeap; // 使用 std::pair<int, int> 作为堆元素
        std::vector<int> nodes(numVertices, std::numeric_limits<int>::max());

        // 初始化堆
        for (int i = 0; i < numVertices; ++i) {
            nodes[i] = distances[i];
            leftistHeap.insert({ i, distances[i] }); // 将顶点和对应的距离插入堆
        }

        while (!leftistHeap.isEmpty()) {
            auto minNode = leftistHeap.getMin(); // 当前距离最小的顶点
            int u = minNode.first; // 顶点 u
            leftistHeap.deleteMin();

            if (u == endVertex) break; // 目标顶点已处理完毕，终止循环

            AdjListNode* current = adjList[u];
            while (current != nullptr) {
                int v = current->vertex;
                int weight = current->weight;

                // 检查是否找到更短的路径
                if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessor[v] = u; // 记录前驱节点
                    leftistHeap.insert({ v, distances[v] }); // 插入新的距离和顶点
                }
                current = current->next;
            }
        }
    }
    else if (heapType == "min") {
        MinHeap<std::pair<int, int>> minHeap; // 使用 std::pair<int, int> 作为堆元素
        std::vector<int> distances(numVertices, std::numeric_limits<int>::max());
        std::vector<int> predecessor(numVertices, -1); // 前驱数组
        distances[startVertex] = 0;

        // 初始化堆
        for (int i = 0; i < numVertices; ++i) {
            minHeap.insert({ i, distances[i] }); // 将顶点和对应的距离插入堆
        }

        while (!minHeap.isEmpty()) {
            auto minNode = minHeap.deleteMin(); // 当前距离最小的顶点
            int u = minNode.first; // 顶点 u

            if (u == endVertex) break; // 目标顶点已处理完毕，终止循环

            AdjListNode* current = adjList[u];
            while (current != nullptr) {
                int v = current->vertex;
                int weight = current->weight;

                // 检查是否找到更短的路径
                if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessor[v] = u; // 记录前驱节点

                    // 重新插入新的距离和顶点
                    minHeap.insert({ v, distances[v] }); // 重新插入新距离
                }
                current = current->next;
            }
        }
    }




    else {
        throw std::invalid_argument("Invalid heap type.");
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






double Graph::test(int queryNumber, const std::string& filename, const std::string& heapType, int num) {
    // 设置随机种子
    srand(time(NULL));
    

    // 开始计时
    clock_t startTime = clock();

    // 运行 Dijkstra 算法 queryNumber 次
    for (int i = 0; i < queryNumber; i++) {
        int src = rand() % num;   // 随机生成源顶点
        int dest = rand() % num;  // 随机生成目标顶点

        // 根据传入的 heapType 参数调用 Dijkstra 算法
        auto result = dijkstra(src, dest, heapType);
    }

    // 结束计时
    clock_t endTime = clock();

    // 计算耗时并转换为秒
    double timeTaken = double(endTime - startTime) / CLOCKS_PER_SEC;

    // 输出测试结果
    std::cout << heapType << " 堆下算法对 " << filename << " 运行 " << queryNumber << " 次的总时间(秒): " << timeTaken << std::endl;

    return timeTaken;
}












































/*
void Graph::displayProgress(float progress) {
    int barWidth = 70;  // 进度条宽度
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}
*///废弃进度条