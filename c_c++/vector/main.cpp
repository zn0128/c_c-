#include <iostream>
#include <vector>
using namespace std;

class A {
public:
    A() { cout << "无参构造" << endl;}

    A(int *_a):a(_a) {
       // cout << "有参构造" << endl;
    }

    A(const A& other) {
        cout << "拷贝构造" << endl;
        a = (int *)malloc(sizeof(int));
        *a = *other.a;
    }

    A(A&& other) noexcept {
        cout << "move构造" << endl;
        a = other.a;
        other.a = NULL;
    }

    int* get_addr() {
        return a;
    }
    int b;

private:
    int *a;
};


int main() {
    vector<A> vec;
    for (int i = 0; i < 2; i++) {
        vec.push_back(new int(2));
        cout << "capacity = " << vec.capacity() << endl;
        for (int j = 0; j < vec.size(); j++) {
            cout << "addr of vec = " << &vec[j] << endl;
            cout << "addr of vec[" << j << "].get_addr() = " << vec[j].get_addr() << endl;
        }
    }
}