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
int main(int argc, char const *argv[])
{
    // emulate(parse(lex(readFile("test.s"))));
    printList(lex(readFile("test.s")));
    parse(lex(readFile("test.s")));
    return 0;
}
