#include <iostream>
using namespace std;
class A{
    public:
        virtual void show() {
        }
};
class B : public A{
    public:
        virtual void show() {
        }
};
class C{
};
class D : public C{
};
int main() {
#if 1 
    A *pbase = new B;
    B *pder = dynamic_cast<B *>(pbase);
    cout << pder << endl;
    delete pder;
    pbase = new A;
    pder = dynamic_cast<B *>(pbase);
    cout << pder << endl;
    delete pbase;
#else
    C *pbase = new D;
    D *pder = dynamic_cast<D *>(pbase);
#endif
    return 0;
}
