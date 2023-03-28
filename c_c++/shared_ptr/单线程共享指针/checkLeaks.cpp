#include <iostream>
#include <cstdlib>
#include <vector>

#define SIZE 500

using namespace std;
void* v[SIZE] = {NULL};
int z = 0;

/* 重载new操作符 */
void* operator new (long unsigned int size) {
    if(size == 0) size = 1;
    void *mem = malloc(size);
    cout << "重载new    内存地址 = " << mem << "      内存长度 = " << size << endl;
    v[z] = mem;
    z++;
    return mem;
}

/* 重载delete操作符 */
void operator delete (void *ptr) {
    if(!ptr) return;
    free(ptr);
    cout << "重载delete    内存地址 = " << ptr << endl;
    for(int i = 0; i < SIZE; i++) {
        if(v[i] == ptr) {
            v[i] = NULL;
        }
    }
}

/* 重载new[]操作符 */
void* operator new[] (long unsigned int size) {
    if(size == 0) size = 1;
    void *mem = malloc(size);
    cout << "重载new[]    内存地址 = " << mem << "      内存长度 = " << size << endl;
    v[z] = mem;
    z++;
    return mem;
}

/* 重载delete[]操作符 */
void operator delete[] (void *ptr) {
    if(!ptr) return;
    free(ptr);
    cout << "重载delete[]    内存地址 = " << ptr << endl;
    for(int i = 0; i < SIZE; i++) {
        if(v[i] == ptr) {
            v[i] = NULL;
        }
    }
}

void checkLeaks() {
    for(int i = 0; i < SIZE; i++) {
        if (v[i] != NULL)
            cout << "发现内存泄漏!!!，内存地址 = " << v[i] << endl << endl;
    }
}