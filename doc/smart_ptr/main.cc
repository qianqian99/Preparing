#include "smart_ptr.hpp"
#include <iostream>
using namespace std;
struct A
{
    int a;
    int b;
    A() : a(10), b(20){}
};
int main()
{
    hgg::shared_ptr<A> sp1(new A);
    hgg::shared_ptr<int> sp2(new int);
    hgg::weak_ptr<A> wp1(sp1);
    hgg::weak_ptr<int> wp2(sp2);
    hgg::shared_ptr<int> sp3(sp2);
    hgg::shared_ptr<int> sp4(wp2);
    hgg::weak_ptr<A> wp3(wp1);
    cout << *sp3 << endl;
    cout << sp1->a << endl;
    cout << wp1.lock()->b << endl;
    return 0;
}
