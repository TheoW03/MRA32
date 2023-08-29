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
struct EncodedInstructions
{
    uint32_t encodedInstruction;
    InstructionType instructionType;
};
uint32_t handle_with_rgsiter(Instructions *instruction)
{
    return (instruction->opcode << 21) | (instruction->I << 25) | (instruction->Rd << 12) | (instruction->OP2 << 8) | instruction->Rn;
}
uint32_t handle_with_number(Instructions *instruction)
{
    return (instruction->opcode << 21) | (instruction->Rn << 16) | (instruction->Rd << 12) | instruction->OP2;
}
vector<EncodedInstructions *> encode(vector<Instructions *> InstructionsList)
{
    vector<EncodedInstructions *> encoded_instructions;

    for (int i = 0; i < InstructionsList.size(); i++)
    {
        EncodedInstructions *ei = new EncodedInstructions;
        if (InstructionsList[i]->instructionType == InstructionType::DATA_PROCESSING)
        {
            if (InstructionsList[i]->I == 1)
            {

                uint32_t instruction = handle_with_number(InstructionsList[i]);
                ei->encodedInstruction = instruction;
                ei->instructionType = InstructionType::DATA_PROCESSING;
                encoded_instructions.push_back(ei);
            }
            else
            {
                uint32_t instruction = handle_with_number(InstructionsList[i]);
                ei->encodedInstruction = instruction;
                ei->instructionType = InstructionType::DATA_PROCESSING;
                encoded_instructions.push_back(ei);
            }
        }
    }
    return encoded_instructions;
}
void emulate(vector<Instructions *> InstructionsList)
{
    vector<EncodedInstructions *> encodedInstrtions = encode(InstructionsList);
    for (int i = 0; i < encodedInstrtions.size(); i++)
    {
        if (encodedInstrtions[i]->instructionType == InstructionType::DATA_PROCESSING)
        {
            uint32_t instruction = encodedInstrtions[i]->encodedInstruction;
            uint8_t opcode = (instruction >> 21) & 0x0F;
            uint8_t rd = (instruction >> 12) & 0x0F;
            uint8_t rn = (instruction >> 16) & 0x0F;
            uint32_t iBit = (instruction >> 25) & 1;
            uint16_t immediate = instruction & 0xFFF;
            if (opcode == 0x4)
            {
                // perform the add operator
            }
        }
    }
}