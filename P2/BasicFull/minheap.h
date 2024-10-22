#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
class MinHeap {
private:
    std::vector<T> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (heap[index].second < heap[parentIndex].second) {
                std::swap(heap[index], heap[parentIndex]);
                index = parentIndex;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int leftChild, rightChild, smallest;

        while (true) {
            leftChild = 2 * index + 1;
            rightChild = 2 * index + 2;
            smallest = index;

            if (leftChild < heap.size() && heap[leftChild].second < heap[smallest].second) {
                smallest = leftChild;
            }

            if (rightChild < heap.size() && heap[rightChild].second < heap[smallest].second) {
                smallest = rightChild;
            }

            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else {
                break;
            }
        }
    }

public:
    MinHeap() {}

    ~MinHeap() {}

    // 插入一个新的元素，假设元素是 std::pair<int, int>
    void insert(T value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    // 删除最小元素并返回顶点
    std::pair<int, int> deleteMin() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }

        std::pair<int, int> minValue = heap[0]; // 返回堆顶元素
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return minValue; // 返回具有最小键值的顶点
    }

    // 获取最小元素的顶点
    std::pair<int, int> getMin() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0]; // 返回具有最小键值的顶点
    }

    // 检查堆是否为空
    bool isEmpty() const {
        return heap.empty();
    }

    // 显示堆中的元素
    void display() const {
        for (const auto& element : heap) {
            std::cout << "(" << element.first << ", " << element.second << ") ";
        }
        std::cout << std::endl;
    }
};

#endif // MIN_HEAP_H
