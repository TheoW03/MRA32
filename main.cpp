#include <iostream>
#include <vector>
#include "src/Frontend/Lexxer.h"
#include "src/Frontend/Parser.h"
#include "src/emulator/emulator.h"

using namespace std;

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 * ARM support
 *
 * MOV: 1101 {opcode}{Rd}{R1}{R2} (for all of them)
 * ADD: 0100
 * SUB: 0010
 * MUL: 1001
 * AND: 0000
 * OR: 1100
 *
 *
 * B:{cond: 0 if true} {branch addr}
 *
 * EQ: 0000
 * NE: 0001
 * LT: 1011
 * GT: 1100
 * GE: 1010
 * LE: 1101
 * Registers are R
 */
// uint32_t leftRotate(int n, unsigned int d)
// {
//     /* In n<<d, last d bits are 0. To put first 3 bits of n
//        at last, do bitwise or of n<<d with n >>(INT_BITS -
//        d) */
//     return (n << d) | (n >> (32 - d));
// }

// /*Function to right rotate n by d bits*/
// uint32_t rightRotate(int n, unsigned int d)
// {
//     /* In n>>d, first d bits are 0. To put last 3 bits of at
//             first, do bitwise or of n>>d with n <<(INT_BITS
//        - d) */
//     return (n >> d) | (n << (32 - d));
// }
int main(int argc, char const *argv[])
{
    // int n = 261120;
    // int d = 22;
    // cout << n << endl;
    emulate(parse(lex(readFile("test.s"))));
    // cout << leftRotate(n, d) << endl;
    // int i = 0;
    // uint32_t a = 258;
    // do
    // {
    //     i += 2;
    //     a = leftRotate(n, i);
    //     cout << a << endl;
    //     cout << i << endl;
    //     cout << (i < 32 || a > 256) << endl;
    // } while (i < 32 && a > 256);

    // cout << rightRotate(a, i) << endl;
    // printList(lex(readFile("test.s")));
    // lex(readFile("test.s"));
    // parse(lex(readFile("test.s")));
    return 0;
}
