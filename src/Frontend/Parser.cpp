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
int handle_condition(vector<Tokens> &tokens)
{
    map<string, int> conditions;
    conditions["EQ"] = 0b0000;
    conditions["NE"] = 0b0001;
    conditions["GT"] = 0b1100;
    conditions["LT"] = 0b1011;
    conditions["GE"] = 0b1010;
    conditions["LE"] = 0b1101;

    if (matchAndRemove(tokens, type::CONDITION) == nullptr)
    {
        return 0xE;
    }
    else
    {

        return conditions[current->buffer]; // hashmap of condition codes here :3
    }
}
int handleRegisters(Tokens *reg)
{

    string substring = reg->buffer.substr(1, reg->buffer.length());
    return stoi(substring);
}
/**
 * @brief B branch
 *
 * @param tokens
 * @return Instructions*
 */
Instructions *handleBranch(vector<Tokens> &tokens)
{
    JMPBranch *a = new JMPBranch;
    a->condition = handle_condition(tokens);
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
/**
 * @brief MUL, Rd, Rn, Rm
 *
 * @param tokens
 * @return Instructions*
 */
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
/**
 * @brief MULA rd, rn, rm, rs
 *
 * @param tokens
 * @return Instructions*
 */
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
/**
 * @brief designed for data processing
 *
 * @param tokens
 * @param opCode
 * @return Instructions*
 */
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
/**
 * @brief designed for mov
 *
 * @param tokens
 * @param opCode
 * @return Instructions*
 */
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
/**
 * @brief parser :pet: :3
 *
 * @param tokens
 * @return vector<Instructions *>
 */
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
            else if (Instructiona->buffer == "CMP")
            {
                Instructions *in = handle1Operand(tokens, 0xA);
                // in->instructionType = InstructionType::DATA_PROCESSING;
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