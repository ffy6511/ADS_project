## 这是浙江大学ADS Project2 的代码。请先阅读本文件再进行代码的阅读和测试

### 文件系统
- 代码总共包含三个文件夹(注意文件夹要正确命名，否则可能会影响代码运行)
  - src文件夹 存放源代码的文件夹
    - main.cpp 主程序
    - graph.cpp 图类
    - fibheap.cpp 斐波那契堆类
    - leftistheap.cpp 左式堆类
    - minheap.cpp 普通最小堆类
  - include文件夹 存放头文件的文件夹(由于我们的数据结构实现为模板类，所有的函数必须要在头文件里，所以这里的某些源文件仅为了程序完整性)
    - graph.h 图函数
    - fibheap.h 
    - leftistheap.h
    - minheap.h
  - data文件夹 存放测试数据的文件夹  
    - test.gr 这个是自己编写的测试文件
    - 其他的均为从网站 http://www.diag.uniroma1.it/challenge9/download.shtml 下载
- 如果data文件丢失(文件夹里少于13个文件)可能是因为PTA提交大小限制，可以自行下载

### 运行环境
- 如果出现错误请优先检查你的环境配置
- 环境1
  - 操作系统：Ubuntu 22.04.5 LTS (GNU/Linux 5.15.153.1-microsoft-standard-WSL2 x86_64)
  - 编译器：g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
  - 运行方法(依次键入以下命令)：
    - g++ -I include src/main.cpp src/graph.cpp src/fibheap.cpp src/leftistheap.cpp src/minheap.cpp -o program
    - ./program
- 环境2
  - 操作系统 Windows 11
  - 集成环境：Visual Studio 2022
  - 运行方法
    - 直接运行
- 环境3(如果前两个环境都没有或者失败的话)
  - lightly网页版IDE
  - 网站 https://lightly.teamcode.com/
  - 运行方法
    - 直接运行
  
### 运行说明
- 测试环境是VS 2022
  - 处理器 13th Gen Intel(R) Core(TM) i9-13900HX   2.20 GHz
  - 跑1000组数据会出现发烫，时间很长（几小时）的问题
- 测试环境WSL
  - 内存6G
  - 跑不了1000组就崩溃了
- 建议性能对标，为了节省时间设置少一点数据
- 测试环境lightly
  - 轻量化IDE还想跑这么大的数据量？可以但是要交钱，不值得

### 阅读建议：
- 先阅读main.cpp
- 再阅读graph.h
- 再阅读各个数据结构
- 最后可以看一下graph.cpp的实现

### 中文编码问题
- 如果出现中文乱码，检查你的电脑是否开启了
  - UTF-8支持的全球编码
- 如果还是不行，检查你的编辑器的编码格式并修改
