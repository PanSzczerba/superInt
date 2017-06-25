#ifndef _SUPER_INT_H
#define _SUPER_INT_H

#include <cstddef>

class superInt
{
    long long int* tblptr;
    std::size_t length;
public:
    superInt(long long int = 0);
    superInt(superInt&);
    ~superInt();
    inline std::size_t getLength() { return length; }
};
#endif // _SUPER_INT_H
