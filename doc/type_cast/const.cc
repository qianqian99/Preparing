#include <iostream>
using namespace std;
int main() {
    /*const_cast 之后执行写操作将会未定义*/
    const int a = 10;
    const int *p = &a;
    const_cast<int&>(*p) = 100;
    cout << a << endl;
    int *q = const_cast<int *>(p);
    *q = 99;
    /*must be pointer reference*/
    //*q = const_cast<int>(*p);
    cout << a << endl;
    int volatile *pa;
    //int *pb = pa;
    int *pb = const_cast<int *>(pa);
    return 0;
}
