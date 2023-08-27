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
    virtual ~Instructions();
};
struct conditions : public Instructions
{
    int Rd;
    int Rx;
};
struct AddInstruct : public Instructions
{
    int Rd;
    int R1;
    int R2;
};
#endif
vector<Instructions *> parse(vector<Tokens> tokens);