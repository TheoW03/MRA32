#include <iostream>
#include <map>
#include <vector>
#include <map>
#include <string>
#include "../../src/Frontend/Lexxer.h"
#include "../../src/Frontend/Parser.h"

using namespace std;
enum class dataProcessingInstructions
{
    ADD,
    SUB,
    MUL,
    MOV
};
int PC = 0;
template <typename Base, typename T>
bool instanceof (T * ptr)
{
    return dynamic_cast<Base>(ptr);
}
Instructions::~Instructions()
{
    // implementation of the destructor goes here
}
struct EncodedInstructions
{
    uint32_t encodedInstruction;
    InstructionType instructionType;
};
uint32_t handle_multiply(Multiply *instruction)
{
    return (instruction->condition << 28) | (instruction->A << 21);
}
uint32_t handle_data_processing(DataProcessing *instruction)
{

    return (instruction->condition << 28) | (instruction->I << 25) | (instruction->opcode << 21) | (instruction->Rn << 16) | (instruction->Rd << 12) | instruction->OP2;
}
vector<EncodedInstructions *> encode(vector<Instructions *> InstructionsList)
{
    vector<EncodedInstructions *> encoded_instructions;

    for (int i = 0; i < InstructionsList.size(); i++)
    {

#pragma region DATA_PROCESSING encoding
        if (instanceof <DataProcessing *>(InstructionsList[i]))
        {
            DataProcessing *pd = dynamic_cast<DataProcessing *>(InstructionsList[i]);
            if (pd == nullptr)
            {
                cout << "null" << to_string(i) << endl;
                exit(0);
            }
            EncodedInstructions *ei = new EncodedInstructions;

            uint32_t instruction = handle_data_processing(pd);
            ei->encodedInstruction = instruction;
            ei->instructionType = InstructionType::DATA_PROCESSING;
            encoded_instructions.push_back(ei);
        }
#pragma endregion

#pragma region MULTIPLY
        else if (instanceof <Multiply *>(InstructionsList[i]))
        {
            Multiply *pd = dynamic_cast<Multiply *>(InstructionsList[i]);

            EncodedInstructions *ei = new EncodedInstructions;

            ei->encodedInstruction = handle_multiply(pd);
            ei->instructionType = InstructionType::MULTIPLY;
            encoded_instructions.push_back(ei);

            // encode multiply instructions
        }
#pragma endregion
    }
    return encoded_instructions;
}
void emulate(vector<Instructions *> InstructionsList)
{
    vector<EncodedInstructions *> encodedInstrtions = encode(InstructionsList);
    uint32_t registersList[16];
    for (int i = 0; i < 16; i++)
    {
        registersList[i] = 0;
    }
    for (int i = 0; i < encodedInstrtions.size(); i++)
    {

#pragma region DATA_PROCESSING decoding
        registersList[15] += 4;

        if (encodedInstrtions[i]->instructionType == InstructionType::DATA_PROCESSING)
        {

            map<int, dataProcessingInstructions> instructionMap;
            instructionMap[0x4] = dataProcessingInstructions::ADD;
            instructionMap[0x2] = dataProcessingInstructions::SUB;
            // instructionMap[0x9] = dataProcessingInstructions::MUL;
            instructionMap[0xD] = dataProcessingInstructions::MOV;

            uint32_t instruction = encodedInstrtions[i]->encodedInstruction;
            uint32_t condition = (instruction >> 28);
            uint32_t iBit = (instruction >> 25) & 1;
            uint32_t opcode = (instruction >> 21) & 0x0F;
            uint32_t rd = (instruction >> 12) & 0x0F;
            uint32_t rn = (instruction >> 16) & 0x0F;
            uint32_t immediate = instruction & 0xFFF;

            dataProcessingInstructions instructionTypes = instructionMap[opcode];
            if (rd > 12)
            {
                return;
            }
            if (condition != 0xE)
            {
                // execute condition
            }
            if (instructionTypes == dataProcessingInstructions::ADD) // ADD
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
            else if (instructionTypes == dataProcessingInstructions::SUB) // SUB
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

            else if (instructionTypes == dataProcessingInstructions::MOV) // MOV
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

#pragma region MULTIPLY decoding
        else if (encodedInstrtions[i]->instructionType == InstructionType::MULTIPLY)
        {
            uint32_t instruction = encodedInstrtions[i]->encodedInstruction;
            uint32_t condition = (instruction >> 28);
            uint32_t aBit = (instruction >> 21);
            if (condition != 0xE)
            {
                // execute condition
            }
            if (aBit == 1)
            {
                // MULA
            }
            else
            {
                cout << "mul" << endl;
                // MUL
            }
        }
#pragma endregion
    }
    for (int i = 0; i < 16; i++)
    {
        cout << "R" << i << " value: " << registersList[i] << endl;
    }
}