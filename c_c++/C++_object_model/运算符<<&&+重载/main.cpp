#include <iostream>

using namespace std;

class Point3d {
public:
    Point3d(int x = 0, int y = 0, int z = 0) : _x(x), _y(y), _z(z) {}   // 默认三维等于0

    Point3d operator+(const Point3d &other);        // 在类中声明，类外实现

    int get_x() const {return _x;};                 // 声明为常函数，无法修改成员变量
    int get_y() const {return _y;};
    int get_z() const {return _z;};

    void set_x(int val) {_x = val;}
    void set_y(int val) {_y = val;}
    void set_z(int val) {_z = val;}
    

private:
    int _x;
    int _y;
    int _z;
};

Point3d Point3d::operator+(const Point3d &other) {                  // 重载+运算符，实现xyz的加法
    Point3d pt(get_x() + other.get_x(), get_y() + other.get_y(), get_z() + other.get_z());
    return pt;
}

inline ostream& operator<<(ostream &os, const Point3d &pt) {       // 指针类型必须和调用的函数类型一致，都是const
    os << "(" << pt.get_x() << "," << pt.get_y() << "," << pt.get_z() << ")";       // 重载<<
    return os;
}

int main() {
    Point3d pt1(1,2,3);
    Point3d pt2(4,5,6);

    cout << "pt1 = " << pt1 << endl;
    cout << "pt1 = " << pt1 << "    pt2 = " << pt2 << endl;

    Point3d pt3 = pt1 + pt2;
    cout << "pt3 = " << pt3 << endl;

    return 0;
}