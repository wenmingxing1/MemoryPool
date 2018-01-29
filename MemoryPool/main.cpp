#include <iostream>
#include<cassert>   //使用assert()
#include<ctime>     //使用clock()
#include<vector>

//#include "MemoryPool.h"     //MemoryPool<T>,内存池头文件
#include "StackAlloc.h"     //stackAlloc<T, Alloc>，链表栈头文件
//StackAlloc是一个链表栈，接受两个模板参数，第一个参数类型是栈中的元素类型，第二个参数为内存分配器

using namespace std;
//循环次数
#define REPS 100
//插入元素个数
#define ELEMS 10000000

int main()
{
    clock_t start;

    //使用STL默认分配器
    //allocator是C++标准库中提供的默认分配器，他的特点在于我们在使用new来申请内存构造新对象使，需要调用默认构造函数
    //而使用allocator则可以将内存分配和对象的构造这两部分逻辑分开，使得分配的内存是原始的，未构造的。
    StackAlloc<int, allocator<int>> stackDefault;
    start = clock();

    for (int j = 0; j < REPS; ++j) {
        assert(stackDefault.empty());
        for (int i = 0; i < ELEMS; ++i)
            stackDefault.push(i);
        for (int i = 0; i < ELEMS; ++i)
            stackDefault.pop();
    }

    cout << "Default Allocator Time : " << endl;
    cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl << endl;   //CLOCKS_PER_SEC是一个常数，表示一秒钟
    //CPU运行的时钟周期数，用于将clock函数的结果转化为以秒为单位的量


    /*
    //使用内存池
    StackAlloc<int, MemoryPool<int>> stackPool;
    start = clock();
    for (int j = 0; j < REPS; ++j) {
        assert(stackPool.empty());
        for (int i = 0; i < ELEMS; ++i)
            stackPool.push(i);
        for (int i = 0; i < ELEMS; ++i)
            stackPool.pop();
    }

    cout << "MemoryPool Allocator Time : " << endl;
    cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl << endl;
    */


    return 0;
}
