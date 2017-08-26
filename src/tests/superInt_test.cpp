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
    superInt s3 = s1;
    s1.printBinary();
    s3 *= s1;
    s3.printBinary();
    s2 = s3 * (-1);
    s2.printBinary();
    s2 *= -8;
    s2.printBinary();

}
