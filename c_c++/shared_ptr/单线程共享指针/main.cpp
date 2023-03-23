#include <iostream>
#include <string>
#include <memory>           // 智能指针头文件
using namespace std;

#define ADDR_GET(smart_ptr) (cout << "up1 的地址 = " << smart_ptr.get() << endl)        // 和宏定义有关的知识整理在同目录下文件

void fun() {
    int *a = new int();

    if (true) {
        // ......
        return;
    }

    delete(a);      // 没有释放堆内存就return了
}

// C++ 11之前的智能指针是 auto_ptr，其缺点有三个
// 1、复制和赋值会改变资源的所有权
// 2、在STL容器内会存在重大风险，因为容器内的元素必须支持复制和赋值
// 3、不支持对象数组的操作
// unique_ptr会对以上的三个问题进行优化
void fun_unique_ptr() {
    new int(1);
    unique_ptr<string> up1(new string("Hello world"));
    unique_ptr<string> up2(new string("Hi world "));
    ADDR_GET(up1);
    ADDR_GET(up2);


}


void fun_shared_ptr() {
    shared_ptr<int> sp(new int());      // 在堆区申请一个int数据类型的大小，并将其在栈区的指针托管到共享指针
    
    cout << "引用计数 = " << sp.use_count() << endl;
}


void fun_weak_ptr() {
    
}

int main() {
    fun();         // 造成内存泄漏

    fun_unique_ptr();         // unique_ptr用法

    fun_shared_ptr();         // shared_ptr用法

    fun_weak_ptr();         // weak_ptr用法

    return 0;
}