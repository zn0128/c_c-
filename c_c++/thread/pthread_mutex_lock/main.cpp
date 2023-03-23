#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

using namespace std;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *fun1(void *ptr){
    pthread_mutex_lock(&mtx);
    cout << "wait begin" << endl;
    pthread_cond_wait(&cond, &mtx);
    cout << "wait end" << endl;
    pthread_mutex_unlock(&mtx);
}

int main() {
    pthread_t tid1, tid2;
    string buf = "";
    pthread_create(&tid1, NULL, fun1, NULL);
    getline(cin, buf);
    if(buf == "ok") {
        cout << "send cond ok" << endl;
        pthread_cond_signal(&cond);         // 也可以用pthread_cond_signal
    } else {
        cout << "send cond not ok" << endl;
    }
    sleep(2);
    return 0;
}