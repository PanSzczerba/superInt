#include <iostream>
#include <cstdlib>
#include "superInt.h"
using namespace std;

#define ASSERT(expr) \
    if(!(expr)) \
    {   \
        cerr<<"Error, expression : \""<<#expr<<"\" retrurned false"<<endl;\
        exit(1);\
    }

int main()
{
    superInt s1 = 0x7fffffff;
    cout<<s1<<endl;
    s1 <<= 32;
    cout<<(((s1 |= 0xffffffff) <<= 32) |= 0xffffffff)<<endl;
    cout<<(s1 *= 2)<<endl;
    cout<<(s1 / 2)<<endl;
    cout<<(-s1)<<endl;
}
