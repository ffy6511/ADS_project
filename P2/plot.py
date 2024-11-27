import matplotlib.pyplot as plt  
  
# 数据（从小到大排序）  
files = ["NY", "BAY", "COL", "FLA", "NW", "NE", "CAL", "LKS", "E", "W", "CTR", "USA"]  
min_times = [114.33, 106.73, 133.06, 357.88, 412.33, 443.21, 1043.43, 989.32, 1132.47, 1394.33, 6152.9, 74080]  
left_times = [106.57, 171.61, 167.76, 458.6, 624.9, 588.43, 1023.21, 1005.2, 1234.7, 3804.37, 8763.20, 78282.5]  
fib_times = [70.37, 93.29, 119.1, 339.17, 400.7, 487.47, 1166.07, 1034.1, 1309.6, 2563.87, 5912.8, 10975]  
  
# 绘制折线图  
plt.figure(figsize=(12, 6))  
plt.plot(files, min_times, marker='o', label='Min Heap')  
plt.plot(files, left_times, marker='o', label='Leftist Heap')  
plt.plot(files, fib_times, marker='o', label='Fibonacci Heap')  
  
# 添加标题和标签  
plt.title('Algorithm Performance on Different Files with Different Heaps')  
plt.xlabel('File Name')  
plt.ylabel('Time (s)')  
plt.yscale('log')  # 使用对数尺度以便更好地展示差异  
  
# 显示图例  
plt.legend()  
  
# 显示图形  
plt.grid(True)  
plt.show()  