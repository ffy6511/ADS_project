#include "../include/graph.h"

#include <iostream>
#include <time.h>
#include <array>
#include <string>
#include<vector>

/*********************************************************************************************************************  
 * 这里是main函数
 * if you cannot see simplified Chinese word, please set your environment according to readme!
 * 主要采用面向对象，只展示接口，所以main函数是阅读友好的，建议从这里开始阅读
 * 
 * 下面有两个超参数可以自己设置
 * threads表示读取文件时的线程，一个好的线程数目可以提高效率，经过测试5~6效率较高
 * query_number表示在进行随机查询时查询的数据对数，题目要求1000对，如果你对自己的电脑有自信（参考readme的配置），可以试着跑1000对
 * 但是就测试而言，为了避免不必要的蓝屏，发热，爆炸等情况，建议使用50对以下
 *
 * 在main函数中的测试函数设置：
 * graph.test(query_number, filename, heaptype, Vnum, israndom, start, end);
 * 这个方法有两种调用模式：
 * 1.随机生成数据开始循环跑：我们需要设置query_number表示循环次数，filename表示文件名称，heaptype表示堆结构名称，Vnum表示顶点个数，israndom设置为1
 * 2.手动输入数据检测：我们需要设置filename表示文件名称，heaptype表示堆结构名称，Vnum表示顶点个数，israndom设置为0，a，b表示起点和终点
 * 上面没说的参数就是随便设置，建议设置为空的基本类型。
 * 返回值是一个double，你也可以对它操作（我们没有操作因为在函数内部默认显示了）
 * heaptype支持三个：fib left int 分别表示斐波那契堆，左式堆和普通最小堆
 **********************************************************************************************************************/

int threads = 5;
int query_number = 10;


int main() {
    try { //尝试创建图（从文件中加载数据）
        std::array<std::string, 13> all_files = {
        "test.gr",
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
        "USA-road-d.W.gr",
        "USA-road-d.CTR.gr",
        };//所有的文件名

        std::string filename;//具体创建图时的文件名，在循环时从上面的数组中替换

        for (int i = 0; i < 13; i++) {
            filename = all_files[i];//替换

            std::cout << "正在读取文件" << filename << "..." << std::endl;//IO操作

            clock_t start_time = clock();
            Graph graph("data/"+filename, threads);//这里实例化一个图，传入文件名，这个类将自动创建出一个图，以后的操作都在类的实例化对象上进行
            clock_t end_time = clock();
            //这是文件读取计时
            std::cout << "文件" << filename << "读取成功" << "，用时" << end_time - start_time << std::endl;

            //显示图的邻接列表
            //graph.displayGraph();
            //很大的图就别显示了，你可以手动设置只读取test.gr然后查看它的效果

            //测试，关于这个测试函数怎么用，请见文件开头
            //下面是我们给出的一些测试例子，你可以有选择地取消他们的注释来试一试
            //这三组是随机测试循环1000轮
            double time_fib = graph.test(query_number, "data/" + filename, "fib", graph.getNumVertices(), 1, 0, 0);
            //double time_left = graph.test(query_number, "data/" + filename, "left", graph.getNumVertices(), 1, 0, 0);
            //double time_min = graph.test(query_number, "data/" + filename, "min", graph.getNumVertices(), 1, 0, 0);

            //这三组是利用test.gr显示最短路径和长度
            //double t1 = graph.test(1, "data/" + "test.gr", "fib", graph.getNumVertices(), 0, 1, 0);
            //double t2 = graph.test(1, "data/" + "test.gr", "left", graph.getNumVertices(), 0, 1, 0);
            //double t3 = graph.test(1, "data/" + "test.gr", "min", graph.getNumVertices(), 0, 1, 0);

        }
    }
    catch (const std::exception& e) {
        // 捕获并处理异常，输出错误信息 在文件内部会扔出各种错误并在此处捕获
        std::cerr << "错误 " << e.what() << std::endl;//我没用英语因为注释和信息用中文写能看得更明白一点
        return 1;  // 返回错误代码
    }

    return 0;  // 程序正常结束
}
