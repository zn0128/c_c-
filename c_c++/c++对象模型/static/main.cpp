// 静态变量相比于全局变量：相同的存储位置（全局数据区）、相同的生命周期，但静态变量只能作用于自己的作用域

#include <iostream>
#include <string>

using namespace std;

class Student{
public:
    Student(int _age) : age(_age) { }

    int get_age() { return age;}

    // 1、静态成员函数属于类，被所有类对象共有
    // 2、静态成员函数并不隐式的包含一个this指针（普通成员函数会包含），因此只能调用静态成员变量或者其他的静态成员函数
    // 3、普通成员函数可以访问静态成员函数，这是因为前者的出现晚于后者
    // 4、静态成员函数效率更高，原因是没有this指针的额外开销
    // 作用：调用一个跟类对象无关的函数，并且可以访问到类内的静态成员变量
    // 优点：静态成员变量相比于全局变量，前者的作用域仅仅在类中，可以实现信息隐藏
    static string get_school() { return school;}                                                                        // 静态成员函数

private:
    // 1、静态成员变量属于类，被所有类对象共有
    // 2、静态成员变量不占用类对象的内存，在类对象实例化之前就已经存在
    // 3、静态成员变量存放在内存的全局数据区，只在初始化时分配
    // 4、静态成员变量的初始化只能在类外实现，不然会导致每个对象都包含该静态成员
    // 5、静态常量成员变量可以在类内初始化
    // 6、静态成员变量的生命周期时初始化到程序结束
    // 作用：静态成员变量可以在不同对象之间共享，即可以用来定义不同对象某项相同的属性（如每个贷款人的利息虽然会改变，但总是相同）
    // 优点：静态成员变量相比于全局变量，前者的作用域仅仅在类中，可以实现信息隐藏
    static string school;                                                                                               // 静态成员变量
    static const int count = 0;                                                                                         // 静态常量成员变量
    int age;
};

// 静态成员变量的初始化（与成员函数的类外实现有点相同）
string Student::school = "黎川二中";


// 1、静态全局变量存放在内存的全局数据区
// 2、静态全局变量如果没有初始化，赋值为0
// 3、静态全局变量的生命周期从声明开始到程序结束
// 作用：声明对整个文件可见的变量
// 优点：静态全局变量相比于全局变量，不能被其它文件所调用，不会和其他文件相同名字的变量产生冲突
static string city = "抚州";                                                                                            // 静态全局变量


// 1、静态函数只在当前文件可见，这个也是和普通函数相比的优点
static string get_school(Student s) {                                                                                     // 静态函数
    return s.get_school();
}

int get_age() {
    // 1、静态局部变量存放在内存的全局数据区
    // 2、静态全局变量如果没有初始化，赋值为0
    // 3、静态局部变量的生命周期是声明开始到程序结束
    // 4、静态局部变量不会随着函数退栈而被释放，因此不需要重新申请内存
    // 5、静态局部变量的作用域只在函数内
    // 作用：静态局部变量的值会保持到下一次函数调用，直到被重新赋值，可以用来定义在某个函数内部需要重复使用的变量
    static int age = 1;                                                                                   // 静态局部变量

    return age++;
}

int main() {
    Student s1(18);
    cout << "age of s1 = " << s1.get_age() << endl;
    cout << "school of s1 = " << s1.get_school() << endl;
    cout << "school of Student = " << Student::get_school() << endl;
    cout << "get school of s1 = " << get_school(s1) << endl;

    cout << "city = " << city << endl;

    cout << "get age, once = " << get_age() << endl;
    cout << "get age, two times = " << get_age() << endl;
    cout << "get age, three times = " << get_age() << endl;
    cout << "get age, four times = " << get_age() << endl;

    return 0;
}