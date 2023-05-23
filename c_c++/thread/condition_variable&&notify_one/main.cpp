#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

using namespace std;

mutex mtx;
condition_variable cv;
bool flag = false;

void print_id(int id) {
    unique_lock<mutex> lck(mtx);
    cv.wait(lck, [&](){ return flag;});

    std::cout << "thread " << id << '\n';
}

void go() {
    unique_lock<std::mutex> lck(mtx);
    flag = true;
	cv.notify_all();
}

int main() {
    go();
    thread thread1(print_id, 1);
    cout << "threads ready to race...\n";
    sleep(2);
    // go();

    thread1.join();
    return 0;
}