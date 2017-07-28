#ifndef _SUPER_INT_H
#define _SUPER_INT_H

#include <cstddef>
#include <cstdint>

class superInt
{
    uint32_t* tblPtr;
    std::size_t tblLength;
    std::size_t length;
public:
    superInt(const int = 0);
    superInt(const superInt&);
    ~superInt();
    void setTblLength(const size_t newTblLength);
    void correct();
    superInt& operator+=(const superInt& other);
    superInt operator+(const superInt& other) const;
    inline std::size_t getLength() { return length; }
    inline short sign() { return (tblPtr[length - 1] & ((uint32_t)1 << sizeof(uint32_t)*8 - 1)) ? 1 : 0; }
    void printBinary();
};
#endif // _SUPER_INT_H
