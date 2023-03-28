#include <iostream>

#define SIZEOF(a) (cout << "size of "#a" = " << sizeof(a) << endl)

using namespace std;

class Empty { };

class Int_include{ int i;};

class Static_include{public: static int a;};

class Const_include{ const int a = 1;};

class Virtual_include{ virtual void fun(){}};

class Public_Int_include : public Int_include { char c;};

int main() {
    Empty empty;
    SIZEOF(empty);      // 空类对象的内存大小为1字节，是为了让对象之间能够相互区别

    Int_include int_include;
    SIZEOF(int_include);      // 含有一个int成员变量对象的内存大小为4字节

    Static_include static_include;
    SIZEOF(static_include);      // static类型的成员变量不计入对象的内存，大小为1字节

    Const_include const_include;
    SIZEOF(const_include);      // const类型的成员变量算入对象的内存，大小为4字节

    Const_include virtual_include;
    SIZEOF(virtual_include);      // 由于包含虚函数，c++编译器会创建默认构造函数为对象生成一个虚表指针vptr，大小为4字节

    Public_Int_include public_int_include;
    SIZEOF(public_int_include);      // 继承会算入基类成员变量的内存。由于内存对齐的原因，原本应该占5字节的对象，实际占用8字节（即处理器只能从4的倍数内存地址读取数据）
    
    return 0;
}