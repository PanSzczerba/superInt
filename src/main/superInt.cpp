#include <cstring>
#include <iostream>
#include <string>
#include "superInt.h"
using namespace std;

#define DEBUG(expr) cout<<#expr<<" = "<<(expr)<<endl;
#define DEBUGHEX(expr) cout<<hex<<#expr<<" = "<<(expr)<<endl;

superInt::superInt(uint32_t number)
{
    length = 1;
    tblLength = 1;
    tblPtr = new uint32_t[tblLength];
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

void superInt::printBinary() const
{
    for(size_t i = 0; i < length; i++)
    {
        for(size_t j = 0; j < sizeof(uint32_t)*8; j++)
        {
            if(tblPtr[length - 1 - i] & ((uint32_t)1 << (sizeof(uint32_t)*8 - 1 - j)))
                cout<<"1";
            else
                cout<<"0";
        }
    }
    cout<<endl;
}

void superInt::changeTblSize(size_t newTblLength)
{
    uint32_t* newTblPtr = new uint32_t[newTblLength];
    memcpy(newTblPtr, tblPtr, length*sizeof(uint32_t));
    delete tblPtr;
    tblPtr = newTblPtr;
    tblLength = newTblLength;
}

void superInt::extend(size_t newLength) throw(logic_error)
{
    if(newLength < length) throw logic_error("Can't extend length if new length is shorter");

    if(newLength > tblLength)
    {
        size_t newTblLength = tblLength;
        while(newTblLength < newLength)
            newTblLength *= 2;

        changeTblSize(newTblLength);
    }

    if(sign())
        memset(tblPtr + length, (uint32_t)(-1), sizeof(uint32_t)*(newLength - length));
    else
        memset(tblPtr + length, (uint32_t)0, sizeof(uint32_t)*(newLength - length));
    length = newLength;

}

void superInt::correct()
{
    if(length < tblLength/4)
        changeTblSize(tblLength/2);

    short this_sign = sign();
    size_t i;

    for(i = length - 1; i > 0 && tblPtr[i] == (uint32_t)(this_sign ? (-1) : 0); i--);

    if(!(this_sign ^ (short)((tblPtr[i] & ((uint32_t)1 << (sizeof(uint32_t)*8 - 1))) ? 1 : 0)))
        length = i + 1;
    else
        length = i + 2;

    if(length < tblLength/4)
        changeTblSize(tblLength/2);
}

char numberToChar(uint32_t num) throw(invalid_argument)
{
    if(num < 0 || num > 16) throw invalid_argument("numberToChar: Given number is not valid");
    return (char)(num < 10 ? (short)'0' + num : (short)'A' + num - 10);
}

string superInt::toString(unsigned int base) const throw(invalid_argument)
{
    if(base < 2 || base > 16) throw invalid_argument("toString: Base can't be below 2 or above 16");
    string str;
    superInt copy(*this);
    short sign = this->sign();

    if(sign)
    {
        copy.negate();
        copy += 1;
    }

    while(copy != 0)
    {
        str.push_back(numberToChar((copy % base).tblPtr[0]));
        copy /= base;
    }

    if(sign)
        str.push_back('-');

    for(size_t i = 0; i < str.length() / 2; i++)
    {
        char tmp = str[i];
        str[i] = str[str.length() - 1 - i];
        str[str.length() - 1 - i] = tmp;
    }

    return str;
}

superInt& superInt::operator=(const superInt& other)
{
    if(length < other.length)
        extend(other.length);
    else
        length = other.length;

    memcpy(tblPtr, other.tblPtr, length*sizeof(uint32_t));
    correct();

    return *this;
}

superInt& superInt::operator|=(const superInt& other)
{
    if(length < other.length) //change size if table size is smaller then the other one
        extend(other.length);
    for(size_t i = 0; i < other.length; i++)
        tblPtr[i] |= other.tblPtr[i];

    correct();
    return *this;
}

superInt& superInt::operator&=(const superInt& other)
{
    if(length < other.length) //change size if table size is smaller then the other one
        extend(other.length);
    for(size_t i = 0; i < other.length; i++)
        tblPtr[i] &= other.tblPtr[i];

    correct();
    return *this;
}

superInt& superInt::operator^=(const superInt& other)
{
    if(length < other.length) //change size if table size is smaller then the other one
        extend(other.length);
    for(size_t i = 0; i < other.length; i++)
        tblPtr[i] ^= other.tblPtr[i];

    correct();
    return *this;
}

superInt& superInt::operator>>=(uint32_t num)
{
    short sign = this->sign();
    uint32_t num_rem = num % (sizeof(uint32_t) * 8);
    uint32_t num_qnt = num / (sizeof(uint32_t) * 8);

    if(num_qnt >= length)
        return *this = superInt(sign ? (-1) : 0);


    int64_t sshift;

    if(length == num_qnt + 1)
    {
        sshift = ((uint64_t)tblPtr[length - 1]) << (sizeof(uint32_t) * 8);
        sshift >>= num_rem;
        tblPtr[0] = (uint32_t)(sshift >> (sizeof(uint32_t) * 8));
        length = 1;

        return *this;
    }
    
    tblPtr[0] = tblPtr[num_qnt] >> num_rem;

    uint64_t shift;
    size_t i;
    for(i = num_qnt + 1; i < length - 1; i++)
    {
        shift = ((uint64_t)tblPtr[i]) << (sizeof(uint32_t) * 8 - num_rem);
        tblPtr[i - num_qnt - 1] |= (uint32_t)shift;
        tblPtr[i - num_qnt] = (uint32_t)(shift >> (sizeof(uint32_t) * 8));
    }

    sshift = ((uint64_t)tblPtr[i]) << (sizeof(uint32_t) * 8);
    sshift >>= num_rem;
    tblPtr[i - num_qnt - 1] |= (uint32_t)sshift;
    tblPtr[i - num_qnt] = (uint32_t)(sshift >> (sizeof(uint32_t) * 8));
    length = length - num_qnt;

    correct();
    return *this;
}

superInt& superInt::operator<<=(uint32_t num)
{
    short sign = this->sign();
    uint32_t num_rem = num % ((sizeof(uint32_t)) * 8);
    uint32_t num_qnt = num / ((sizeof(uint32_t)) * 8);
    uint32_t oldLength = length;

    extend(length + num_qnt + 1);

    uint64_t shift;

    shift = ((sign ? ((uint64_t)0xffffffff << (sizeof(uint32_t) * 8)) : 0) | (uint64_t)tblPtr[oldLength - 1]) << num_rem;
    if(sign == 0)
        tblPtr[length - 1] |= (uint32_t)(shift >> (sizeof(uint32_t) * 8));
    else
        tblPtr[length - 1] &= (uint32_t)(shift >> (sizeof(uint32_t) * 8));
    tblPtr[length - 2] = (uint32_t)shift;

    size_t i;
    for(i = 1; i < oldLength; i++)
    {
        shift = (((uint64_t)tblPtr[(oldLength - 1) - i]) << num_rem);
        tblPtr[(length - 2) - i + 1] |= (uint32_t)(shift >> (sizeof(uint32_t) * 8));
        tblPtr[(length - 2) - i] = (uint32_t)shift;
    } 
    
    for(; i < length - 1; i++)
        tblPtr[(length - 2) - i] = 0;

    correct();
    
    return *this;
}

superInt& superInt::negate()
{
    for(size_t i = 0; i < length; i++)
        tblPtr[i] = ~tblPtr[i];
    return *this;
}

superInt& superInt::operator+=(const superInt& other)
{
    short this_sign = sign();
    short other_sign = other.sign();

    if(length < other.length) //change size if table size is smaller then the other one
        extend(other.length);

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

    while(i < length)
    {
        result = tblPtr[i] + (other_sign ? -1 : 0) + (carry ? 1 : 0);
        if(result < tblPtr[i])
            carry = true;
        else
            carry = false;
        tblPtr[i] = result;

        i++;
    }

    if(!(this_sign ^ other_sign) && (this_sign ^ sign()))
    {
        if(length == tblLength)
            changeTblSize(tblLength * 2);
        if(this_sign)
            tblPtr[length] = -1;
        else
            tblPtr[length] = 0;

        length++;
    }

    correct();
    return *this;
}

superInt& superInt::operator+=(int32_t num)
{
    short this_sign = sign();
    short num_sign;

    if(num < 0)
        num_sign = 1;
    else
        num_sign = 0;

    bool carry = false;
    uint32_t result;

    result = num + tblPtr[0];
    if(result < tblPtr[0])
        carry = true;
    else
        carry = false;
     tblPtr[0] = result;

    size_t i = 1;
    while(i < length)
    {
        result = tblPtr[i] + (num_sign ? -1 : 0) + (carry ? 1 : 0);
        if(result < tblPtr[i])
            carry = true;
        else
            carry = false;
        tblPtr[i] = result;

        i++;
    }

    if(!(this_sign ^ num_sign) && (this_sign ^ sign()))
    {
        if(length == tblLength)
            changeTblSize(tblLength * 2);
        if(this_sign)
            tblPtr[length] = -1;
        else
            tblPtr[length] = 0;

        length++;
    }

    correct();
    return *this;
}

superInt& superInt::operator-=(const superInt& other)
{
    short this_sign = sign();
    short other_sign = other.sign();

    if(length < other.length) //change size if table size is smaller then the other one
        extend(other.length);

    bool carry = false;
    uint32_t result;
    size_t i;
    for(i = 0; i < other.length; i++)
    {
        result = tblPtr[i] - other.tblPtr[i] - (carry ? 1 : 0);
        if(result > tblPtr[i])
            carry = true;
        else
            carry = false;

        tblPtr[i] = result;
    }

    while(i < length)
    {
        result = tblPtr[i] - (other_sign ? -1 : 0) - (carry ? 1 : 0);
        if(result > tblPtr[i])
            carry = true;
        else
            carry = false;
        tblPtr[i] = result;

        i++;
    }

    if((this_sign ^ other_sign) && (this_sign ^ sign()))
    {
        if(length == tblLength)
            changeTblSize(tblLength * 2);
        if(this_sign)
            tblPtr[length] = -1;
        else
            tblPtr[length] = 0;
         length++;
    }

    correct();
    return *this;
}

superInt& superInt::operator-=(int32_t num)
{
    short this_sign = sign();
    short num_sign;

    if(num < 0)
        num_sign = 1;
    else
        num_sign = 0;

    bool carry = false;
    uint32_t result;

    result = tblPtr[0] - num;
    if(result > tblPtr[0])
        carry = true;
    else
        carry = false;
     tblPtr[0] = result;

    size_t i = 1;
    while(i < length)
    {
        result = tblPtr[i] - (num_sign ? -1 : 0) - (carry ? 1 : 0);
        if(result > tblPtr[i])
            carry = true;
        else
            carry = false;
        tblPtr[i] = result;

        i++;
    }

    if((this_sign ^ num_sign) && (this_sign ^ sign()))
    {
        if(length == tblLength)
            changeTblSize(tblLength * 2);
        if(this_sign)
            tblPtr[length] = -1;
        else
            tblPtr[length] = 0;
         length++;
    }

    correct();
    return *this;

}

superInt& superInt::operator*=(const superInt& other)
{
    short this_sign = sign();
    short other_sign = other.sign();

    superInt result;

    result.extend(this->length + other.length);

    uint64_t product;
    uint32_t rest = 0;
    bool carry = false;
    uint32_t tmp;

    size_t i;
    for(i = 0; i < length; i++)
    {
        rest = 0;
        size_t j;
        for(j = 0; j < other.length; j++)
        {
            product = (uint64_t)tblPtr[i] * other.tblPtr[j] + rest;

            carry = false;
            tmp = result.tblPtr[i + j] + (uint32_t)product;
            if(tmp < result.tblPtr[i + j])
                carry = true;
            result.tblPtr[i + j] = tmp;
            for(size_t k = i + j + 1; carry == true && k < result.length; k++)
            {
                carry = false;
                tmp = result.tblPtr[k] + 1;
                if(tmp < result.tblPtr[k])
                    carry = true;
                result.tblPtr[k] = tmp;
            }

            rest = (uint32_t)(product >> 32);
        }

        for(;j + i < result.length; j++)
        {
            product = (uint64_t)tblPtr[i] * (other_sign ? 0xffffffff : 0) + rest;

            carry = false;
            tmp = result.tblPtr[i + j] + (uint32_t)product;
            if(tmp < result.tblPtr[i + j])
                carry = true;
            result.tblPtr[i + j] = tmp;
            for(size_t k = i + j + 1; carry == true && k < result.length; k++)
            {
                carry = false;
                tmp = result.tblPtr[k] + 1;
                if(tmp < result.tblPtr[k])
                    carry = true;
                result.tblPtr[k] = tmp;
            }

            rest = (uint32_t)(product >> 32);
        }
    }

    for(; i < result.length; i++)
    {
        rest = 0;
        size_t j;
        for(j = 0; j + i < result.length; j++)
        {
            carry = false;
            product = (uint64_t)other.tblPtr[j] * (this_sign ? 0xffffffff : 0)  + rest;

            tmp = result.tblPtr[i + j] + (uint32_t)product;
            if(tmp < result.tblPtr[i +j])
                carry = true;
            result.tblPtr[i + j] = tmp;
            for(size_t k = i + j + 1; carry == true && k < result.length; k++)
            {
                carry = false;
                tmp = result.tblPtr[k] + 1;
                if(tmp < result.tblPtr[k])
                    carry = true;
                result.tblPtr[k] = tmp;
            }

            rest = (uint32_t)(product >> 32);
        }
    }

    *this = result;
    correct();

    return *this;
}

/*superInt& superInt::operator*=(int32_t num)
{
    short this_sign = sign();
    short num_sign;

    if(num < 0)
        num_sign = 1;
    else
        num_sign = 0;

    superInt copy;

    copy.extend(this->length + 1);

    uint64_t product;
    uint32_t rest = 0;
    bool carry = false;

    size_t i;
    for(i = 0; i < length; i++)
    {
        rest = 0;

        product = (uint64_t)tblPtr[i] * num;
        copy.tblPtr[i] += (uint32_t)product;
        rest = (uint32_t)(product >> 32);

        size_t j = 1;
        for(;j + i < copy.length; j++)
        {
            product = (uint64_t)tblPtr[i] * (num_sign ? -1 : 0) + rest;

            copy.tblPtr[i + j] += (uint32_t)product;
            rest = (uint32_t)(product >> 32);
        }
    }

    product = (uint64_t)tblPtr[i] * num;
    copy.tblPtr[i + 1] += (uint32_t)product;

    *this = copy;
    correct();

    return *this;
}*/

superInt& superInt::operator/=(const superInt& other) throw(logic_error)
{
    short this_sign = sign();
    short other_sign = other.sign();

    if(other == 0)
        throw logic_error("Can't divide by 0");
    
    superInt copy(other);
    superInt result;
    superInt one(1);

    if(this_sign)
    {
        negate();
        operator+=(1);
    }
    if(other_sign)
    {
        copy.negate();
        copy += 1;
    }

    uint32_t shift = (length - copy.length + 1) * sizeof(uint32_t) * 8;
    copy <<= shift;
    one <<= shift;

    for(size_t i = 0; i <= shift; i++)
    {
        if(copy <= *this)
        {
            *this -= copy;
            result |= one; 
        }

        one >>= 1;
        copy >>= 1;
    }

    *this = result;
    if(this_sign ^ other_sign)
    {
        negate();
        operator+=(1);
    }

    return *this;
}

superInt& superInt::operator%=(const superInt& other) throw(logic_error)
{
    short this_sign = sign();
    short other_sign = other.sign();

    if(other == 0)
        throw logic_error("Can't divide by 0");
    
    superInt copy(other);
    superInt result;
    superInt one(1);

    if(this_sign)
    {
        negate();
        operator+=(1);
    }
    if(other_sign)
    {
        copy.negate();
        copy += 1;
    }

    uint32_t shift = (length - copy.length + 1) * sizeof(uint32_t) * 8;
    copy <<= shift;
    one <<= shift;

    for(size_t i = 0; i <= shift; i++)
    {
        if(copy <= *this)
        {
            *this -= copy;
            result |= one; 
        }

        one >>= 1;
        copy >>= 1;
    }

    if(this_sign)
    {
        negate();
        operator+=(1);
    }

    return *this;
}

superInt superInt::operator|(const superInt& other) const
{
    if(length > other.length)
        return superInt(*this) |= other;
    else
        return superInt(other) |= *this;
}

superInt superInt::operator&(const superInt& other) const
{
    if(length > other.length)
        return superInt(*this) &= other;
    else
        return superInt(other) &= *this;
}

superInt superInt::operator^(const superInt& other) const
{
    if(length > other.length)
        return superInt(*this) ^= other;
    else
        return superInt(other) ^= *this;
}

superInt superInt::operator<<(uint32_t num) const
{
    return superInt(*this) <<= num;
}

superInt superInt::operator>>(uint32_t num) const
{
    return superInt(*this) >>= num;
}

superInt superInt::operator+(const superInt& other) const
{
    if(length > other.length)
        return superInt(*this) += other;
    else
        return superInt(other) += *this;
}

superInt superInt::operator+(uint32_t num) const
{
    return superInt(*this) += num;
}

superInt superInt::operator-(const superInt& other) const
{
    if(length > other.length)
        return superInt(*this) -= other;
    else
        return superInt(other) -= *this;
}

superInt superInt::operator-(uint32_t num) const
{
    return superInt(*this) -= num;
}

superInt superInt::operator*(const superInt& other) const
{
    short this_sign = sign();
    short other_sign = other.sign();

    superInt result;

    result.extend(this->length + other.length);

    uint64_t product;
    uint32_t rest = 0;
     bool carry = false;
    uint32_t tmp;

    size_t i;
    for(i = 0; i < length; i++)
    {
        rest = 0;
        size_t j;
        for(j = 0; j < other.length; j++)
        {
            product = (uint64_t)tblPtr[i] * other.tblPtr[j] + rest;

            carry = false;
            tmp = result.tblPtr[i + j] + (uint32_t)product;
            if(tmp < result.tblPtr[i +j])
                carry = true;
            result.tblPtr[i + j] = tmp;
            for(size_t k = i + j + 1; carry == true && k < result.length; k++)
            {
                carry = false;
                tmp = result.tblPtr[k] + 1;
                if(tmp < result.tblPtr[k])
                    carry = true;
                result.tblPtr[k] = tmp;
            }

            rest = (uint32_t)(product >> 32);
        }

        for(;j + i < result.length; j++)
        {
            product = (uint64_t)tblPtr[i] * (other_sign ? 0xffffffff : 0) + rest;

            carry = false;
            tmp = result.tblPtr[i + j] + (uint32_t)product;
            if(tmp < result.tblPtr[i +j])
                carry = true;
            result.tblPtr[i + j] = tmp;
            for(size_t k = i + j + 1; carry == true && k < result.length; k++)
            {
                carry = false;
                tmp = result.tblPtr[k] + 1;
                if(tmp < result.tblPtr[k])
                    carry = true;
                result.tblPtr[k] = tmp;
            }

            rest = (uint32_t)(product >> 32);
        }
    }

    for(; i < result.length; i++)
    {
        rest = 0;
        size_t j;
        for(j = 0; j + i < result.length; j++)
        {
            product = (uint64_t)other.tblPtr[j] * (this_sign ? 0xffffffff : 0)  + rest;

            carry = false;
            tmp = result.tblPtr[i + j] + (uint32_t)product;
            if(tmp < result.tblPtr[i +j])
                carry = true;
            result.tblPtr[i + j] = tmp;
            for(size_t k = i + j + 1; carry == true && k < result.length; k++)
            {
                carry = false;
                tmp = result.tblPtr[k] + 1;
                if(tmp < result.tblPtr[k])
                    carry = true;
                result.tblPtr[k] = tmp;
            }

            rest = (uint32_t)(product >> 32);
        }
    }

    result.correct();

    return result;
}

superInt superInt::operator/(const superInt& other) const throw(logic_error)
{
    
    return superInt(*this) /= other;
}

superInt superInt::operator%(const superInt& other) const throw(logic_error)
{
    
    return superInt(*this) %= other;
}

superInt superInt::operator~() const
{
    return superInt(*this).negate();
}

int superInt::compare(const superInt& other) const
{
    short this_sign = this->sign();
    short other_sign = other.sign();

    if(this_sign ^ other_sign || length > other.length)
        return (this_sign ? -1 : 1);
    if(length < other.length)
        return (this_sign ? 1 : -1);
    for(int i = length - 1; i >= 0; i--)
    {
        if(tblPtr[i] > other.tblPtr[i])
            return (this_sign ? -1 : 1);
        if(tblPtr[i] < other.tblPtr[i])
            return (this_sign ? 1 : -1);
    }
    return 0;
}

bool superInt::operator<(const superInt& other) const
{
    return (compare(other) == -1);
}

bool superInt::operator>(const superInt& other) const
{
    return (compare(other) == 1);
}

bool superInt::operator<=(const superInt& other) const
{
    return (compare(other) <= 0);
}

bool superInt::operator>=(const superInt& other) const
{
    return (compare(other) >= 0);
}

bool superInt::operator==(const superInt& other) const
{
    return (compare(other) == 0);
}

bool superInt::operator!=(const superInt& other) const
{
    return (compare(other) != 0);
} 

superInt superInt::operator-() const
{
    return superInt(*this).negate() += 1;
}

ostream& operator<<(std::ostream& out,const superInt& s)
{
    return out<<s.toString();
}
