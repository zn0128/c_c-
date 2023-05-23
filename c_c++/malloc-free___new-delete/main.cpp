// https://www.cnblogs.com/liushui-sky/p/5776638.html

// 名词解释
// 1、链接表：Linux内核在多个区域各自维护了一个空闲内存块的链表，用来保存当前可用的内存块
// 2、.h头文件：可供阅读的源文件，在编译过程中使用，存放了函数的声明，与一个.lib库对应
// 3、.lib：静态库情况下，不存在dll只有lib，其中既有函数接口也有实现，声明和实现的二进制代码都被嵌入可执行文件。动态库情况下，既有dll也有lib，链接lib后用作dll依赖项，用来告诉当前执行的函数对应的实现在dll中的什么位置
// 4、.dll：动态库情况下的函数实现二进制代码，通过lib中函数声明来链接获取
// 5、运算符：如‘+’、‘-’、“new”、“delete”等，是 C++中的特定符号，C++编译器在编译过程会根据给定运算符执行指定的编译操作

// malloc:
// 1、调用malloc函数时，会沿着链接表找到一个大到足以满足用户申请所需内存的内存块，并将可用内存块链接为一个空闲链表
// 2、之后将这块内存一分为二，一块大小与用户请求的大小相等，另一块为多余的内存块（实际上划分的堆区内存块会比申请的更大些，原因时需要额外的空间记录管理信息（如块长度），以及指向下一个块的指针等等）
// 3、接下来将前者的内存快传给用户，剩下的内存块返回链接表
// 4、如果用户申请了一个较大的内存块，且当前的链接表没有满足用户的内存块，这个时候就会将链接表上相邻的小空闲内存块合并为较大的内存块，以满足内存的申请

// free:
// 调用free函数时，会将用户释放的内存块链接到链接表上

// new:
// 1、通过operator new从堆上申请内存（底层调用的是malloc）
// 2、调用构造函数（如果操作对象是一个类对象）

// delete:
// 1、调用析构函数（如果操作对象是一个类对象）
// 2、通过operator delete释放内存（底层调用的是free）


// 相同点
// 用于动态申请堆区内存和释放内存

// 不同点
// 1、malloc和free是标准库函数；new和delete是运算符
// 2、malloc只分配内存；不会调用对象的构造函数，而new会
// 3、malloc失败时返回NULL；而new分配内存失败时会抛出bad_alloc异常
// 4、malloc返回void类型指针，需要进行类型转换后才能使用；而new返回的的对象指针
// 5、malloc需要给出具体的内存大小；而new会根据类自行计算所需内存大小
// 6、运算符new封装了malloc，直接free不会报错，单只会释放内存不会析构对象


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
class Father {
    public:
    Father() {
        cout << "这里是Father的构造函数" << endl;
    }

    ~Father(){
        cout << "这里是Father的析构函数" << endl;
    }

    private:
};


int main () {
    Father* f1 = new Father();
    delete(f1);                             // 如果不调用delete，就不会调用析构函数，造成内存泄漏

    int length = 5;
    int *p = (int *)malloc(sizeof(int) * length);       // 通过malloc分配五个int变量大小的内存，并用整型指针p指向它
    p[1] = 1;
    p[3] = 3;
    for (int i = 0; i < length; i++) {
        cout << "p[" << i << "] = " << p[i] << endl;        // 没有赋值的位置默认为0
    }
    free(p);

    Father* f2 = new Father();
    free(f2);                           // 这里用free来释放f2指向的内存，释放内存操作是正确的但不会触发析构函数（由于内部数据类型没有构造与析构的过程，对它们而言 malloc/free和new/delete是等价的）

    return 0;
}