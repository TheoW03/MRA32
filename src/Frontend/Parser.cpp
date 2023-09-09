#include <iostream>
#include <map>
#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <sstream>

#include "../../src/Frontend/Lexxer.h"

using namespace std;

enum class InstructionType
{
    CONDITIONAL,
    DATA_PROCESSING,
    MULTIPLY,
    BRANCH,
    JMPBRANCH
};

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
// struct Instructions
// {
//     uint16_t opcode;
//     int condition;
//     int I;
//     int S;
//     int Rn;
//     int Rd;
//     int OP2;
//     InstructionType instructionType;
// };

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
        current = new Tokens;
        current = t;
        tokens.erase(tokens.begin());

        return t;
    }

    return nullptr;
}
int handleRegisters(Tokens *reg)
{

    string substring = reg->buffer.substr(1, reg->buffer.length());
    return stoi(substring);
}
Instructions *handleBranch(vector<Tokens> &tokens)
{
    JMPBranch *a = new JMPBranch;
    a->condition = 0xE;
    a->L = 0;
    if (matchAndRemove(tokens, type::WORD) != nullptr)
    {

        a->branchName = current->buffer;
    }
    else
    {
        cout << "branch name required" << endl;
        exit(EXIT_FAILURE);
    }
    return a;
}
Instructions *handleMul(vector<Tokens> &tokens)
{
    Multiply *a = new Multiply;
    a->condition = 0xE;

    a->A = 0;
    a->Rd = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA);
    a->Rn = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA); // write abstraction over this
    a->Rm = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    return a;
}
Instructions *handleMulA(vector<Tokens> &tokens)
{
    Multiply *a = new Multiply;
    a->condition = 0xE;

    a->A = 1;
    a->Rd = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA);
    a->Rn = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA); // write abstraction over this
    a->Rm = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA); // write abstraction over this
    a->Rs = handleRegisters(matchAndRemove(tokens, type::REGISTER));

    return a;
}
Instructions *handle2Operands(vector<Tokens> &tokens, uint16_t opCode)
{
    DataProcessing *a = new DataProcessing;
    a->condition = 0xE;
    a->opcode = opCode;
    a->S = 0;
    a->Rd = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA);
    a->Rn = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    matchAndRemove(tokens, type::COMMA); // write abstraction over this
    if (matchAndRemove(tokens, type::NUMBER) != nullptr)
    {
        a->I = 1;
        a->OP2 = stoi(current->buffer);
    }
    else
    {
        a->I = 0;
        a->OP2 = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    }

    return a;
}
Instructions *handle1Operand(vector<Tokens> &tokens, uint16_t opCode)
{

    DataProcessing *a = new DataProcessing;
    a->condition = 0xE;

    a->opcode = opCode;
    a->S = 1;

    a->Rd = handleRegisters(matchAndRemove(tokens, type::REGISTER));

    a->Rn = a->Rd;

    matchAndRemove(tokens, type::COMMA);

    if (matchAndRemove(tokens, type::NUMBER) != nullptr)
    {
        a->I = 1;
        a->OP2 = stoi(current->buffer);
    }
    else
    {
        a->I = 0;
        a->OP2 = handleRegisters(matchAndRemove(tokens, type::REGISTER));
    }
    return a;
}
// void RemoveEOLS(vector<Tokens> &list)
// {
//     while (true)
//     {
//         Tokens *e = matchAndRemove(list, type::END_OF_LINE);

//         if (e == nullptr)
//         {
//             return;
//         }
//     }
// }
vector<Instructions *> parse(vector<Tokens> tokens)
{
    vector<Instructions *> a;

    while (matchAndRemove(tokens, type::END_OF_PROGRAM) == nullptr)
    {

        Tokens *Instructiona = matchAndRemove(tokens, type::INSTRUCTION);

        if (Instructiona != nullptr)
        {
            if (Instructiona->buffer == "ADD")
            {
                Instructions *in = handle2Operands(tokens, 0x4);
                // in->instructionType = InstructionType::DATA_PROCESSING;

                a.push_back(in);
            }
            else if (Instructiona->buffer == "SUB")
            {
                Instructions *in = handle2Operands(tokens, 0x2);
                // in->instructionType = InstructionType::DATA_PROCESSING;

                a.push_back(in);
            }
            else if (Instructiona->buffer == "MUL" || Instructiona->buffer == "MULA")
            {
                // Multiply *mul = new Multiply;
                if (Instructiona->buffer == "MUL")
                {
                    Instructions *in = handleMul(tokens);
                    a.push_back(in);
                }
                else
                {
                    Instructions *in = handleMulA(tokens);
                    a.push_back(in);
                }

                // in->instructionType = InstructionType::MULTIPLY;
            }
            else if (Instructiona->buffer == "AND")
            {
                Instructions *in = handle2Operands(tokens, 0x0);
                // in->instructionType = InstructionType::DATA_PROCESSING;

                a.push_back(in);
            }
            else if (Instructiona->buffer == "MOV")
            {

                Instructions *in = handle1Operand(tokens, 0xD);
                // in->instructionType = InstructionType::DATA_PROCESSING;
                a.push_back(in);
            } //..so on for every possible instruction
            else if (Instructiona->buffer == "B")
            {

                Instructions *in = handleBranch(tokens);

                a.push_back(in);
            }
        }
        else
        {

            if (matchAndRemove(tokens, type::WORD) != nullptr)
            {
                Branch *branch = new Branch;

                branch->branchName = current->buffer;

                a.push_back(branch);
            }
        }
    }
    cout << "parsing compete" << endl;
    return a;
}