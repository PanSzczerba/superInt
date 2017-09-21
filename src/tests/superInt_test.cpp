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
    superInt s1("1111111111111111111111111");
    cout<<s1<<endl;
    cout<<(s1 *= 12)<<endl;
}
