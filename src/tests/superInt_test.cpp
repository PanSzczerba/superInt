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
    superInt s2 = 0x7fffffff;
    s1.printBinary();
    s2.printBinary();
    s1 <<= 1;
    s1.printBinary();
    s2.negate();
    s2.printBinary();
    s2 <<= 4;
    s2.printBinary();


    cout<<endl;
    s1 = s2*(-1);
    s1.printBinary();
    (s1 << 123123 >> 123123).printBinary();
}
