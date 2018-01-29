#include <iostream>
#include<cassert>   //ʹ��assert()
#include<ctime>     //ʹ��clock()
#include<vector>

//#include "MemoryPool.h"     //MemoryPool<T>,�ڴ��ͷ�ļ�
#include "StackAlloc.h"     //stackAlloc<T, Alloc>������ջͷ�ļ�
//StackAlloc��һ������ջ����������ģ���������һ������������ջ�е�Ԫ�����ͣ��ڶ�������Ϊ�ڴ������

using namespace std;
//ѭ������
#define REPS 100
//����Ԫ�ظ���
#define ELEMS 10000000

int main()
{
    clock_t start;

    //ʹ��STLĬ�Ϸ�����
    //allocator��C++��׼�����ṩ��Ĭ�Ϸ������������ص�����������ʹ��new�������ڴ湹���¶���ʹ����Ҫ����Ĭ�Ϲ��캯��
    //��ʹ��allocator����Խ��ڴ����Ͷ���Ĺ������������߼��ֿ���ʹ�÷�����ڴ���ԭʼ�ģ�δ����ġ�
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
    cout << (((double)clock() - start) / CLOCKS_PER_SEC) << endl << endl;   //CLOCKS_PER_SEC��һ����������ʾһ����
    //CPU���е�ʱ�������������ڽ�clock�����Ľ��ת��Ϊ����Ϊ��λ����


    /*
    //ʹ���ڴ��
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
