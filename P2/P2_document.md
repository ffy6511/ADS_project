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

我们分别使用最小堆和斐波那契堆来实现算法：

## 2.1 Min_heap_version

### ①数据结构：

- ##### 结构伪代码：

```c
// 堆节点结构
STRUCT HeapNode
    INT dist                // 节点的距离
    INT vertex              // 节点的唯一标识

// 最小堆结构
STRUCT MinHeap
    HeapNode* nodes         // 堆节点数组，使用 1-based 索引
    INT size                 // 当前堆的大小
    INT capacity             // 堆的容量
    INT* position            // 辅助数组，position[vertex] = 在堆中的位置

// 邻接链表节点结构
STRUCT AdjListNode
    INT vertex              // 邻接点的唯一标识
    INT weight              // 边的权重
    AdjListNode* next       // 指向下一个邻接节点的指针

// 图的结构
STRUCT Graph
    INT num_vertex          // 图中顶点的数量
    AdjListNode** adj_list  // 邻接链表数组

```

> 1. 为了更好地处理稀疏图，图的边通过链表实现，即邻接表。该二维指针的每个一维指针，对应着索引处的顶点所指出的边；
> 2. 最小堆采用`1-based`索引，使用辅助数组`position`方便`decrease-key`操作时将顶点与堆节点一一对应

### ②主函数部分：

- ##### 整体流程：

1. 读取图信息，按照邻接表的方式存储
2. 初始化最小堆 (将`src`除外的节点`dist`设置为`INT_MAX`，表示无穷大)
3. 不断取出最小堆的堆顶元素，更新其邻接顶点的`dist`，直到堆中没有节点

- ##### 功能函数伪代码

1. **添加边**：

```c
FUNCTION addEdge(graph g, src, dest, weight)
    CREATE new_node of type AdjListNode
    SET new_node.vertex = dest
    SET new_node.weight = weight
    SET new_node.next = graph.adj_list[src]
    SET graph.adj_list[src] = new_node
END FUNCTION

```

> 此处采用有向图，并且通过让邻接表插入新节点到链表头，消除尾插入带来的时间成本

2. ##### 初始化最小堆

```c
FUNCTION createMinHeap(capacity)
    CREATE heap of type MinHeap
    SET heap.size = 0
    SET heap.capacity = capacity
    ALLOCATE heap.nodes of size (capacity + 1)

    FOR i FROM 0 TO capacity
        SET heap.nodes[i].dist = INT_MAX
        SET heap.nodes[i].vertex = i
        IF i < capacity THEN
            SET heap.position[i] = -1
        END IF
    END FOR

    RETURN heap
END FUNCTION

```

> 1. 由于采用了`1-based`的索引，因此初始化时申请`capacity+1`个节点空间；
> 2. 初始化每个节点的`dist`为无穷大，`position`为`-1`表示还没有插入到堆中；

3. ##### 插入最小堆

```c
FUNCTION insertMinHeap(heap, vertex, distance)
    IF heap.size >= heap.capacity THEN
        PRINT "Heap is full"
        RETURN
    END IF

    INCREMENT heap.size
    SET position = heap.size
    SET heap.nodes[position].vertex = vertex
    SET heap.nodes[position].dist = distance
    SET heap.position[vertex] = position

    WHILE position > 1 AND heap.nodes[position].dist < heap.nodes[position / 2].dist DO
        SWAP heap.nodes[position] WITH heap.nodes[position / 2]
        UPDATE heap.position for swapped vertices
        SET position = position / 2
    END WHILE
END FUNCTION

```

> 采用经典的最小堆插入方式，简要叙述：将新的节点初始置于堆数组的最右端，然后利用`position/2`找到其父节点，并根据大小比较决定是否交换，同时更新堆类型的`position`

4. ##### 提取最小元素（较长，用源码辅助注释来展示）

```c
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
```

> 1. 假定最后一个节点代替根节点的位置；
> 2. 与`child`子节点比较，如果大于较小的那一个，将其交换；
> 3. 将最后的位置`position`同步更新



5. 修改堆中的节点距离

```c
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
```

> 1. 对于已经在堆中需要更新`dist`属性的节点，适用上述的`decreaseKey`函数
> 2. 由于`decreaseKey`处理时，需要预先知道对应节点在堆中的位置，因此通过`position`属性的维持，可以在O(1)的时间内定位需要处理的节点；
> 3. 进而更新对应节点的`dist`，然后不断上浮维持堆属性



# Chapter 3: **Testing Results**

## 3.1 正确性测试

- 测试目的：利用小规模的数据、边界数据验证基本功能的正确性

### 3.2 性能型测试

- 测试目的：利用`USA road networks`的数据集测试，以比较不同堆的性能

- ##### `run time table` 

|      数据集       |  最小堆   | 斐波那契堆 |
| :---------------: | :-------: | :--------: |
| USA-road-d.BAY.gr | 82.906 s  |            |
| USA-road-d.CAL.gr | 522.583 s |            |
| USA-road-d.COL.gr | 123.827 s |            |
| USA-road-d.FLA.gr | 191.028 s |            |
| USA-road-d.LKS.gr | 521.628 s |            |
| USA-road-d.NE.gr  | 312.300 s |            |
| USA-road-d.NW.gr  | 234.449 s |            |
| USA-road-d.NY.gr  | 68.533 s  |            |

- ##### `Plot`



# Chapter 4: **Analysis and Comments**

# **Appendix: Source Code** 

# Others

### References

### Author List

### Declaration

### Signatures

![ ](C:\Users\86150\AppData\Roaming\Typora\typora-user-images\image-20241020134537711.png)
