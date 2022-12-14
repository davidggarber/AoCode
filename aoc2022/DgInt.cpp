#include <iostream>

#define BI_SIZE 4

using namespace std;

class BigInt
{
    uint64_t buf[BI_SIZE];
public:
    BigInt(int x)
    {
        memset(buf, 0, sizeof(buf));
        buf[0] = x;
    }
    BigInt(const BigInt& b)
    {
        memcpy(buf, b.buf, sizeof(buf));
    }
    const BigInt& operator=(const BigInt& b)
    {
        memcpy(buf, b.buf, sizeof(buf));
        return *this;
    }

    inline static bool HighBit(uint64_t n)
    {
        return (n >> 63) != 0;
    }
    bool Add(BigInt b)
    {
        uint64_t carry = 0;
        for (int i = 0; i < BI_SIZE; i++)
        {
            buf[i] += carry;
            carry = (HighBit(buf[i]) && HighBit(b.buf[i])) ? 1 : 0;
            buf[i] += b.buf[i];
        }
        return carry == 0;  // true == !overflow
    }
    inline static int BitCount()
    {
        return 64 * BI_SIZE;
    }
    int BitAt(int index)
    {
        int i = index / 64;
        int j = index % 64;
        return (buf[i] >> j) & 0x1;
    }

    bool ShiftL(int shift)
    {
        if (shift == 0)
            return true;
        uint64_t carry = 0;
        for (int i = 0; i < BI_SIZE; i++)
        {
            uint64_t next = buf[i] >> (64 - shift);
            buf[i] <<= shift;
            buf[i] |= carry;
            carry = next;
        }
        return carry == 0;  // true == !overflow
    }
    bool Mult(BigInt b)
    {
        bool valid = true;
        BigInt prod(0);
        for (int i = 0; i < BitCount(); i++)
        {
            if (b.BitAt(i))
            {
                BigInt part(*this);
                valid &= part.ShiftL(i);
                valid &= prod.Add(part);
            }
        }
        *this = prod;
        return valid;
    }
    void Div3()
    {
        BigInt quotient(0);
        uint64_t carry = 0;
        for (int i = BI_SIZE * 2 - 1; i >= 0; i--)
        {
            uint64_t part = buf[i / 2];
            if (i % 2 == 1)
                part >>= 32;
            else
                part &= (1ULL << 32) - 1;
            part |= carry << 32;
            uint64_t div = part / 3;
            carry = part % 3;
            if (i % 2 == 1)
                div <<= 32;
            quotient.buf[i / 2] |= div;
        }
        *this = quotient;
    }
    bool Divisible(int by)
    {
        uint64_t carry = 0;
        for (int i = BI_SIZE * 2 - 1; i >= 0; i--)
        {
            uint64_t part = buf[i / 2];
            if (i % 2 == 0)
                part >>= 32;
            else
                part &= (1ULL << 32) - 1;
            part |= carry << 32;
            uint64_t div = part / by;
            carry = part % by;
        }
        return carry == 0;
    }
};
