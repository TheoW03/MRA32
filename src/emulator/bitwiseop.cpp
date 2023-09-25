#include <iostream>

using namespace std;

uint32_t leftRotate(int n, unsigned int d)
{

    return (n << d) | (n >> (32 - d));
}

uint32_t rightRotate(int n, unsigned int d)
{
    return (n >> d) | (n << (32 - d));
}
uint32_t add(uint32_t a, uint32_t b, int &carryCond)
{
    while (b != 0)
    {
        int carry = a & b;
        if (carry != 0)
        {
            carryCond = 1;
        }
        a = a ^ b;
        b = carry << 1;
    }

    return a;
}

uint32_t sub(uint32_t a, uint32_t b, int &carryCond)
{
    while (b != 0)
    {
        uint32_t carry = (~a) & b;
        if (carry != 0)
        {
            carryCond = 1;
        }
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}
uint32_t mul(uint32_t a, uint32_t b)
{
    uint32_t result = 0;
    while (a > 0 && b > 0)
    {
        if (a & 1)
        {                // if Least significant bit exists
            result += b; // add by shifted left
        }
        a >>= 1;
        b <<= 1; // next bit
    }
    return result;
}