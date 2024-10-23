<center><strong>
    <span  style="font-size: 35px">Shortest Path Algorithm with Heaps</span>   
</strong></center>





<center>Date: 2024-10-20
</center>



**目录**：

[TOC]

<div style="page-break-after: always;"></div>

# Chapter 1: Introduction
​	Dijkstra算法是一种用于寻找图中最短路径的贪心算法。该算法主要用于解决带权有向图或无向图中的单源最短路径问题，即从起始节点到其他所有节点的最短路径。	

​	本项目的目标是实现 Dijkstra 算法以解决最短路径问题，并比较不同数据结构（包括斐波那契堆和其他堆）在该算法中的性能表现，并找到最适合该算法的数据结构类型。

​	整体流程主要包括：

1. 实现算法
2. 使用美国公路网络数据集进行评估（生成至少1000对查询以测试运行时间）
3. 将评估结果以图表形式呈现
4. 分析结果



<div style="page-break-after: always;"></div>



# Chapter 2: **Data Structure / Algorithm Specification**

我们分别使用最小堆、左式堆和斐波那契堆作为数据结构来实现算法：

## 2.1 所用堆的模板类实现

### 2.1.1 最小堆

- ##### 上浮操作：

```pseudocode
FUNCTION heapify( index )
    WHILE index > 0 // 当前节点不是根节点时继续上浮
        parent_index = (index-1)/2 //计算父节点的索引（基于0-based)
        IF heap[index].second < heap[parent_index].second THEN //此处`second`在具体实现中指的就是`dist`优先级
            SWAP heap[index] WITH heap[parent_index] //交换节点
            index = parent_index //更新索引
        ELSE //当前节点满足最小堆的性质，停止上浮
            BREAK
        END IF
    END WHILE
END FUNCTION
    
```

- ##### 下滤操作：

```pseudocode
FUNCTION heapifyDown(index)
        WHILE TRUE
            leftChild = 2 * index + 1  // 左孩子索引
            rightChild = 2 * index + 2  // 右孩子索引
            smallest = index  // 假设当前节点是最小的

            // 检查左孩子
            IF leftChild < SIZE OF heap AND leftChild's priority < smallest's priority THEN
                smallest = leftChild  // 更新最小节点为左孩子
            END IF

            // 检查右孩子
            IF rightChild < SIZE OF heap AND rightChild's priority < smallest's priority THEN
                smallest = rightChild  // 更新最小节点为右孩子
            END IF

            // 如果最小节点不是当前节点，则交换
            IF smallest != index THEN
                SWAP current node WITH smallest  // 交换
                index = smallest  // 更新当前索引为最小节点索引
            ELSE
                BREAK  // 已满足堆性质，停止调整
            END IF
        END WHILE
    END FUNCTION

PUBLIC
    FUNCTION MinHeap()  // 构造函数
        // 初始化空堆
    END FUNCTION

    FUNCTION ~MinHeap()  // 析构函数
        // 无需特殊清理
    END FUNCTION

```

- ##### 插入新元素

```pseudocode
 FUNCTION insert(value)
        APPEND value TO heap  // 添加到堆末尾
        CALL heapifyUp(SIZE OF heap - 1)  // 调整堆
    END FUNCTION
```

- ##### 删除并返回最小元素

```pseudocode
    FUNCTION deleteMin() RETURNS pair<int, int>
        IF heap IS EMPTY THEN
            THROW "Heap is empty"  // 如果heap已经为空，抛出异常
        END IF

        minValue = heap[0]  // 保存堆顶元素
        heap[0] = last element  // 将最后一个元素移到堆顶
        REMOVE LAST ELEMENT FROM heap  // 移除最后元素
        IF heap IS NOT EMPTY THEN
            CALL heapifyDown(0)  // 通过下滤调整堆，将最后一个元素调整到合适为止
        END IF
        RETURN minValue  // 返回最小值
    END FUNCTION
```

- ##### 获取堆顶最小元素，但是不删除

```pseudocode
 FUNCTION getMin() RETURNS pair<int, int>
        IF heap IS EMPTY THEN
            THROW "Heap is empty"  // 如果heap为空，不存在最小元素，抛出异常
        END IF
        RETURN heap[0]  // 返回堆顶元素
    END FUNCTION
```

---

### 2.1.2 左式堆

- ##### 节点结构体

```pseudocode
 STRUCT Node
    T key  // 节点的值，可以是 std::pair<int, int>
    int npl  // null path length
    Node* left  // 左子节点
    Node* right  // 右子节点

    // 节点构造函数
    FUNCTION Node(T value)
        key = value
        npl = 0
        left = NULL
        right = NULL
    END FUNCTION
END STRUCT

```

- ##### 合并左式堆

```pseudocode
FUNCTION merge(Node* h1, Node* h2) RETURNS Node*
        IF h1 IS NULL THEN RETURN h2  // 如果 h1 为空，返回 h2
        IF h2 IS NULL THEN RETURN h1  // 如果 h2 为空，返回 h1

        // 确保 h1 的键小于 h2 的键
        IF h1->key.second > h2->key.second THEN
            SWAP h1 WITH h2  // 交换
        END IF

        // 递归合并 h1 的右子树和 h2
        h1->right = merge(h1->right, h2)

        // 确保左子树的 NPL（null path length）大于等于右子树
        IF h1->left IS NULL OR (h1->right IS NOT NULL AND h1->left->npl < h1->right->npl) THEN
            SWAP h1->left WITH h1->right  // 交换左右子树
        END IF

        // 更新 h1 的 NPL
        h1->npl = (h1->right IS NOT NULL ? h1->right->npl + 1 : 0)
        RETURN h1  // 返回合并后的堆
    END FUNCTION
```

- ##### 插入新的元素

```pseudocode
 FUNCTION insert(T value)
        Node* newNode = NEW Node(value)  // 创建新节点
        root = merge(root, newNode)  // 合并新节点与当前堆
END FUNCTION
```

- ##### 删除并返回堆顶元素

```pseudocode
FUNCTION deleteMin() RETURNS std::pair<int, int>
    IF root IS NULL THEN
        THROW "Heap is empty"  // 抛出异常
    END IF

    minValue = root->key  // 保存最小元素
    Node* oldRoot = root  // 保存旧根节点
    root = merge(root->left, root->right)  // 合并左子树和右子树
    DELETE oldRoot  // 删除旧根节点
    RETURN minValue  // 返回最小元素
END FUNCTION
```

- ##### 获取堆顶的最小元素

```pseudocode
FUNCTION getMin() RETURNS std::pair<int, int>
    IF root IS NULL THEN
        THROW "Heap is empty"  // 如果heap为空，不存在最小元素，抛出异常
    END IF
    RETURN root->key  // 返回最小元素
END FUNCTION
```

- ##### 中序遍历显示堆中元素

```pseudocode
FUNCTION inorder(Node* node)  // 递归遍历节点
    IF node IS NOT NULL THEN
        inorder(node->left)  // 遍历左子树
        OUTPUT "(" + node->key.first + ", " + node->key.second + ") "  // 输出当前节点
        inorder(node->right)  // 遍历右子树
    END IF
END FUNCTION
```

---

### 2.1.3 斐波那契堆

- ##### 节点结构体

```pseudocode
STRUCT Node
    T key         // 存储的值（键）
    int val       // 顶点的值
    int degree    // 子节点的数量
    Node* parent  // 父节点
    Node* child   // 子节点
    Node* left    // 左兄弟
    Node* right   // 右兄弟
    bool mark     // 标记节点是否被切割

    // 节点构造函数
    FUNCTION Node(T k, int v)
        key = k
        val = v
        degree = 0
        //初始化parent 与 child都为空；
        //左右子节点指向自己，形成双向循环链表
        parent = NULL
        child = NULL
        left = this
        right = this
        mark = false
    END FUNCTION
END STRUCT
```

- ##### 插入新元素

```pseudocode
FUNCTION insert(T value, int vertex)
    Node* node = NEW Node(value, vertex)
    //如果堆为空，将该节点设置为最小节点
    IF minNode IS NULL THEN
        minNode = node
    ELSE
        // 将新节点加入到根列表中
        node->left = minNode
        node->right = minNode->right
        minNode->right->left = node
        minNode->right = node
        IF value < minNode->key THEN
            minNode = node  // 更新最小节点
        END IF
    END IF
    heapSize++ //堆中节点个数自增
END FUNCTION
```

- ##### 删除最小节点

```pseudocode
FUNCTION deleteMin()
    IF minNode IS NULL THEN RETURN  // 如果堆为空，返回

    Node* oldMin = minNode
    IF oldMin->child IS NOT NULL THEN
        // 将子节点加入根列表
        Node* child = oldMin->child
        DO
            child->parent = NULL
            child = child->right
        WHILE child != oldMin->child

        // 连接子节点与根列表
        CONNECT child NODES
    END IF
	// 从根列表中移除最小节点
    REMOVE oldMin FROM ROOT LIST
    IF minNode IS minNode->right THEN
        minNode = NULL
    ELSE
        minNode = minNode->right
        consolidate()  // 合并根列表
    END IF
    DELETE oldMin
    heapSize--
END FUNCTION

```

- ##### 减小节点的键值

```pseudocode
 FUNCTION decreaseKey(Node* node, T newValue)
 	//检查新的键值是否满足输入要求
    IF newValue > node->key THEN
        THROW "New value is greater than current value." //异常则抛出警告
    END IF
    
    node->key = newValue
    Node* parent = node->parent

    IF parent IS NOT NULL AND node->key < parent->key THEN
        cut(node, parent)  // 切割
        cascadingCut(parent)  // 级联切割
    END IF

    IF minNode IS NOT NULL AND node->key < minNode->key THEN
        minNode = node  // 更新最小节点
    END IF
END FUNCTION
```

- ##### 将两个节点连接

```pseudocode
FUNCTION link(Node* y, Node* x)
    REMOVE y FROM ROOT LIST
    // 将 y 连接到 x 的子节点
    y->parent = x
    IF x->child IS NULL THEN
        x->child = y
        y->left = y->right = y
    ELSE
        // 将 y 插入到 x 的子列表中
        INSERT y INTO CHILD LIST OF x
    END IF
    x->degree++  // 更新 x 的度数
    y->mark = false  // 重置标记
END FUNCTION
```

- ##### 合并根列表

```pseudocode
FUNCTION consolidate()
    int D = LOG2(heapSize) + 1  // 计算最大度数
    CREATE ARRAY A[D]  // 初始化根列表

    // 遍历根节点，合并同样度数的节点
    FOR EACH node IN rootList DO
        Node* x = node
        int d = x->degree
        WHILE A[d] IS NOT NULL DO
            Node* y = A[d]
            IF x->key > y->key THEN
                SWAP x WITH y  // 确保 x 是最小的
            END IF
            link(y, x)  // 连接
            A[d] = NULL  // 清空
            d++
        END WHILE
        A[d] = x  // 将 x 放入数组
    END FOR
    // 重新建立最小节点
    minNode = NULL
    FOR EACH node IN A DO
        IF node IS NOT NULL THEN
            ADD node TO ROOT LIST
            IF minNode IS NULL OR node->key < minNode->key THEN
                minNode = node  // 更新最小节点
            END IF
        END IF
    END FOR
END FUNCTION
```

- ##### 切割节点

```pseudocode
FUNCTION cut(Node* x, Node* y)
    // 从父节点的子列表中移除 x
    REMOVE x FROM y->child
    y->degree--

    // 将 x 添加到根列表
    ADD x TO ROOT LIST
    x->parent = NULL
    x->mark = false
END FUNCTION
```

- ##### 级联切割

```pseudocode
FUNCTION cascadingCut(Node* y)
    Node* z = y->parent
    IF z IS NOT NULL THEN
        IF NOT y->mark THEN
            y->mark = true  // 标记mark，表示节点被切割
        ELSE
            cut(y, z)  // 进行切割
            cascadingCut(z)  // 对父节点递归进行级联切割
        END IF
    END IF
END FUNCTION
```

- ##### 删除指定节点

```pseudocode
FUNCTION deleteNode(Node* node)
    decreaseKey(node, MIN_VALUE)  // 将节点值减至最小
    deleteMin()  // 删除最小节点
END FUNCTION
```



## 2.2 主函数分析

### 2.2.1 整体流程：

**1. 初始化和设置**：

- 定义常量（如 `DEMO`）。
- 创建一个包含图文件名的数组。
- 设置线程数量以优化文件读取。

**2.  循环读取文件**：

>  对于每个图文件：

- 输出当前读取的文件名。
- 开始计时，创建图对象并加载数据。
- 停止计时，输出读取成功的信息。

**3.  性能测试**：

- 对加载的图执行多个性能测试，使用不同的堆实现（如包括斐波那契堆、左式堆和二叉堆）。

---

### 2.2.2 多线程读取文件

在读取大规模的图信息时，我们采取多线程读取的方式来加快文件读取的处理速度：

```c++
void Graph::loadGraphFromFileMultithreaded(const std::string& filename, int numThreads) {
    std::ifstream infile(filename, std::ios::ate);  // 打开文件
    if (!infile.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);  // 异常处理
    }

    std::streampos fileSize = infile.tellg();  // 获取文件大小
    infile.seekg(0, std::ios::beg);  // 重置到文件开头

    // 读取图的顶点和边的数量
    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] == 'p') {  // 处理文件头
            std::istringstream iss(line);
            std::string type;
            iss >> type >> type >> numVertices >> numEdges;  // 解析
            break;  // 结束循环
        }
    }

    adjList.resize(numVertices, nullptr);  // 初始化邻接列表

    // 计算每个线程的起始位置
    std::vector<std::streampos> threadStarts;
    threadStarts.push_back(infile.tellg());  // 第一个线程的起始位置

    std::streamoff partSize = static_cast<std::streamoff>(fileSize) / numThreads;  // 部分大小
    for (int i = 1; i < numThreads; ++i) {
        infile.seekg(i * partSize);  // 定位到部分开始
        std::string dummyLine;
        std::getline(infile, dummyLine);  // 跳到下一行
        threadStarts.push_back(infile.tellg());  // 记录起始点
    }

    threadStarts.push_back(fileSize);  // 文件末尾

    // 创建并启动线程
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        std::streampos start = threadStarts[i];
        std::streampos end = threadStarts[i + 1];
        threads.emplace_back(&Graph::processFilePart, this, filename, start, end, i);  // 启动线程
    }

    // 等待线程完成
    for (auto& th : threads) {
        th.join();  // 等待结束
    }

    infile.close();  // 关闭文件
}

```

> 1. 首先打开文件并定位到文件末尾，如果失败则抛出异常信号
> 2. 通过`infile.tellg();`获取文件末尾的位置，然后重置到文件开头
> 3. 读取文件头部信息，找到顶点和边的数量进而初始化图（邻接链表的形式）
> 4. `std::vector<std::streampos> threadStarts; `存储各线程的起始位置，并通过`static_cast<std::streamoff>(fileSize) / numThreads;`计算各个线程需要处理的文件大小；将先前存储的文件末尾位置作为最后一个线程的结束位置
> 5. 调用`processFilePart`执行各个线程的读取工作



- ##### `processFilePart` 单线程的读取工作

```cpp
void Graph::processFilePart(const std::string& filename, std::streampos start, std::streampos end, int tqdm) {
    std::ifstream infile(filename);  // 打开指定文件
    if (!infile.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);  // 如果打开失败，抛出异常
    }

    infile.seekg(start);  // 从文件的指定位置开始读取
    std::string line;
    
    // 读取文件内容直到文件结束或到达指定结束位置
    while (infile.tellg() < end && std::getline(infile, line)) {

        // 处理以 'a' 开头的边信息
        if (line[0] == 'a') {
            int src, dest, weight;  // 源节点、目标节点和权重
            std::istringstream iss(line);  // 创建输入流
            char a;  // 用于读取行首字符
            iss >> a >> src >> dest >> weight;  // 解析边信息

            src -= 1;  // 转为从0开始的索引
            dest -= 1;

            // 添加边，使用互斥锁保证线程安全
            {
                std::lock_guard<std::mutex> lock(graphMutex);  // 锁住互斥量
                AdjListNode* newNode = new AdjListNode(dest, weight);  // 创建新邻接节点
                newNode->next = adjList[src];  // 将新节点插入邻接列表
                adjList[src] = newNode;
            }
        }

    }

    infile.close();  // 关闭文件
}

```

> 通过`loadGraphFromFileMultithreaded`函数给出的文件位置，定位到该线程需要处理的文件位置，然后读取文件内容来构建图的部分。



# Chapter 3: **Testing Results**

## 3.1 正确性测试

- 测试目的：利用小规模的数据、边界数据验证基本功能的正确性

### 3.2 性能型测试

- 测试目的：利用`USA road networks`的数据集测试，以比较不同堆的性能

- ##### `run time table` 

| 数据集 | 最小堆 | 左式堆 | 斐波那契堆 |
| :----: | :----: | :----: | ---------- |
|        |        |        |            |
|        |        |        |            |
|        |        |        |            |
|        |        |        |            |
|        |        |        |            |
|        |        |        |            |
|        |        |        |            |
|        |        |        |            |
|        |        |        |            |

- ##### `Plot`



# Chapter 4: **Analysis and Comments**

## 4.1 时间复杂度分析

## 4.2 空间复杂度分析

### 4.2.1 读入数据集的空间复杂度

### 4.2.2 算法处理的空间复杂度



# **Appendix: Source Code** 

# Others

### References

### Author List

### Declaration

### Signatures

![ ](C:\Users\86150\AppData\Roaming\Typora\typora-user-images\image-20241020134537711.png)
