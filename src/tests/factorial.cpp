#include <iostream>
#include <cstdlib>
#include "superInt.h"
using namespace std;

int main(int argc, char** argv)
{
    long long int num;
    if(argc < 2)
        num = 100;
    else
        num = atoi(argv[1]);

    if(num < 0)
    {
        cout<<"Error: Can't count factorial if given number is below 0"<<endl;
        return 1;
    }
    
    superInt factorial(1);

    for(int i = 2; i < num; i += 1)
        factorial *= i;

//  cout<<factorial<<endl;
}
