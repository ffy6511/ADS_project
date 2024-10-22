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
        double time_costs[9]; //各个文件的测试时间
        std::array<std::string, 9> all_files = {
        "USA-road-d.BAY.gr",
        "USA-road-d.CAL.gr",
        "USA-road-d.COL.gr",
        "USA-road-d.FLA.gr",
        "USA-road-d.LKS.gr",
        "USA-road-d.NE.gr",
        "USA-road-d.NW.gr",
        "USA-road-d.NY.gr",
        "USA-road-d.USA.gr"
    };
    
    // 尝试从文件中创建图

    std::string filename;
    int threads = 6;
    //TODO:尝试一下几个线程是最快读取文件的

    for(int i = 0; i < 9; i++){
        int query_number = 1000;
        filename = all_files[i];
        
        std::cout << "正在读取文件" << filename <<"..." << std::endl;
        clock_t start_time = clock();

        Graph graph(filename, threads);

        clock_t end_time = clock();
        std::cout << "文件"<<filename<<"读取成功"<< "|用时"<< end_time - start_time<< std::endl;

        // 显示图的邻接列表
        //graph.displayGraph();
        //很大的图就别显示了

        //到这里图已经构建完了
        srand(time(NULL)); // 设置随机种子

        start_time = clock();   //开始计时

        for(int i=0; i< query_number; i++){

            int src = rand() % graph.getNumVertices();  //根据图的顶点数选取合适的src
            int dest = rand() % graph.getNumVertices(); //同理生成随机的dest
            auto result = graph.dijkstra(src, dest); //调用dijkstra算法
        }
        end_time = clock();     //结束计时
        
        double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC; //转换为秒
        time_costs[i] = time_taken; //记录时间

        
        std::cout << "斐波那契堆下算法对" << filename << query_number << "次运行时间(秒):" << time_taken << std::endl;
        
    }

    //输出各组的运行时间
    std::cout << "各组运行时间：" << std::endl;
    for(int i = 0; i < 9; i++){
        std::cout << all_files[i] << " " << time_costs[i] << std::endl;
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
