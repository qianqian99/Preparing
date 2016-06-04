#include <iostream>
using namespace std;
int main() {
    int a = 10;
    float f = *reinterpret_cast<float *>(&a);
    cout << f << endl;
    //2
    void *p = &a;
    p = reinterpret_cast<int *>(a);
    // error reinterpret_cast<int>(p);
    cout << p << endl;
    return 0;
}
