#include "graph.h"
#include "fibheap.h"
#include <iostream>
#include <time.h>

# define DEMO

//TODO：整理代码，分成src和include和input三个文件夹

int main() {
    // 创建图并从文件中加载
    try {
        // 尝试从文件中创建图

        std::string filename = "USA-road-d.USA.gr";
        int threads = 6;
        //TODO:尝试一下几个线程是最快读取文件的
        //GPT:多进程内核是GPT写的

        std::cout << "正在读取文件" << filename <<"..." << std::endl;
        clock_t start_time = clock();

        Graph graph(filename, threads);

        clock_t end_time = clock();
        std::cout << "文件"<<filename<<"读取成功"<< "|用时"<< end_time - start_time<< std::endl;

        // 显示图的邻接列表
        //graph.displayGraph();
        //很大的图就别显示了

        //到这里图已经构建完了

        
        int start_V = 0;
        int end_V = 1;
        
        start_time = clock();

        auto result = graph.dijkstra(start_V, end_V);

        end_time = clock();

        std::vector<int> path = result.first;
        int distance = result.second;

        std::cout << "最短路径start：" << start_V << " 最短路径end：" << end_V << ":\n";
        for (int vertex : path) {
            std::cout << vertex << " ";
        }
        std::cout << "\n距离： " << distance << std::endl;
        std::cout << "用时" << end_time - start_time << std::endl;
    }

    //GPT：这里的报错系统也是GPT给加的

    catch (const std::exception& e) {
        // 捕获并处理异常，输出错误信息
        std::cerr << "出现异常行为 " << e.what() << std::endl;
        return 1;  // 返回错误代码
    }

    return 0;  // 程序正常结束
}
