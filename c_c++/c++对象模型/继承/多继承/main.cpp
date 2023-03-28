// https://tangocc.github.io/2018/03/20/cpp-class-memory-struct/
#include <iostream>

using namespace std;

class Point1d {
public:
    Point1d(int x = 0) : _x(x){}

    int get_x() const {return _x;};

    void set_x(int val) {_x = val;}

private:
    int _x;
};

class Point2d : public Point1d{         // 继承父类Point1d的成员函数和成员变量
public:
    Point2d(int x = 0, int y = 0) : Point1d(x), _y(y) {}

    int get_y() const {return _y;};

    void set_y(int val) {_y = val;}
    
private:
    int _y;
};

class Point3d : public Point2d{
public:
    Point3d(int x = 0, int y = 0, int z = 0) : Point2d(x, y), _z(z) {}

    int get_z() const {return _z;};

    void set_z(int val) {_z = val;}
    
private:
    int _z;
};

inline ostream& operator<<(ostream &os, const Point3d &pt) {       // 指针类型必须和调用的函数类型一致，都是const
    os << "(" << pt.get_x() << "," << pt.get_y() << "," << pt.get_z() << ")";       // 重载<<
    return os;
}

int main() {
    Point3d pt1(1,2,3);
    Point3d pt2(4,5,6);

    cout << "pt1 = " << pt1 << endl;
    cout << "pt1 = " << pt1 << "    pt2 = " << pt2 << endl;

    return 0;
}