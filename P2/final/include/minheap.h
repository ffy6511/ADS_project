#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>
#include <unordered_map>

/************************************************************************************************************
 * 这是普通最小堆的内部实现
 * 在本次，我们编写的数据结构都是模板类 未实例化时不指定类型名称 
 * 因此无法按照传统的 .h 放名称 .cpp 放主体的方式实现
 * 为了结构完整性加了cpp文件，但它们全是空的
 * 所以在实现上会有注释“用**结构作为堆元素”
 * 
 * 包含操作增删查减小键值等
 * 
 ***********************************************************************************************************/

template <typename T>
class MinHeap {
private:
    std::vector<T> heap; // 用于存储堆的元素，假设每个元素是一个 std::pair<int, int>，其中第一个 int 是顶点，第二个 int 是键值
    std::unordered_map<int, int> pos; // 用来追踪每个顶点在堆中的位置，key 是顶点，value 是该顶点在 heap 数组中的索引

    // 向上调整堆，以保持最小堆的性质
    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2; // 计算父节点的索引
            // 如果当前节点的键值小于父节点的键值，则交换
            if (heap[index].second < heap[parentIndex].second) {
                std::swap(heap[index], heap[parentIndex]);
                // 更新 pos 数组中对应顶点的索引
                pos[heap[index].first] = index;
                pos[heap[parentIndex].first] = parentIndex;
                // 移动到父节点继续检查
                index = parentIndex;
            }
            else {
                break; // 如果当前节点的键值不小于父节点，则堆已调整完毕
            }
        }
    }

    // 向下调整堆，以保持最小堆的性质
    void heapifyDown(int index) {
        int leftChild, rightChild, smallest;

        while (true) {
            leftChild = 2 * index + 1; // 计算左子节点的索引
            rightChild = 2 * index + 2; // 计算右子节点的索引
            smallest = index; // 假设当前节点是最小的

            // 如果左子节点存在且左子节点的键值小于当前节点，更新最小节点
            if (leftChild < heap.size() && heap[leftChild].second < heap[smallest].second) {
                smallest = leftChild;
            }

            // 如果右子节点存在且右子节点的键值小于当前最小节点，更新最小节点
            if (rightChild < heap.size() && heap[rightChild].second < heap[smallest].second) {
                smallest = rightChild;
            }

            // 如果最小节点不是当前节点，进行交换并继续向下调整
            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                // 更新 pos 数组中对应顶点的索引
                pos[heap[index].first] = index;
                pos[heap[smallest].first] = smallest;
                index = smallest; // 继续向下调整
            }
            else {
                break; // 如果最小节点是当前节点，堆已调整完毕
            }
        }
    }

public:
    // 构造函数，初始化空的堆
    MinHeap() {}

    // 析构函数，这里没有需要特殊处理的资源，默认析构函数就可以了
    ~MinHeap() {}

    // 插入一个新的元素，假设元素是 std::pair<int, int>，第一个值是顶点，第二个是键值
    void insert(T value) {
        heap.push_back(value); // 将新元素添加到堆的末尾
        int index = heap.size() - 1; // 获取新元素的索引
        pos[value.first] = index; // 记录该顶点在堆中的位置
        heapifyUp(index); // 向上调整堆，以保持最小堆的性质
    }

    // 删除并返回堆中的最小元素（堆顶元素）
    std::pair<int, int> deleteMin() {
        if (heap.empty()) { // 如果堆为空，抛出异常
            throw std::runtime_error("Heap is empty");
        }

        std::pair<int, int> minValue = heap[0]; // 保存堆顶元素
        heap[0] = heap.back(); // 将最后一个元素移到堆顶
        pos[heap[0].first] = 0; // 更新该顶点在堆中的位置
        heap.pop_back(); // 移除最后一个元素
        pos.erase(minValue.first); // 从 pos 中移除已删除的最小顶点
        if (!heap.empty()) {
            heapifyDown(0); // 向下调整堆，以保持最小堆的性质
        }
        return minValue; // 返回具有最小键值的顶点和键值
    }

    // 减少某个顶点的键值
    void decreaseKey(int vertex, int newDist) {
        if (pos.find(vertex) == pos.end()) {
            // 如果顶点不在堆中，则直接插入
            insert({ vertex, newDist });
        }
        else {
            int index = pos[vertex]; // 获取顶点在堆中的索引
            if (newDist < heap[index].second) { // 只有当新键值更小时才更新
                heap[index].second = newDist; // 更新顶点的键值
                heapifyUp(index); // 向上调整堆
            }
        }
    }

    // 检查堆是否为空
    bool isEmpty() const {
        return heap.empty(); // 如果堆为空，返回 true
    }

    // 检查某个顶点是否在堆中
    bool hasVertex(int vertex) const {
        return pos.find(vertex) != pos.end(); // 如果顶点在 pos 中，返回 true
    }

    // 显示堆中的所有元素
    void display() const {
        for (const auto& element : heap) { // 遍历堆数组中的所有元素
            std::cout << "(" << element.first << ", " << element.second << ") "; // 打印顶点和键值
        }
        std::cout << std::endl;
    }
};


#endif // MIN_HEAP_H
