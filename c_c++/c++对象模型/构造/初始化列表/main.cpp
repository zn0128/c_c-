// 如果类中未定义任何构造函数，c++编译器会在构造函数“被需要”时候，合成默认构造函数（包括无参构造和拷贝构造<浅拷贝>），何为“被需要”呢：
// 1、如果类中声明了非基础类型（如Student s），且该非基础类中含有默认构造函数
// 2、作为一个子类，且父类含有默认构造函数
// 3、带有虚函数的类，也包括了继承了虚函数的类，此时c++编译器需要合成默认构造函数为类对象生成一个虚表指针vptr
// 4、带有虚基类的类，和上述类似

// 如果类中只类int等基础类型时，c++编译器是不会合成默认构造函数的

#include <iostream>
#include <memory>           // 智能指针头文件

using namespace std;

class Student {
public:
    Student() {
        m_age = (int*)(malloc(sizeof(int)));
        m_age[0] = 0;
        shared_ptr<int> pt(new int(0));
        m_sp_age = pt;
        cout << "无参构造, age = " << m_age  << "    m_sp_age = " << m_sp_age << endl;
    }

    Student(int age, shared_ptr<int> sp_age) {
        m_age = (int*)(malloc(sizeof(int)));
        m_age[0] = age;
        m_sp_age = sp_age;
        cout << "有参构造, age = " << m_age  << "    m_sp_age = " << m_sp_age << endl;
    }

    Student(Student &s) {       // 拷贝构造的形参必须是引用，否则形参赋值的时候本身就是一次拷贝构造，会再次调用拷贝构造函数，并陷入死循环
        // m_age = s.m_age;                                    // 浅拷贝，只简单的进行指针赋值，两个指针指向同一块内存，如果在析构函数中释放内存就会释放同一块内存两次，造成程序崩溃
    
        m_age = (int*)(malloc(sizeof(int)));                   // 深拷贝，如果需要对堆区数据进行拷贝，可以另外开辟一块堆区内存存放，避免两个指针指向同一块内存的情形（用智能指针也可以避免）
        *m_age = *s.m_age;

        m_sp_age = s.m_sp_age;                                 // 利用智能指针也可以避免浅拷贝造成的重复内存释放

        cout << "拷贝构造, age = " << m_age  << "    m_sp_age = " << m_sp_age << endl;
    }

    Student& operator=(Student &s) {
        *m_age = *s.m_age;
        m_sp_age = s.m_sp_age;
        cout << "拷贝赋值, age = " << m_age  << "    m_sp_age = " << m_sp_age << endl;
        return *this;
    }

    // 左值和右值：只要能放在等号左边的就是左值；而右值只能放在等号右边
    Student(Student &&s) {                                    // 移动构造，通过右值引用传递形参
        m_age = s.m_age;
        m_sp_age = s.m_sp_age;

        s.m_age = NULL;                                       // 将右值的指针废弃，避免深拷贝造成的空间浪费，并且避免重复释放内存造成的崩溃

        cout << "移动构造, age = " << m_age  << "    m_sp_age = " << m_sp_age << endl;
    }

    Student& operator=(Student &&s) {                                    // 移动构造，通过右值引用传递形参
        m_age = s.m_age;
        m_sp_age = s.m_sp_age;

        s.m_age = NULL;                                       // 将右值的指针废弃，避免深拷贝造成的空间浪费，并且避免重复释放内存造成的崩溃

        cout << "移动赋值, age = " << m_age  << "    m_sp_age = " << m_sp_age << endl;
        return *this;
    }

    ~Student() {
        free(m_age);
        m_age = NULL;
        cout << "析构" << endl;
    }

private:
    int *m_age;
    shared_ptr<int> m_sp_age;
};

class Pig {
public:
    Pig(int i) : s(25, shared_ptr<int>(new int(29))), a('s') {      // 利用初始化列表可以减少一次临时的Student对象的产生，提升效率。并且const类型的参数只能通过()完成初始化，不能在构造函数内赋值
        m_i = i;
        // cout << "begin" << endl;
        // s = Student(25, shared_ptr<int>(new int(29)));
        // cout << "end" << endl;
    }

private:
    int m_i;
    const char a;
    Student s;
};

class Dog {
public:
private:

};

int main() {
    Pig p1(2);

    return 0;
}