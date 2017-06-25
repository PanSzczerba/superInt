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
    superInt s1 = 3;
    superInt s2 = 2;
    ASSERT(s1 == 3)
    ASSERT(s1 + s2 == 5)
    ASSERT(s1 * s2 == 6)
}
