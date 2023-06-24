# 多道程序缓冲区——生产者消费者

[![Language](https://img.shields.io/badge/language-c++-brightgreen.svg)](C++) 

## 说明

OS三级项目

### 开发环境

- 编译环境：MinGW 7.3.0

- 数据库：SQLite 3.41.2

- QT框架：Qt 5.14.2

### 基本功能

1. 随机产生字符数据

2. 可以设定各Buffer的容量、PUT、GET、MOVE操作的个数；

3. 可以设定PUT、GET、MOVE操作的速度；

4. 提供良好图形界面，可以暂停和继续系统的执行，实时显示每个Buffer中数据的个数和数据的内容，当前每个Buffer中空闲的空间的个数；

5. 实时显示线程、进程所处于等待（阻塞）状态的个数；

6. 程序运行结束，显示汇总数据：

   1. 总的运行时间；

   2. 当前Buffer中数据的个数；

   3. 已放入Buffer的数据个数；

   4. 已取出Buffer的数据个数；

   5. 平均每个Buffer中的数据个数。

7. 能够将每次的实验输入和实验结果存储起来，下次运行时或以后可查询。

### 使用说明

主界面的使用基本上按对应的按钮就可以了，但是每个Buffer进度球也是可以点击的，点击将会弹出该Buffer数据量的实时监控窗口。

数据分析界面右上角有使用说明。

主界面和数据分析界面均有可拖动的组件，可以自行尝试。

## 设计亮点

1. 友好的界面
2. 实时图表+数据统计
3. 可以开启数据回放(实验性)
4. 支持数据导出
5. 支持功能拓展可插拔

## 展示

![loading](/pic/loading.png)

![mainwindow](/pic/mainwindow.png)

![config](/pic/config.png)

![result](/pic/result.png)

![running](/pic/running.png)

![data_distribution](/pic/data_distribution.png)

![thread_trend](/pic/thread_trend.png)

![control](/pic/control.png)

![data_supervise](/pic/data_supervise.png)

![save_json](/pic/save_json.png)

![data_json](/pic/data_json.png)