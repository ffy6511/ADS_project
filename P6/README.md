在`code`文件夹下：

1. `Partition_heap.h`与`Rectangle.h`分别实现分区堆与矩形BST的函数与结构体，在`.c`文件中被调用，因此需要一起编译运行；
2. `file_input.c`等`.c`文件，如其名称所示，分别是本算法在不同的输入环境下的主函数（`correctness_test.c` 无需输入）；
3. `Generate_input`文件夹下为生成输入文本的函数以及示例输入文本；
4. `compare_algorithm`文件夹内的代码是参考其他算法的实现（用于与本算法进行比较）；



- 请保持本文件夹的结构以便于`file_input.c`读取输入文本。且您可以在`main`内部修改读取的文件名；
- 如果需要自行生成输入文件，在`Generator.c`当中改变宏定义的输入规模与生成文件的文件名即可。