#include "graph.h"
#include "fibheap.h"
#include <iostream>
#include <time.h>
#include <array>
#include <string>
#include<vector>

# define DEMO

//TODO：整理代码，分成src和include和input三个文件夹

int main() {
    // 创建图并从文件中加载
    try {

        std::array<std::string, 11> all_files = {
        //"test.gr",
        "USA-road-d.BAY.gr",
        "USA-road-d.CAL.gr",
        "USA-road-d.COL.gr",
        "USA-road-d.E.gr",
        "USA-road-d.FLA.gr",
        "USA-road-d.LKS.gr",
        "USA-road-d.NE.gr",
        "USA-road-d.NW.gr",
        "USA-road-d.NY.gr",
        "USA-road-d.USA.gr",
        "USA-road-d.W.gr"
        };

        // 尝试从文件中创建图

        std::string filename;
        int threads = 6;
        //TODO:尝试一下几个线程是最快读取文件的


        for (int i = 0; i < 9; i++) {
            int query_number = 20;
            filename = all_files[i];

            std::cout << "正在读取文件" << filename << "..." << std::endl;
            clock_t start_time = clock();

            Graph graph(filename, threads);

            clock_t end_time = clock();
            std::cout << "文件" << filename << "读取成功" << "，用时" << end_time - start_time << std::endl;

            // 显示图的邻接列表
            //graph.displayGraph();
            //很大的图就别显示了

            //到这里图已经构建完了

            double time_fib = graph.test(query_number, filename, "fib", graph.getNumVertices());
            double time_left = graph.test(query_number, filename, "left", graph.getNumVertices());
            double time_bin = graph.test(query_number, filename, "min", graph.getNumVertices());

        }
    }
    //GPT：这里的报错系统也是GPT给加的
    catch (const std::exception& e) {
        // 捕获并处理异常，输出错误信息
        std::cerr << "error " << e.what() << std::endl;
        return 1;  // 返回错误代码
    }

    return 0;  // 程序正常结束
}
