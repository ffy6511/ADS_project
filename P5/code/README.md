1. correctness_check文件夹存放的代码供手动输入检验程序的正确性，输入和输出的格式要求与题干相同，您可以分别编译并运行`Brutal_solution`与`Optimize_solution`。
2. time_test文件夹内，`Brutal_solution`与`Optimize_solution`均设计为文件读取的版本。它们将读取子文件夹`input`内的文本文件作为输入(输入文件的名称含义请见报告说明），您可以直接编译并运行对应的程序（请确保文件层次符合要求）。或者，您也可以在input文件夹中的`Input_Generator`手动输入`N`，同时命名输出文件名，来手动生成输入的数据文本。具体请参见报告的测试部分。

3. 经检验，在`vscode`的`gcc`编译器下能够直接运行。但是对于Brutal_solution版本，请不要尝试过大的输入数据。
4. 阅读建议：请先阅读 `document`部分，然后测试代码的正确性，最后若有兴趣，您也可以在`time_test`当中手动测试并比较优化前后的时间成本。

