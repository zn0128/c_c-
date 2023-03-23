#include <iostream>

using namespace std;

class Point1d {
public:
    Point1d(int x = 0) : _x(x){}

    virtual void get_x() const {cout << "Point1d x" << endl;}
    virtual void get_y() const {cout << "Point1d y" << endl;}

    void set_x(int val) {_x = val;}
    
private:
    int _x;
};

class Point2d : public Point1d{         // 继承父类Point1d的成员函数和成员变量
public:
    Point2d(int x = 0, int y = 0) : Point1d(x), _y(y) {}

    virtual void get_x() const {cout << "Point2d x" << endl;}       // 重写了父类Point1d的虚函数get_x

    void set_y(int val) {_y = val;}
    
private:
    int _y;
};

class Point3d : public Point2d{
public:
    Point3d(int x = 0, int y = 0, int z = 0) : Point2d(x, y), _z(z) {}

    virtual void get_y() const {cout << "Point3d y" << endl;}           // 重写了父类Point2d的虚函数get_y

    void set_z(int val) {_z = val;}
    
private:
    int _z;
};

int main() {
    Point2d pt2;
    Point3d pt3;

    Point1d *p1_1 = &pt2;
    Point1d *p1_2 = &pt3;

    p1_1->get_x();                  // 重写虚函数后实现了多态
    p1_1->get_y();

    p1_2->get_x();                  // 重写虚函数后实现了多态
    p1_2->get_y();                  // 重写虚函数后实现了多态

    return 0;
}