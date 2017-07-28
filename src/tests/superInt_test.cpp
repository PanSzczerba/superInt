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
    superInt s1 = 1;
    superInt s2 = -1;
    cout<<s1.sign()<<endl;
    s1.printBinary();
    s2.printBinary();
    superInt s3 = s1 + s2;
    s3.printBinary();
    return 0;
}
