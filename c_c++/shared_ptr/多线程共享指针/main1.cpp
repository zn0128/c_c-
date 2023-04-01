#include <iostream>
#include <string>
#include <mutex>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <memory>           // 智能指针头文件
#include "checkLeaks.cpp"
using namespace std;

class ReportClass {
public:
    ReportClass() {}
    ReportClass(const ReportClass&) = delete;
    ReportClass& operator=(const ReportClass&) = delete;
    ReportClass(const ReportClass&&) = delete;
    ReportClass& operator=(const ReportClass&&) = delete;

    static ReportClass* GetInstance();      // 生成或获取单例
    static void ReleaseInstance();          // 释放单例

private:
    mutex mutex_;
    int count_ = 0;
    void addWorkThread();                   // 新增工作线程
    static void workThread(ReportClass* report);        // 
    static ReportClass* instance_;
    static mutex static_mutex_;

    bool stop_ = false;
    thread *work_thread_;
};

mutex ReportClass::static_mutex_;
ReportClass* ReportClass::instance_;

ReportClass* ReportClass::GetInstance()
{
    lock_guard<mutex> lock(static_mutex_);
    if (instance_ == nullptr) {
        instance_ = new ReportClass();
        instance_->addWorkThread();
    }
    return instance_;
}

void ReportClass::ReleaseInstance()
{
    lock_guard<mutex> lock(static_mutex_);
    if(instance_ != nullptr)
    {
        delete instance_;
        instance_ = nullptr;
    }
}

// 1、线程指针未被释放，产生了内存泄露
// 2、由于detach的线程中的静态成员变量在主线程中被释放，此时调用该堆区数据会产生崩溃
void ReportClass::workThread(ReportClass *report)
{
    while(true)
    {
        // 线程运行过程中，report可能已经被销毁了
        unique_lock<mutex> lock(report->mutex_);

        if(report->count_ > 0)
        {
            report->count_--;
        }
        sleep(1);
    }
}

void ReportClass::addWorkThread()
{
    thread new_thread(workThread, this);
    new_thread.detach();
}


int main() {
    ReportClass::GetInstance();

    ReportClass::GetInstance()->ReleaseInstance();

    sleep(2);

    return 0;
}