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

    int get_count() { return count_;}

    ~ReportClass()
    {
        // 通过join来停止内部线程
        stop_ = true;
        work_thread_->join();
        delete work_thread_;
        work_thread_ = nullptr;
    }

    static shared_ptr<ReportClass> GetInstance();      // 生成或获取单例
    static void ReleaseInstance();          // 释放单例

private:
    mutex mutex_;
    int count_ = 0;
    void addWorkThread();                   // 新增工作线程
    static void workThread(ReportClass* report);        // 
    static shared_ptr<ReportClass> instance_;
    static mutex static_mutex_;

    bool stop_ = false;
    thread *work_thread_;
};

mutex ReportClass::static_mutex_;
shared_ptr<ReportClass> ReportClass::instance_;

shared_ptr<ReportClass> ReportClass::GetInstance()
{
    lock_guard<mutex> lock(static_mutex_);
    if (instance_ == nullptr) {
        instance_ = make_shared<ReportClass>();
        instance_->addWorkThread();
    }
    return instance_;
}

void ReportClass::ReleaseInstance()
{
    lock_guard<mutex> lock(static_mutex_);
    if(instance_ != nullptr)
    {
        instance_.reset();
    }
}

// 1、将detach的线程换成join，并在静态成员变量被析构时，通知该工作线程结束，之后再和主线程一起结束，避免了代码崩溃，并且防止了线程对象的内存泄露
// 2、但是在多线程的情况下，假如某个线程想要释放这个对象，但另外一个线程还在使用这个对象，可能会出现野指针问题，所以这个静态成员变量什么时候才能被释放就是需要解决的问题
void ReportClass::workThread(ReportClass *report)
{
    while(true)
    {
        // 线程运行过程中，report可能已经被销毁了
        unique_lock<mutex> lock(report->mutex_);

        if(report->stop_)
        {
            break;
        }

        if(report->count_ > 0)
        {
            report->count_--;
        }
        sleep(1);
    }
}

void ReportClass::addWorkThread()
{
    work_thread_ = new thread(workThread, this);
}

void thread1_fun() {
    sleep(1);
    cout << "引用计数 in thread1 = " << ReportClass::GetInstance().use_count() << endl;
    ReportClass::GetInstance()->ReleaseInstance();
}

void thread2_fun() {
    shared_ptr<ReportClass> report = ReportClass::GetInstance();
    cout << "引用计数 in thread2 point 1 = " << report.use_count() << endl;
    if(report) {
        sleep(2);
        // 此时切换到线程 A
        cout << "引用计数 in thread2 point 2 = " << report.use_count() << endl;
    }
}

int main() {
    thread *work_thread_1 = new thread(thread1_fun);
    
    thread *work_thread_2 = new thread(thread2_fun);
    
    work_thread_1->join();

    work_thread_2->join();

    delete(work_thread_1);
    work_thread_1 = NULL;
    delete(work_thread_2);
    work_thread_2 = NULL;

    return 0;
}