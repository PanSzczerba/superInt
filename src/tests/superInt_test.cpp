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
    superInt s1("1111111111111111111111111111111111");
    cout<<s1<<endl;
    cout<<(-s1)<<endl;
    cout<<(s1*9)<<endl;
    cout<<(s1 << 2)<<"\n\n";

    superInt s2("2222222222222222222222222222222222");
    cout<<s2<<endl;
    cout<<(s2*s1)<<endl;
}
