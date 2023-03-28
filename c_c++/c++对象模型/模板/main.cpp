#include <iostream>

using namespace std;

template <class type1, class type2>
class Point3d {
public:
    Point3d(type1 x = 0, type1 y = 0, type2 z = 'a') : _x(x), _y(y), _z(z) {}   // 默认三维等于0

    Point3d operator+(const Point3d &other);        // 在类中声明，类外实现

    type1 get_x() const {return _x;};                 // 声明为常函数，无法修改成员变量
    type1 get_y() const {return _y;};
    type2 get_z() const {return _z;};

    void set_x(type1 val) {_x = val;}
    void set_y(type1 val) {_y = val;}
    void set_z(type2 val) {_z = val;}
    

private:
    type1 _x;
    type1 _y;
    type2 _z;
};

template<>                                      // c++编译器要求在类外实现时，需要加上template<>让编译器知道时模板类
Point3d<int, char> Point3d<int, char>::operator+(const Point3d<int, char> &other) {                  // 重载+运算符，实现xyz的加法
    Point3d pt(get_x() + other.get_x(), get_y() + other.get_y(), get_z() + other.get_z());
    return pt;
}

inline ostream& operator<<(ostream &os, const Point3d<int, char> &pt) {       // 指针类型必须和调用的函数类型一致，都是const
    os << "(" << pt.get_x() << "," << pt.get_y() << "," << pt.get_z() << ")";       // 重载<<
    return os;
}

int main() {
    Point3d<int, char> pt1(1,2,'b');
    Point3d<int, char> pt2(4,5,'c');

    cout << "pt1 = " << pt1 << endl;
    cout << "pt1 = " << pt1 << "    pt2 = " << pt2 << endl;

    Point3d<int, char> pt3 = pt1 + pt2;
    cout << "pt3 = " << pt3 << endl;

    return 0;
}