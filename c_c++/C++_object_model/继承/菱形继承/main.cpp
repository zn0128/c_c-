// https://tangocc.github.io/2018/03/20/cpp-class-memory-struct/
#include <iostream>

using namespace std;

//              **********              //  菱形继承（无虚继承）
class People1 {
public:
    int age;
};

class Teacher1 : public People1 {
public:
    int level;
};

class Student1 : public People1 {
public:
    int score;
};

class Assistant1 : public Teacher1, public Student1{
public:
    int money;
};
//              **********              //


//              **********              //  菱形继承（虚继承）
class People2 {
public:
    int age;
};

class Teacher2 : virtual public People2 {
public:
    int level;
};

class Student2 : virtual public People2 {
public:
    int score;
};

class Assistant2 : public Teacher2, public Student2{
public:
    int money;
};
//              **********              //

int main() {
    Assistant1 a1;
    // a1.age = 19;        // 由于最底层的类继承了两次基类，当对基类public属性调用是会产生二义性，即不知道是从哪个中间父类过去的
    a1.Teacher1::age = 19;          // 二义性可以通过强制定义路径消除，但数据冗余无法解决


    Assistant2 a2;         // 通过将中间类设置为虚基类继承，可以除去二义性和数据冗余，这是由于设置为虚基类时，只会生成一个基类对象（有点像虚函数表，虽然产生了继承，但子类和基类的vptr都指向同一个虚表,避免冗余）
    a2.age = 21;

    return 0;
}