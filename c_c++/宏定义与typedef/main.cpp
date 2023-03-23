#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

// C语言从书写到执行的流程为：预处理、编译、汇编、链接
// 其中#开头的宏定义和#include一样是在预处理阶段完成的，宏形式如下
// #define 宏名 替换文本
// 替换文本可以是常数、表达式、字符串等，在预处理过程中，代码中所有出现的宏名都会被“替换文本”所替换，这个过程被成为宏展开
// 宏只参与嵌套，不会进行运算，如果出现错误也只会在编译过程

#define M 5

#define N "Hello world"

#define HELLO "hello wo\
rld"

#define COUNT(M) M*M

#define INT1 int*
typedef int* INT2;

#define SUM(a, b) printf(#a" + "#b" = %d\n", ((a) + (b)))

#define NAME(n) (num ## n)

#define PR(...) printf(__VA_ARGS__)

#define PR_INT(X, ...) printf("msg = "#X, __VA_ARGS__)


// 1、宏定义的基本用法
void fun1() {
    cout << "M = " << M << endl;
    cout << "N = " << N << endl;
    cout << "HELLO = " << HELLO << endl;            // 通过\符换行
}

// 2、有参宏
// 格式：#define 宏名(实参) 替换文本。有参宏在调用中，不仅进行宏展开，还要用实参去替换形参
void fun2() {
    cout << "COUNT(5) = " << COUNT(5) << endl;
    cout << "COUNT(5 + 1) = " << COUNT(5 + 1) << endl;      // 这里的实际输出是11，原因是宏定义只进行文本替换，导致运算的过程变成了5 + 1 * 5 + 1 = 11
}

// 3、与 typedef 的区别
// 宏定义只做文本替换，代替指针时只能为第一个变量设置为指针；而typedef相当于类型说明符的重定义
void fun3() {
    int x = 1;
    INT1 a1, b1;
    INT2 a2, b2;

    // b1 = &x;        // 此处编译不通过，原因是宏定义只做文本替换，INT1 a1, b1;对应 int *a1, b1;。b1是一个int型变量而非指针
    b2 = &x;
}

// 4、# 运算符
// 通过#运算符可以在“替换文本”中将实参转换为字符串，可以在printf或者cout时使用，如 #1 = “1”
void fun4() {
    SUM(1 + 2, 3 + 4);
}

// 5、## 运算符
// 通过##运算符可以把“替换文本”中两个语言符号粘合为一个语言符号，也称为预处理器的粘合剂
void fun5() {
    int num0 = 100;
    cout << "num0 = " << NAME(0) << endl;
}

// 6、可变宏：… 和 __VA_ARGS__
// 省略号“…”只能用来替换宏的形参列表中最后一个！
// 形参的最后一个参数为省略号 "..."，而“__VA_ARGS__”的作用就是替换省略号代表的文本
void fun6() {
    PR("hello world\n");            // 等同于 print("hello world\n")

    double msg = 10;
    PR_INT(%f, msg);                // 等同于 printf("msg = %f", msg)
}

int main() {
    fun1();             // 1、宏定义的基本用法

    fun2();             // 2、有参宏

    fun3();             // 3、与 typedef 的区别

    fun4();             // 4、# 运算符

    fun5();             // 5、## 运算符

    fun6();             // 6、可变宏：… 和 __VA_ARGS__

    return 0;
}