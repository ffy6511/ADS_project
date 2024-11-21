# 题干
There are N items on your shopping list, and you have N kinds of coupon that can save you some money on buying any of these items.  At the mean time, you have D dollars in your pocket.  How can you buy as many items as you can?  Here you may use any coupon many times and buy any item many times, but only one coupon may be used to get a reduction of payment for one item once -- that is, for example, you may use coupon A1 to buy item B1, and then use coupon A2 to buy item B1.  At the mean time, coupon A1 can be used to buy item B2.  But you may not use coupon A1 to buy item B1 AGAIN.
For example, suppose there are 4 items with prices of  10,  12,  15 and  20; and 4 kinds of coupons with values of  6,  7,  8 and  9.  If you have  30 in your pocket, the best way to buy is the following:

buy 10 item for 4 times, with each coupon once, and hence pay  10x4 -  6 -  7 -  8 -  9 =  10 in total;
buy 12 item for 3 times, with coupons of  7,  8 and  9, and hence pay  12x3 -  7 -  8 -  9 =  12 in total;
buy 15 item with  9 coupon, and hence pay  6;
Now you have  2 left, not enough to buy any more items. Therefore the maximum number of items you can buy is 8.

## 输入与输出
---

# 分析
1. 思路:
   1. 将读入的价格和优惠券按照升序排列；
   2. 采用暴力枚举或者优化贪心的算法计算

## 暴力枚举
用O($N^2$)的时间计算实际开支`actual_cost`数组，并排序。然后依次取用。

## 优化算法
在price和coupon的基础上开设一个结构体数组curr_least,结构体是index和cost,第i个元素表示第i个物品当前情况下购买时最低的开支，初始化显然为每个物件的价格-最大的优惠券。然后将curr_least根据cost属性建立小顶堆，每次用logN的时间取用最小元素并调整（包括自增index并更新cost然后调整小顶堆）。
```c
typedef struct {
    int index;    // 商品索引
    int cost;     // 当前最低花费
    int coup_idx; // 当前使用的优惠券索引
} Item;
```
