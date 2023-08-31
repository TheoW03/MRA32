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
    return (instruction->I << 25) | (instruction->opcode << 21) | (instruction->Rd << 12) | (instruction->OP2 << 8) | instruction->Rn;
}
uint32_t handle_with_number(Instructions *instruction)
{
    if (instruction == nullptr)
    {
        cout << "null" << endl;
    }
    return (instruction->I << 25) | (instruction->opcode << 21) | (instruction->Rn << 16) | (instruction->Rd << 12) | instruction->OP2;
}
vector<EncodedInstructions *> encode(vector<Instructions *> InstructionsList)
{
    vector<EncodedInstructions *> encoded_instructions;

    for (int i = 0; i < InstructionsList.size(); i++)
    {

        #pragma region DATA_PROCESSING encoding
        if (InstructionsList[i]->instructionType == InstructionType::DATA_PROCESSING)
        {
            if (InstructionsList[i]->I == 1)
            {
                EncodedInstructions *ei = new EncodedInstructions;

                uint32_t instruction = handle_with_number(InstructionsList[i]);
                ei->encodedInstruction = instruction;
                ei->instructionType = InstructionType::DATA_PROCESSING;
                encoded_instructions.push_back(ei);
            }
            else
            {
                EncodedInstructions *ei = new EncodedInstructions;

                uint32_t instruction = handle_with_number(InstructionsList[i]);
                ei->encodedInstruction = instruction;
                ei->instructionType = InstructionType::DATA_PROCESSING;
                encoded_instructions.push_back(ei);
            }
        }
        #pragma endregion
    }
    return encoded_instructions;
}
void emulate(vector<Instructions *> InstructionsList)
{
    vector<EncodedInstructions *> encodedInstrtions = encode(InstructionsList);
    uint32_t registersList[10];
    for (int i = 0; i < 10; i++)
    {
        registersList[i] = 0;
    }
    for (int i = 0; i < encodedInstrtions.size(); i++)
    {

#pragma region DATA_PROCESSING decoding
        if (encodedInstrtions[i]->instructionType == InstructionType::DATA_PROCESSING)
        {

            uint32_t instruction = encodedInstrtions[i]->encodedInstruction;
            uint32_t iBit = (instruction >> 25) & 1;
            uint32_t opcode = (instruction >> 21) & 0x0F;
            uint32_t rd = (instruction >> 12) & 0x0F;
            uint32_t rn = (instruction >> 16) & 0x0F;
            uint32_t immediate = instruction & 0xFFF;
            if (opcode == 4) // ADD
            {
                if (iBit == 1)
                {
                    registersList[rd] = registersList[rn] + immediate;
                }
                else
                {
                    registersList[rd] = registersList[rn] + registersList[immediate];
                }
            }
            else if (opcode == 2) // SUB
            {
                if (iBit == 1)
                {
                    registersList[rd] = registersList[rn] - immediate;
                }
                else
                {
                    registersList[rd] = registersList[rn] - registersList[immediate];
                }
            }
            else if (opcode == 9) // MUL
            {
                if (iBit == 1)
                {
                    registersList[rd] = registersList[rn] * immediate;
                }
                else
                {
                    registersList[rd] = registersList[rn] * registersList[immediate];
                }
            }
            else if (opcode == 13) // MOV
            {
                if (iBit == 1)
                {
                    registersList[rd] = immediate;
                }
                else
                {
                    registersList[rd] = registersList[immediate];
                }
            }
        }
#pragma endregion
    
    }
    for (int i = 0; i < 10; i++)
    {
        cout << "R" << i << " value: " << registersList[i] << endl;
    }
}