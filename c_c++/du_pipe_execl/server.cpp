#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <vector>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <unordered_map>
#include <queue>
using namespace std;

template<class T, T nullvalue, class D, D d> // auto d works in new compilers.
struct generic_delete
{
    class pointer
    {
        T t;
    public:
        pointer(T t) : t(t) {}
        pointer(std::nullptr_t = nullptr) : t(nullvalue) { }
        explicit operator bool() { return t != nullvalue; }
        friend bool operator ==(pointer lhs, pointer rhs) { return lhs.t == rhs.t; }
        friend bool operator !=(pointer lhs, pointer rhs) { return lhs.t != rhs.t; }
        operator T() { return t; }
    };
    void operator()(pointer p)
    {
        d(p);
    }
};

using unique_fd = std::unique_ptr<void, generic_delete<int, -1, decltype(&close), &close>>;      // Android中有现成的
int k = 20; 

int insertPosition(vector<pair<std::string, int>> &vec, int target) {
    if (!vec.empty() && vec.back().second >= target) return vec.size();

    int n = vec.size(), mid;
    int left = 0, right = n - 1, ans = n;

    while (left <= right) {
        mid = ((right - left) >> 1) + left;
        if (target >= vec[mid].second) {
            ans = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return ans;
}

void RunDuCommand(vector<pair<std::string, int>>& retVector) {
    const char kDuPath[] = "/usr/bin/du";       // Android源码中换成/system/bin/du
    const char kSortPath[] = "/usr/bin/sort";       // Android源码中换成/system/bin/sort

    int fds[2];
    if (pipe(fds) != 0) {
        printf("Failed to create pipe");
    }
    unique_fd read_fd(fds[0]);
    unique_fd write_fd(fds[1]);
    fcntl(read_fd.get(), F_SETFL, O_CLOEXEC);


    const pid_t child_pid = fork();
    if (child_pid == -1) {
        cout << "Failed to fork child";
        return;
    } else if (child_pid == 0) {
        close(0);
        dup2(write_fd.get(), 1);
        read_fd.reset();
        write_fd.reset();
        if (execl(kDuPath, "du", "-d 5", "/home/mi/study/language_study/", nullptr) < 0) {
            cout << "Failed to execl in child process";
            return;
        }
    }

    write_fd.reset();
    char line[500], *p, *q;
    int a;
    FILE* file = fdopen(read_fd.get(), "r");
    while (fgets(line, sizeof(line), file) != NULL) {
        p = strtok(line, "	");
        q = strtok(NULL, "\n");
        // cout << p << "         " << q << endl;
        // cout << strlen(p) << "         " << strlen(q) << endl;

        a = atoi(p);
        // cout << "a = " << a << endl;
        int position = insertPosition(retVector, a);
        if (position > k) continue;

        retVector.insert(retVector.begin() + position, make_pair(q, a));
        if (retVector.size() > k) retVector.pop_back();
    }

    return;
}

int main(int argc, char** argv){
    vector<pair<std::string, int>> retVector;
    RunDuCommand(retVector);
    cout << "\n \n" << "Top " << k << " file list as below : " << endl;
    std::vector<std::string> retVec;
    for (int i = 0; i < retVector.size(); i++) {
        retVec.push_back(std::to_string(retVector[i].second).append("\t\t").append(retVector[i].first));
        // cout << retVector[i].second << "\t\t" << retVector[i].first << endl;
        cout << retVec[i] << endl;
    }
    return 0;
}