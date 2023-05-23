#include <iostream>
#include <string>
#include <memory>           // 智能指针头文件
#include "checkLeaks.cpp"
#include <vector>
using namespace std;

#define ADDR_GET(smart_ptr) (cout << "up1 的地址 = " << smart_ptr.get() << endl)        // 和宏定义有关的知识整理在同目录下文件

class B;
class Weak_B;

class A {
public:
    void set_b(shared_ptr<B> b) {
        _b = b;
    }

    void set_weak_b(shared_ptr<Weak_B> b) {
        _weak_b = b;
    }
private:
    shared_ptr<B> _b;
    shared_ptr<Weak_B> _weak_b;
};

class B {
public:
    void set_a(shared_ptr<A> a) {
        _a = a;
    }
private:
    shared_ptr<A> _a;
};

class Weak_B {
public:
    void set_a(shared_ptr<A> a) {
        _a = a;
    }

    void show_a(){
        shared_ptr<A> temp_a = _a.lock();

        cout << "_a的引用计数 = " << temp_a.use_count() << endl;
        temp_a = NULL;
    }
private:
    weak_ptr<A> _a;
};

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
    unique_ptr<string> up1(new string("Hello World"));
    unique_ptr<string> up2(new string("Hello China"));
    
    // up1 = up2;                                       // 禁止左值赋值
    // unique_ptr<string> up3(up1);                     // 禁止左值构造赋值
    up1 = move(up2);                                    // 允许右值构造，原因是unique_ptr具有排他性，使用右值拷贝构造函数时会将右值指向的内存置空
    unique_ptr<string> up3(move(up1));                  // 允许右值赋值构造

    // 在STL容器中使用unique_ptr时，不允许直接赋值
    vector<unique_ptr<string>> vec;
    // vec.push_back(up3);                              // 不允许直接赋值
    vec.push_back(move(up3));                           // 需要通过右值构造赋值

    unique_ptr<string> up4(new string("Hello JiangXI"));
    // unique_ptr中一些可能用到的成员函数
    cout << "通过运算符 * 返回普通对象 ：" << (*up4) << endl;           // 能够通过*或者->操作普通对象或普通指针的原因是在类中重载了这两个运算符
    cout << "通过运算符 -> 返回指针对象 ：" << up4->size() << endl;

    cout << "智能指针托管的指针地址 = " << vec[0].get() << endl;    // 获取智能指针中托管的普通指针，一般用不到，直接操作智能指针就行
    
    up4.reset(new string("Hello LiChuan"));                     // 重置智能指针托管的内存地址，如果地址不一致，原来的会被析构掉
    cout << "通过运算符 * 返回普通对象 ：" << (*up4) << endl;
}

// unique_ptr一旦取消托管，再使用就会报错，这是unique_ptr的排他性导致的
// 为了解决上述局限性，让指针对象可以被共享，shared_ptr便出现了
// 核心思想：当出现智能指针的拷贝时，引用计数+1；当智能指针析构时，引用计数-1，如果引用计数为0，就将这块内存释放
void fun_shared_ptr() {
    shared_ptr<int> sp1 = make_shared<int>(1);                       // 使用make_shared的初始化方式，只会在堆区new一次，效率更高
    shared_ptr<int> sp2(sp1);
    shared_ptr<int> sp3 = sp2;
    cout << "引用计数 = " << sp1.use_count() << endl;       // 获取引用计数

    // 同一普通指针不能托管给多个shared_ptr，否则会多次delete，造成程序崩溃
    // int* zn = new int(2);
    // shared_ptr<int> sp5(zn);
    // shared_ptr<int> sp6(zn);
}

// shared_ptr的使用陷阱：如果A中有B的智能指针、B中有A的智能指针，当他们相互持有对方的引用，会因为循环引用无法释放内存
void circul_ref() {
    // shared_ptr<A> a(new A());
    // shared_ptr<B> b(new B());
    shared_ptr<A> a = make_shared<A>();
    shared_ptr<B> b = make_shared<B>();

    a->set_b(b);
    b->set_a(a);                        // 如果将这行注释，a、b被析构 -> *A引用计数为0内存被释放 -> *A的成员变量_b被析构 -> *B的引用计数为0内存被释放

    cout << "a 的引用计数 = " << a.use_count() << endl;
    cout << "b 的引用计数 = " << b.use_count() << endl;
}

// 1、weak_ptr可以解决shared_ptr循环引用的问题，因为弱引用的构造和析构不会影响引用计数
// 2、weak_ptr没有重载*和->，可以通过成员函数.lock转换为共享指针后使用，但使用后必须将新的共享指针置NULL
void fun_weak_ptr() {
    shared_ptr<A> a = make_shared<A>();
    shared_ptr<Weak_B> weak_b = make_shared<Weak_B>();

    a->set_weak_b(weak_b);
    weak_b->set_a(a);
    weak_b->show_a();

    cout << "a 的引用计数 = " << a.use_count() << endl;
    cout << "weak_b 的引用计数 = " << weak_b.use_count() << endl;
}

int main() {
    cout << endl << "-------------------------普通内存泄漏-----------------------" << endl << endl;
    fun();                    // 内存泄漏

    cout << endl << "-------------------------智能指针unique-----------------------" << endl << endl;

    fun_unique_ptr();         // unique_ptr用法

    cout << endl << "-------------------------智能指针shared-----------------------" << endl << endl;

    fun_shared_ptr();         // shared_ptr用法

    cout << endl << "-------------------------shared_ptr的循环引用-----------------------" << endl << endl;

    circul_ref();

    cout << endl << "-------------------------智能指针weak-----------------------" << endl << endl;

    fun_weak_ptr();          // weak_ptr用法

    cout << endl << "-------------------------内存泄漏检测-----------------------" << endl << endl;

    checkLeaks();

    return 0;
}