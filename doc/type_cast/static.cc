#include <iostream>
#include <typeinfo>
using namespace std;
class A{
    public:
        int a;
};
class B : public A{
    public:
        int c;
};
int main() {
    //1
    int a = 10;
    float f = static_cast<float>(a);
    cout << f << endl;
    //2
    A *base;
    B *der;
    der = static_cast<B *>(base);
    base = der;
    //3
    static_cast<void>(a);
    return 0;
}
