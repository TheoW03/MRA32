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
    MULTIPLY,
    BRANCH,
    JMPBRANCH
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
    int S;
    int Rn;
    int Rs;
    int Rd;
    int Rm;
};
struct Branch : public Instructions
{
    int PCLocation;
    string branchName;
};
struct JMPBranch : public Instructions
{
    int condition;
    int L;
    string branchName;
};

#endif
vector<Instructions *> parse(vector<Tokens> tokens);