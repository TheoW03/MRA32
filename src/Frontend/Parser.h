#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "../../src/Frontend/Lexxer.h"

using namespace std;

#ifndef INSTRTUCTIPN_TYPE_H
#define INSTRTUCTIPN_TYPE_H
enum class InstructionType
{
    CONDITIONAL,
    DATA_PROCESSING
};
#endif

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

struct Instructions
{
    uint16_t opcode;
    int condition;
    int I;
    int S;
    int Rn;
    int Rd;
    int OP2;
    InstructionType instructionType;
};

#endif
vector<Instructions *> parse(vector<Tokens> tokens);