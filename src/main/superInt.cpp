#include <cstring>
#include <iostream>
#include "superInt.h"
using namespace std;

superInt::superInt(const int number)
{
    tblPtr = new uint32_t[1];
    length = 1;
    tblLength = 1;
    tblPtr[0] = number;
}
superInt::superInt(const superInt& other)
{
//    cout<<"superInt(superInt& other)"<<endl;
    length = other.length;
    tblLength = other.tblLength;
    tblPtr = new uint32_t[tblLength];
    memcpy(tblPtr, other.tblPtr, length*sizeof(uint32_t));
}
superInt::~superInt()
{
    delete tblPtr;
}
void superInt::printBinary()
{
    for(size_t i = 0; i < length; i++)
    {
        for(size_t j = 0; j < sizeof(uint32_t)*8; j++)
        {
            if(tblPtr[length - 1 - i] & ((uint32_t)1 << sizeof(uint32_t)*8 - 1 - j))
                cout<<"1";
            else
                cout<<"0";
        }
    }
    cout<<endl;
}

void superInt::setTblLength(const size_t newTblLength)
{
    uint32_t* newTblPtr = new uint32_t[newTblLength];
    memcpy(newTblPtr, tblPtr, length*sizeof(uint32_t));
    delete tblPtr;
    tblPtr = newTblPtr;
    tblLength = newTblLength;
}

void superInt::correct()
{
    if(length < tblLength/4)
        setTblLength(tblLength/2);
}

superInt& superInt::operator+=(const superInt& other)
{
    if(tblLength < other.tblLength && length < other.length) //change size if table size is smaller then the other one
    {
        setTblLength(other.tblLength);
        memset(tblPtr + length, 0, sizeof(uint32_t)*(other.length - length));
        length = other.length;
    }

    bool carry = false;
    uint32_t result;
    size_t i;
    for(i = 0; i < other.length; i++)
    {
        result = other.tblPtr[i] + tblPtr[i] + (carry ? 1 : 0);
        if(result < tblPtr[i])
            carry = true;
        else
            carry = false;

        tblPtr[i] = result;
    }

    while(carry)
    {
        if(i == length)
        {
            if(length == tblLength)
                setTblLength(tblLength*2);

            tblPtr[length++] = (uint32_t)1;
            carry = false;
        }
        else
        {
            result = tblPtr[i] + (carry ? 1 : 0);
            if(result == 0)
                carry = true;
            else
                carry = false;

            tblPtr[i] = result;
        }
        i++;
    }
    return *this;
}
superInt superInt::operator+(const superInt& other) const
{
    if(length > other.length)
        return superInt(*this) += other;
    else
        return superInt(other) += *this;
}
