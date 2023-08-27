#include <iostream>
#include <map>
#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <sstream>

#include "../../src/Frontend/Lexxer.h"

using namespace std;

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
    int OP1;
    int OP2;
};
Tokens *current;
Tokens *matchAndRemove(vector<Tokens> &tokens, type typeT)
{
    if (tokens.empty())
    {
        return nullptr;
    }
    if (tokens[0].id == typeT)
    {

        Tokens *t = new Tokens(tokens[0]);
        current = t;
        tokens.erase(tokens.begin());
        return t;
    }
    // cout << "unmatched \n";
    return nullptr;
}
int handleRegisters(Tokens *reg)
{

    string substring = reg->buffer.substr(1, reg->buffer.length());
    return stoi(substring);
}
Instructions *handleAdd(vector<Tokens> &tokens)
{
    AddInstruct *a = new AddInstruct;
    a->opcode = 0x4;
    cout << a->opcode << endl;
    matchAndRemove(tokens, type::COMMA);
    a->Rd = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA);
    a->OP1 = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA); //write abstraction over this
    a->OP1 = handleRegisters(matchAndRemove(tokens, type::REGISTER));

    return a;
}
void RemoveEOLS(vector<Tokens> &list)
{
    while (true)
    {
        Tokens *e = matchAndRemove(list, type::END_OF_LINE);

        if (e == nullptr)
        {
            return;
        }
    }
}
vector<Instructions *> parse(vector<Tokens> tokens)
{
    vector<Instructions *> a;

    while (matchAndRemove(tokens, type::END_OF_PROGRAM) == nullptr)
    {
        if (tokens.empty())
        {
            return a;
        }
        RemoveEOLS(tokens);

        Tokens *Instructiona = matchAndRemove(tokens, type::INSTRUCTION);
        if (Instructiona != nullptr)
        {
            if (Instructiona->buffer == "ADD")
            {
                a.push_back(handleAdd(tokens));
            }
            else if (Instructiona->buffer == "SUB")
            {
            }
            else if (Instructiona->buffer == "MUL")
            {
            }
            else if (Instructiona->buffer == "MOV")
            {
            }
        }
        RemoveEOLS(tokens);
    }
    return a;
}