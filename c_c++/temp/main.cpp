#include <iostream>

using namespace std;

int main() {
    char *c = (char* )malloc(5);
    *(c + 0) = 'a';
    *(c + 1) = 's';
    *(c + 16) = 'd';
    cout << *(c+12) << endl;
    free(c);
    return 0;
}