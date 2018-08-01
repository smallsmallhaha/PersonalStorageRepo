# OpenMP指南
## 什么是OpenMP ?
OpenMP（Open Multi-Processing）是一套支持跨平台共享内存方式的多线程并发的编程API，使用C,C++和Fortran语言，可以在大多数的处理器体系和操作系统中运行，包括Solaris, AIX, HP-UX, GNU/Linux, Mac OS X, 和Microsoft Windows。包括一套编译器指令、库和一些能够影响运行行为的环境变量。

## 开启OpenMP支持
### Visual C++
1. 给编译器加上 -openmp 选项
2. 打开"Property -> Configuration Properties -> C/C++ -> Language ->  OpenMP Support"，开启openmp支持。

### gcc
1. 命令行使用  
加上"-fopenmp"选项
2. Qt使用  
在.pro文件中添加下面两行
> QMAKE_CXXFLAGS += -fopenmp   
> QMAKE_LFLAGS += -fopenmp

### clang
1. 安装支持库  
sudo apt install libomp-dev
2. 命令行使用  
加上"-fopenmp"选项
3. Qt使用  
在.pro文件中添加下面两行
> QMAKE_CXXFLAGS += -fopenmp   
> QMAKE_LFLAGS += -fopenmp

### 说明
Q: 为什么clang要安装支持库而gcc不需要？  
A: gcc自带对openmp的支持依赖库为gomp，clang需要第三方库omp。  
Q: 为什么Qt中带了两个"-fopenmp"选项？  
A: 不严格地来说，C/C++程序编译可以分为编译和链接两步，两步都要加"-fopenmp"选项才能开启OpenMP支持，Qt中的QMAKE_CXXFLAGS代表编译器参数，QMAKE_LFLAGS代表链接器参数。


## 使用OpenMP
### OpenMP指令
```cpp
// parallel
// 并行执行代码块
#pragma omp parallel [clause[ [, ]clause] ...]
structured-block

// for
// 并行执行for循环
#pragma omp for [clause[ [, ]clause] ...]
for-loops

// sections
// 并行执行各节代码块
#pragma omp sections [clause[ [, ] clause] ...]
{
[#pragma omp section]
structured-block
[#pragma omp section
structured-block] ...
}

// parallel for
// 只含有for循环的parallel结构的缩写
#pragma omp parallel for [clause[ [, ]clause] ...]
for-loop

// parallel sections
#pragma omp parallel sections [clause[ [, ]clause] ...]
{
[#pragma omp section]
 structured-block
[#pragma omp section
 structured-block]
...
}

// master
// 主线程代码块
#pragma omp master
structured-block

// critical
// 临界区
#pragma omp critical [(name) [hint (hint-expression)]]
structured-block

// barrier
// "障碍区"，必须等待所有线程都运行完才能进行下一步
#pragma omp barrier

// 
```
### OpenMP子句
```cpp
// 共享数据区
shared(list)

// 私有数据，代码块内是变量的副本
private(list)

// 规约，规约符号可以是 + - * | ^ & || && max min
reduction(reduction-identifier:list)
```

## 参考
https://www.openmp.org/wp-content/uploads/OpenMP-4.5-1115-CPP-web.pdf  
https://www.openmp.org/wp-content/uploads/openmp-examples-4.5.0.pdf
