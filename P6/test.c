#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义矩形结构
typedef struct {
    int width;
    int height;
    int used;
} Rectangle;

// 定义 Skyline 的最大宽度
#define MAX_WIDTH 1000

// Skyline 数组表示容器的当前高度
int skyline[MAX_WIDTH];

// 初始化 Skyline
void initializeSkyline(int width) {
    for (int i = 0; i < width; i++) {
        skyline[i] = 0; // 初始高度为 0
    }
}

// 打印 Skyline 状态
void printSkyline(int containerWidth) {
    printf("Current Skyline: ");
    for (int i = 0; i < containerWidth; i++) {
        printf("%d ", skyline[i]);
    }
    printf("\n");
}

// 找到 Skyline 的最低点及其最大宽度
int findMaxWidth(int containerWidth, int *startIndex) {
    int minHeight = skyline[0];
    int minStart = 0;
    int maxWidth = 1;
    int currentStart = 0;


    for (int i = 0; i < containerWidth;) {
        if (skyline[i] == minHeight) {
            int currentWidth = 1;
            while (i < containerWidth && skyline[i] == minHeight) {
                currentWidth++;
                i++;
            }
            if (currentWidth > maxWidth) {
                maxWidth = currentWidth;
                minStart = currentStart;
            }
        } else {
            if (skyline[i] < minHeight) {
                minHeight = skyline[i];
                currentStart = i;   
                minStart = i;
                maxWidth = 1;
            }
            i++;
        }
    }

    *startIndex = minStart;
    return maxWidth;
}

// 二分查找：找到小于等于指定宽度的最大矩形
int binarySearchLargestLessOrEqual(Rectangle *rectangles, int left, int right, int targetWidth) {
    int bestIndex = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (!rectangles[mid].used && rectangles[mid].width <= targetWidth) {
            bestIndex = mid; // 当前矩形符合条件
            left = mid + 1;  // 尝试寻找更大的矩形
        } else {
            right = mid - 1;
        }
    }
    return bestIndex;
}


// 放置矩形并更新 Skyline
void placeRectangle(int startIndex, int rectWidth, int rectHeight) {
    for (int i = startIndex; i < startIndex + rectWidth && i < MAX_WIDTH; i++) {
        skyline[i] += rectHeight;
    }
}

// 计算容器的最终高度
int calculateHeight(int containerWidth) {
    int maxHeight = 0;
    for (int i = 0; i < containerWidth; i++) {
        if (skyline[i] > maxHeight) {
            maxHeight = skyline[i];
        }
    }
    return maxHeight;
}

// 快速排序辅助函数
void swap(Rectangle *a, Rectangle *b) {
    Rectangle temp = *a;
    *a = *b;
    *b = temp;
}

// 快速排序实现：按宽度升序排序
void quickSort(Rectangle *rectangles, int low, int high) {
    if (low < high) {
        // 分区
        int pivot = rectangles[high].width; // 选择最后一个元素作为基准
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (rectangles[j].width <= pivot) {
                i++;
                swap(&rectangles[i], &rectangles[j]);
            }
        }
        swap(&rectangles[i + 1], &rectangles[high]);

        // 递归排序子数组
        int partitionIndex = i + 1;
        quickSort(rectangles, low, partitionIndex - 1);
        quickSort(rectangles, partitionIndex + 1, high);
    }
}

// 主函数：实现矩形装箱
int skylinePacking(Rectangle *rectangles, int n, int containerWidth) {
    // 初始化 Skyline
    initializeSkyline(containerWidth);

    // 使用快速排序对矩形按宽度升序排序
    quickSort(rectangles, 0, n - 1);

    // 打印排序后的矩形集合
    printf("Sorted rectangles by width:\n");
    for (int i = 0; i < n; i++) {
        printf("Width: %d, Height: %d\n", rectangles[i].width, rectangles[i].height);
    }

    int count;
    // 遍历所有矩形
    for (count  = 0; count < n;) {
        // 找到 Skyline 的最低点及其最大宽度
        int startIndex = 0;
        int maxWidth = findMaxWidth(containerWidth, &startIndex);
        printf("\nAttempting to place rectangle #%d \n", count);
        printf("Lowest point at index %d with maxWidth %d\n", startIndex, maxWidth);

        // 找到符合条件的矩形（宽度 <= maxWidth）
        int bestIndex = binarySearchLargestLessOrEqual(rectangles, 0, n - 1, maxWidth);
        if (bestIndex != -1 && rectangles[bestIndex].width != -1) {
            printf("Placing rectangle with width: %d, height: %d at index %d\n", rectangles[bestIndex].width, rectangles[bestIndex].height, startIndex);
            // 放置矩形并更新 Skyline
            placeRectangle(startIndex, rectangles[bestIndex].width, rectangles[bestIndex].height);
             // 标记矩形已被使用
             rectangles[bestIndex].used = 1;

    // 打印更新后的 Skyline
        printSkyline(containerWidth);
        count+=1;
        } else {
        // 修正当前无法放置的 Skyline 区域
        printf("No suitable rectangle found for placement. Adjusting Skyline.\n");
        int endIndex = startIndex + maxWidth;
        int leftHeight = (startIndex > 0) ? skyline[startIndex - 1] : skyline[startIndex];
        int rightHeight = (endIndex < containerWidth) ? skyline[endIndex] : skyline[startIndex];
        int newHeight = (leftHeight < rightHeight) ? leftHeight : rightHeight;

        // 将当前 Skyline 的范围调整为邻近高度的较小值
            for (int i = startIndex; i < endIndex && i < containerWidth; i++) {
                skyline[i] = newHeight;
            }

            // 打印修正后的 Skyline
            printSkyline(containerWidth);
        }
       }
    // 计算最终容器高度
    return calculateHeight(containerWidth);
}

// 测试程序
int main() {
    int containerWidth = 10; // 固定容器宽度
    Rectangle rectangles[] = {
        {4, 3, 0}, {3, 2, 0}, {6, 5, 0}, {5, 3, 0}, {7, 1, 0}
    };
    int n = sizeof(rectangles) / sizeof(rectangles[0]);

    // 执行 Skyline Packing 算法
    int finalHeight = skylinePacking(rectangles, n, containerWidth);

    // 输出结果
    printf("\nFinal container height: %d\n", finalHeight);
    return 0;
}
