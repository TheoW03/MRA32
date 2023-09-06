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
    DATA_PROCESSING,
    MULTIPLY
};
#endif

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

struct Instructions
{
    virtual ~Instructions();
};
struct DataProcessing : public Instructions
{
    uint16_t opcode;
    int condition;
    int I;
    int S;
    int Rn;
    int Rd;
    int OP2;
};
struct Multiply : public Instructions
{
    int condition;
    int A;
    int Rn;
    int Rs;
    int Rd;
    int Rm;
};

#endif
vector<Instructions *> parse(vector<Tokens> tokens);