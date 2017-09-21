#ifndef _SUPER_INT_H
#define _SUPER_INT_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <stdexcept>

class superInt
{
    uint32_t* tblPtr;
    std::size_t tblLength;
    std::size_t length;

public:
    superInt(long long int=0);
    explicit superInt(std::string str);
    explicit superInt(const char* str);
    superInt(const superInt&);
    ~superInt();

    superInt& operator=(const superInt& other);

    superInt& operator|=(const superInt& other);
    superInt& operator&=(const superInt& other);
    superInt& operator^=(const superInt& other);
    superInt& operator>>=(uint32_t num);    // shift right
    superInt& operator<<=(uint32_t num);    // shift left 
    superInt& negate();

    superInt& operator+=(const superInt& other);
//  superInt& operator+=(int32_t num);
    superInt& operator-=(const superInt& other);
//  superInt& operator-=(int32_t num);
    superInt& operator*=(const superInt& other);
//  superInt& operator*=(long long int num);
    superInt& operator/=(const superInt& other) throw(std::logic_error);
    superInt& operator%=(const superInt& other) throw(std::logic_error);
//  superInt& operator/=(int32_t num) throw(std::logic_error);

    superInt operator|(const superInt& other) const;
    superInt operator&(const superInt& other) const;
    superInt operator^(const superInt& other) const;
    superInt operator~() const;
    superInt operator<<(uint32_t num) const;
    superInt operator>>(uint32_t num) const;

    int compare(const superInt& other) const;
    bool operator<(const superInt& other) const;
    bool operator>(const superInt& other) const;
    bool operator<=(const superInt& other) const;
    bool operator>=(const superInt& other) const;
    bool operator==(const superInt& other) const;
    bool operator!=(const superInt& other) const;

    superInt operator-()const;

    superInt operator+(const superInt& other) const;
//  superInt operator+(uint32_t num) const;
    superInt operator-(const superInt& other) const;
//  superInt operator-(uint32_t num) const;
    superInt operator*(const superInt& other) const;
    superInt operator/(const superInt& other) const throw(std::logic_error);
    superInt operator%(const superInt& other) const throw(std::logic_error);
    inline std::size_t getLength() { return length*sizeof(uint32_t); }
    inline short sign() const { return (tblPtr[length - 1] & ((uint32_t)1 << (sizeof(uint32_t)*8 - 1))) ? 1 : 0; }
    void printBinary() const;
    std::string toString(unsigned int base=10) const throw(std::invalid_argument);

    friend std::ostream& operator<<(std::ostream& out,const superInt& s);
    
private:
    void changeTblSize(size_t newTblLength);
    void extend(size_t newLength) throw(std::logic_error);
    void correct();
};
#endif // _SUPER_INT_H
