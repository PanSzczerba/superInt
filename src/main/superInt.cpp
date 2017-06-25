#include <cstring>
#include "superInt.h"

superInt::superInt(long long int number)
{
    tblptr = new long long int[1];
    length = 1;
    tblptr[0] = number;
}
superInt::superInt(superInt& other)
{
    length = other.getLength();
    tblptr = new long long int[length];
    memcpy(tblptr, other.tblptr, length*sizeof(long long int));
}
superInt::~superInt()
{
    delete tblptr;
}
