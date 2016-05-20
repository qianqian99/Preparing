#include "smart_ptr.hpp"
using namespace std;
int main()
{
    hgg::shared_ptr<int> sp(new int);
    hgg::shared_ptr<int> spp(sp);
    return 0;
}
