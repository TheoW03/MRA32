#include <iostream>
#include <map>
#include <vector>
#include <map>
#include <string>
#include "../../src/Frontend/Lexxer.h"
#include "../../src/Frontend/Parser.h"

using namespace std;

int PC = 0;
template <typename Base, typename T>
bool instanceof (T * ptr)
{
    return dynamic_cast<Base>(ptr);
}

uint32_t handle_with_rgsiter(Instructions *instruction)
{
    return (instruction->opcode << 21) | (instruction->I << 25) | (instruction->Rd << 12) | (instruction->OP2 << 8) | instruction->Rn;
}
uint32_t handle_with_number(Instructions *instruction)
{
    return (instruction->opcode << 21) | (instruction->Rn << 16) | (instruction->Rd << 12) | instruction->OP2;
}
vector<uint32_t> encode(vector<Instructions *> InstructionsList)
{
    vector<uint32_t> encoded_instructions;
    for (int i = 0; i < InstructionsList.size(); i++)
    {
        if (InstructionsList[i]->I == 1)
        {
            uint32_t instruction = handle_with_number(InstructionsList[i]);
            encoded_instructions.push_back(instruction);
            cout << instruction << endl;
        }
        else
        {
            uint32_t instruction = handle_with_number(InstructionsList[i]);
            encoded_instructions.push_back(instruction);
            cout << instruction << endl;
        }
    }
    return encoded_instructions;
}
void emulate(vector<Instructions *> InstructionsList)
{
    vector<uint32_t> encodedInstrtions = encode(InstructionsList);
    for (int i = 0; i < encodedInstrtions.size(); i++)
    {
        uint32_t instruction = encodedInstrtions[i];
        uint8_t opcode = (instruction >> 21) & 0x0F;
        uint8_t rd = (instruction >> 12) & 0x0F;
        uint8_t rn = (instruction >> 16) & 0x0F;
        uint32_t iBit = (instruction >> 25) & 1;
        uint16_t immediate = instruction & 0xFFF;
    }
}