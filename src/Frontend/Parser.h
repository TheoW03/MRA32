#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "../../src/Frontend/Lexxer.h"

using namespace std;

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
};

#endif
vector<Instructions *> parse(vector<Tokens> tokens);