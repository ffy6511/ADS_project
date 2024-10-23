#include "../include/graph.h"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <stdio.h>
#include <algorithm>

/***********************************************
 * 
 * 这里比较抽象，我尽力把注释写明白一点
 * 
 * 建议先看数据结构的实现，因为写的都是模板类，在实例化时选择elementtype
 * 
 * 
 * 
 * 
 * 
 ***********************************************/




// 构造函数：接受文件名并从文件中读取图（多线程）
Graph::Graph(const std::string& filename, int numThreads) : numVertices(0), numEdges(0) {
    loadGraphFromFileMultithreaded(filename, numThreads);
}

// 析构函数：释放动态分配的内存
Graph::~Graph() { // 主要是释放邻接表的内存
    for (int i = 0; i < numVertices; ++i) {
        AdjListNode* node = adjList[i]; 
        while (node != nullptr) {
            AdjListNode* temp = node;
            node = node->next;
            delete temp;
        }
    }
}

/********************************
 * 在文件中，每一行都有个标识符
 * c开头是注释
 * p开头是顶点数和边数
 * a开头是具体的一条一条边
 ********************************/

// 私有成员函数：多线程处理一部分文件
void Graph::processFilePart(const std::string& filename, std::streampos start, std::streampos end, int tqdm) {
    //tqdm参数请无视它，这里本来有进度条实现的，但是在WSL和VS里结果不一样，所以废弃了，保留这个参数为了保留这个功能的实现空间
    std::ifstream infile(filename);//文件打开为流
    if (!infile.is_open()) {
        throw std::runtime_error("文件打开错误：" + filename);
    }//抛出错误信息 在main被捕获

    infile.seekg(start);  // 从文件的指定位置开始读取，这是多线程的实现函数
    std::string line; //一行一行的开始读取
    while (infile.tellg() < end && std::getline(infile, line)) {
        if (line[0] == 'a') { 
            int src, dest, weight;//a的处理
            std::istringstream iss(line);
            char a;
            iss >> a >> src >> dest >> weight; // 输入的a就是开头字符，但是没用

            src -= 1;   // 转为从0开始的索引 因为数据里面的节点是：12345678
            dest -= 1;  // 我们想要把它变成                      01234567

            // 添加边，使用互斥锁保证线程安全
            {
                std::lock_guard<std::mutex> lock(graphMutex);  //互斥锁，如果两个线程同时要操作链表，互斥锁可以保证不产生冲突，依序操作
                AdjListNode* newNode = new AdjListNode(dest, weight); //新建节点
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
    infile.close(); //文件关闭
}

// 私有成员函数：从文件中读取图（多线程，按行分割，因为按照字节分会把一行掰断）
void Graph::loadGraphFromFileMultithreaded(const std::string& filename, int numThreads) {
    std::ifstream infile(filename, std::ios::ate);  // 打开文件并定位到文件末尾
    if (!infile.is_open()) {
        throw std::runtime_error("文件打开错误： " + filename);
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
            iss >> type >> type >> numVertices >> numEdges; //总的图信息，在p一行
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
        infile.seekg(i * partSize);  // 定位到每个部分的大致开始位置（按字节划分）
        // 跳到下一行，避免截断 
        std::string dummyLine;
        std::getline(infile, dummyLine);
        threadStarts.push_back(infile.tellg());  // 每个线程的起始点
    }

    // 最后一个线程处理到文件末尾
    threadStarts.push_back(fileSize);

    // 创建线程并分配工作
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) { //线程创建
        std::streampos start = threadStarts[i];
        std::streampos end = threadStarts[i + 1];
        threads.emplace_back(&Graph::processFilePart, this, filename, start, end, i);
        //把创建好的工作放入线程池 
    }

    // 等待所有线程完成
    for (auto& th : threads) {
        th.join(); //挂起
    }

    infile.close();//完毕
}


// 显示图的邻接列表 
void Graph::displayGraph() const {
    for (int i = 0; i < numVertices; ++i) {
        AdjListNode* node = adjList[i];
        std::cout << "Vertex " << i << ":";
        while (node != nullptr) {
            std::cout << " -> " << node->vertex << "(weight: " << node->weight << ")"; //按照 根->节点1->节点2->...的方式
            node = node->next;
        }
        std::cout << std::endl;
    }
}


//迪杰斯特拉算法的具体实现

/***************************************************************************************** 
 * 参数说明
 * 返回值是一个pair<vector<int>, int>类型，第一个是路径数组，第二个是路径长度
 * 参数的start和end表示开始点和结束点（从0开始编码）
 * heaptype就是堆结构
 * 
 * 我们选择堆结构实现的方式时每个结构都有一套算法，类似于C的风格
 * 因为我们的数据结构是模板类，模板类的继承会导致代码复杂难懂
 ******************************************************************************************/

std::pair<std::vector<int>, int> Graph::dijkstra(int startVertex, int endVertex, const std::string& heapType) {
    // 初始化
    std::vector<int> distances(numVertices, std::numeric_limits<int>::max());//距离数组
    std::vector<int> predecessor(numVertices, -1);  // 前驱数组
    distances[startVertex] = 0; //把起点距离设置为0

    // 根据heapType进行选择 
    // 使用斐波那契堆
    //我们选择堆结构实现的方式时每个结构都有一套算法，类似于C的风格
    //因为我们的数据结构是模板类，模板类的继承会导致代码复杂难懂

    if (heapType == "fib") {
        FibonacciHeap<int> fibHeap;//实例化，用int作为堆元素
        std::vector<FibonacciHeap<int>::Node*> nodes(numVertices, nullptr);//初始化

        for (int i = 0; i < numVertices; ++i) {
            nodes[i] = new FibonacciHeap<int>::Node(distances[i], i);//结构函数，包含val和key两个参数，可以去头文件查找
            fibHeap.insert(distances[i], i);//插入必须要保证两个，是因为要想办法建立节点编号和距离的对应关系
        }//把所有的距离入堆

        while (!fibHeap.isEmpty()) {
            int u = fibHeap.getMin()->val;  // 当前距离最小的顶点 注意getmin是出来一个结构体，我们要选择编号val作为u
            fibHeap.deleteMin(); //删除最小顶点，也就是标记为visited

            if (u == endVertex) break;  // 目标顶点已处理完毕，终止循环

            //野指针访问 报错
            if (u >= adjList.size()) {
                throw std::runtime_error("野指针");
            }

            AdjListNode* current = adjList[u]; 

            while (current != nullptr) { //更新距离部分 
                int v = current->vertex; //current的邻接表，就是它连接到的点
                int weight = current->weight; 

                if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    //如果更新后距离变小 那么更新
                    distances[v] = distances[u] + weight;
                    predecessor[v] = u;  // 记录前驱节点
                    fibHeap.decreaseKey(nodes[v], distances[v]);
                    //decreasekey操作减少键值
                }
                current = current->next;//下一节点，循环
            }
        }
    }

    // 使用左式堆 类似
    else if (heapType == "left") {
        LeftistHeap<std::pair<int, int>> leftistHeap; // 使用 std::pair<int, int> 作为堆元素，表示节点编号-键值对
        std::vector<int> nodes(numVertices, std::numeric_limits<int>::max());

        // 初始化堆
        for (int i = 0; i < numVertices; ++i) {
            nodes[i] = distances[i];
            leftistHeap.insert({ i, distances[i] }); // 将顶点和对应的距离插入堆
        }
        while (!leftistHeap.isEmpty()) {
            auto minNode = leftistHeap.getMin(); // 当前距离最小的顶点 也是取first表示编号
            int u = minNode.first; // 顶点 u
            leftistHeap.deleteMin();

            if (u == endVertex) break; // 目标顶点已处理完毕，终止循环

            AdjListNode* current = adjList[u];
            while (current != nullptr) { //更新路径
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
    //普通最小堆
    else if (heapType == "min") {
        MinHeap<std::pair<int, int>> minHeap; // 使用 std::pair<int, int> 作为堆元素
        std::vector<bool> visited(numVertices, false); // 用来记录顶点是否已经处理过
        //这里选择反着操作，就是不再全部插入堆，而是把顶点检测完后入堆，类似于FDS里写的
        distances[startVertex] = 0;
        // 初始化堆，插入起始顶点
        minHeap.insert({ startVertex, distances[startVertex] });
        while (!minHeap.isEmpty()) {
            auto minNode = minHeap.deleteMin(); // 当前距离最小的顶点
            int u = minNode.first; // 顶点 u
            // 如果该顶点已经处理过，跳过
            if (visited[u]) continue;
            // 标记该顶点为已访问
            visited[u] = true;
            if (u == endVertex) break; // 目标顶点已处理完毕，终止循环
            // 遍历邻接表的节点
            AdjListNode* current = adjList[u];
            while (current != nullptr) {
                int v = current->vertex;
                int weight = current->weight;
                // 检查是否找到更短的路径
                if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessor[v] = u; // 记录前驱节点
                    // 如果堆中已经包含顶点 v，调用 decreaseKey；否则直接插入
                    if (minHeap.hasVertex(v)) { //hasVertex 方法检查顶点是否在堆中
                        minHeap.decreaseKey(v, distances[v]);
                    }
                    else {
                        minHeap.insert({ v, distances[v] });
                    }
                }
                current = current->next;
            }
        }
    }
    else {
        throw std::invalid_argument("不存在该结构");
    }

    // 构建从startVertex到endVertex的路径
    std::vector<int> path;
    for (int at = endVertex; at != -1; at = predecessor[at]) {
        path.push_back(at); //每一个点，入队。
    }
    std::reverse(path.begin(), path.end()); // 反转路径，因为我们是从终点回溯到起点

    // 如果path的第一个顶点不是startVertex，说明没有有效路径
    if (path[0] != startVertex) {
        path.clear(); // 没有找到有效路径
    }

    // 返回路径和终点的最短距离
    return { path, distances[endVertex] };
}

//这是测试函数 我们测试的最重要接口 在main里对参数介绍比较详细
double Graph::test(int queryNumber, const std::string& filename, const std::string& heapType, int num, int random, int a, int b) {

    if (random) {//如果是随机生成数据
        // 设置随机种子 在随机生成数据要用
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
    else {//否则根据传入的a和b输出路径和结果
        auto result = dijkstra(a, b, heapType);//调用算法
        printf("长度是：%d\n", result.second);
        printf("最短路径是：");
        for (int j = 0; j < result.first.size(); j++) {
            printf("%d ",result.first[j]);
        }
        printf("\n");
        return 0;
    }
}
