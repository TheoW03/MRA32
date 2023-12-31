#include <iostream>
#include <map>
#include <vector>
#include <map>
#include <string>
#include "../../src/Frontend/Lexxer.h"
#include "../../src/Frontend/Parser.h"
#include "../../src/emulator/bitwiseop.h"
using namespace std;
enum class dataProcessingInstructions
{
    ADD,
    SUB,
    MOV,
    CMP,
    ORR,
    AND,
    ADC,
    SBC
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
struct Memory
{
    vector<EncodedInstructions *> instructions;
    vector<uint16_t> memory_allocate;
};

#pragma region ENCODING_FUNCTIONS

uint32_t encode_immediate(uint32_t n)
{
    int i = 0;
    uint32_t a = 258;
    do
    {
        i += 2;
        a = leftRotate(n, i);

    } while (i < 32 && a > 256);
    if (n < 4096)
    {
        return 0;
    }
    if (i >= 32 && a > 256)
    {
        return 0;
    }
    return ((i / 2) << 8) | a;
}
uint32_t handle_multiply(Multiply *instruction)
{

    return (instruction->condition << 28) | (instruction->A << 21) | (instruction->S << 20) | (instruction->Rd << 16) | (instruction->Rn << 12) | (instruction->Rs << 8) | instruction->Rm;
}
uint32_t encode_Branch(JMPBranch *instruction, int offset)
{
    return (instruction->condition << 28) | (instruction->L << 23) | offset;
}

uint32_t handle_data_processing(DataProcessing *instruction)
{
    int b = 0;
    if (instruction->I == 1)
    {
        if (encode_immediate(instruction->OP2) != 0)
        {
            b = 1;
            return (instruction->condition << 28) | (b << 27) | (instruction->I << 25) | (instruction->opcode << 21) | (instruction->S << 20) | (instruction->Rn << 16) | (instruction->Rd << 12) | encode_immediate(instruction->OP2);
        }
    }

    return (instruction->condition << 28) | (b << 27) | (instruction->I << 25) | (instruction->opcode << 21) | (instruction->S << 20) | (instruction->Rn << 16) | (instruction->Rd << 12) | instruction->OP2;
}

vector<EncodedInstructions *> encode(vector<Instructions *> InstructionsList)
{
    map<string, int> branchMap;
    branchMap["."] = -1;
    for (int i = 0; i < InstructionsList.size(); i++)
    {
        if (instanceof <Branch *>(InstructionsList[i]))
        {
            Branch *pd = dynamic_cast<Branch *>(InstructionsList[i]);
            branchMap[pd->branchName] = i;
        }
    }
    vector<EncodedInstructions *> encoded_instructions;

    for (int i = 0; i < InstructionsList.size(); i++)
    {

#pragma region DATA_PROCESSING encoding
        if (instanceof <DataProcessing *>(InstructionsList[i]))
        {
            DataProcessing *pd = dynamic_cast<DataProcessing *>(InstructionsList[i]);
            if (pd == nullptr)
            {
                cout << "null " << to_string(i) << endl;
                exit(0);
            }
            EncodedInstructions *ei = new EncodedInstructions;

            uint32_t instruction = handle_data_processing(pd);
            ei->encodedInstruction = instruction;
            ei->instructionType = InstructionType::DATA_PROCESSING;
            encoded_instructions.push_back(ei);
        }
#pragma endregion

#pragma region MULTIPLY encoding
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

#pragma region BRANCH encoding
        else if (instanceof <Branch *>(InstructionsList[i]))
        {
            EncodedInstructions *ei = new EncodedInstructions;

            ei->encodedInstruction = 0;
            ei->instructionType = InstructionType::BRANCH;
            encoded_instructions.push_back(ei);
        }
        else if (instanceof <JMPBranch *>(InstructionsList[i]))
        {
            JMPBranch *pd = dynamic_cast<JMPBranch *>(InstructionsList[i]);

            EncodedInstructions *ei = new EncodedInstructions;
            ei->encodedInstruction = encode_Branch(pd, branchMap[pd->branchName]);
            ei->instructionType = InstructionType::JMPBRANCH;
            encoded_instructions.push_back(ei);
        }
#pragma endregion
    }

    cout << "encoding instructions complete" << endl;
    return encoded_instructions;
}
#pragma endregion

uint16_t Z_FLAG = 0;
uint16_t C_FLAG = 0;
uint16_t N_FLAG = 0;
uint16_t V_FLAG = 0;
int cycles = 0;
int total_cycles = 0;
void editCPSRFlags(uint32_t calculation)
{
    N_FLAG = ((calculation >> 31) & 1);
    Z_FLAG = (bool)!calculation;
    if (calculation > 2147483648)
    {
        V_FLAG = 1;
    }
    else
    {
        V_FLAG = 0;
    }
}
#pragma region DECODING_FUNCTIONS
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
        registersList[15] = i + 1;
        uint32_t instruction = encodedInstrtions[i]->encodedInstruction;
        cycles = 1;

#pragma region CONDITIONS
        // condition code
        uint32_t condition = (instruction >> 28) & 0x0F;
        if (condition != 0xE)
        {
            if (condition == 0x0) // EQ
            {
                if (Z_FLAG == 0)
                {
                    continue;
                }
            }
            else if (condition == 0x1) // NE
            {
                if (Z_FLAG == 1)
                {
                    continue;
                }
            }
            else if (condition == 0xc) // GT
            {
                if (N_FLAG == 1)
                {
                    continue;
                }
            }
            else if (condition == 0xb) // LT
            {
                if (N_FLAG == 0)
                {
                    continue;
                }
            }
            // the recomended way was N == V and Z == 0 || N != V. but this way i feel is more efficent

            else if (condition == 0xa)
            {
                if (N_FLAG == 1 && Z_FLAG != 1) // LE
                {
                    continue;
                }
                else
                {
                }
            }
            else if (condition == 0xd)
            {
                if (N_FLAG == 0 && Z_FLAG != 1) // GE
                {
                    continue;
                }
                else
                {
                }
            }
        }
#pragma endregion

#pragma region DATA_PROCESSING decoding

        if (encodedInstrtions[i]->instructionType == InstructionType::DATA_PROCESSING)
        {
            cycles = 0;

            map<int, dataProcessingInstructions> instructionMap;
            instructionMap[0x0] = dataProcessingInstructions::AND;

            instructionMap[0x4] = dataProcessingInstructions::ADD;
            instructionMap[0x2] = dataProcessingInstructions::SUB;
            instructionMap[0xA] = dataProcessingInstructions::CMP;
            instructionMap[0xD] = dataProcessingInstructions::MOV;
            instructionMap[0xC] = dataProcessingInstructions::ORR;
            instructionMap[0x5] = dataProcessingInstructions::ADC;
            instructionMap[0x6] = dataProcessingInstructions::SBC;

            uint32_t iBit = (instruction >> 25) & 1;
            uint32_t opcode = (instruction >> 21) & 0x0F;
            uint32_t SBit = (instruction >> 20) & 1;
            uint32_t rd = (instruction >> 12) & 0x0F;
            uint32_t rn = (instruction >> 16) & 0x0F;
            uint64_t immediate = instruction & 0xFFF;
            uint64_t canror = (instruction >> 27) & 1;

            int carry = 0;
            if (iBit == 1 && canror == 1)
            {
                uint64_t rotationamt = (instruction >> 8) & 0xF;
                uint64_t value = (instruction)&0xFF;
                immediate = rightRotate((value), (rotationamt * 2));
            }

            dataProcessingInstructions instructionTypes = instructionMap[opcode];

            if (rd > 12)
            {
                cout << "rd to big " << rd << endl;
                exit(EXIT_FAILURE);
                return;
            }
            if (instructionTypes == dataProcessingInstructions::ADD) // ADD
            {

                if (iBit == 1)
                {
                    registersList[rd] = (int)add(registersList[rn], immediate, carry);
                }
                else
                {
                    registersList[rd] = (int)add(registersList[rn], registersList[immediate], carry);
                }

                if (SBit == 1)
                {
                    editCPSRFlags(registersList[rd]);
                    C_FLAG = carry;
                }
            }
            else if (instructionTypes == dataProcessingInstructions::SUB) // SUB
            {

                if (iBit == 1)
                {
                    registersList[rd] = (int)sub(registersList[rn], immediate, carry);
                }
                else
                {
                    registersList[rd] = (int)sub(registersList[rn], registersList[immediate], carry);
                }
                if (SBit == 1)
                {
                    editCPSRFlags(registersList[rd]);
                    C_FLAG = carry;
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
            else if (instructionTypes == dataProcessingInstructions::CMP)
            {
                uint32_t a;
                if (iBit == 1)
                {
                    a = (int)sub(registersList[rd], immediate, carry);
                }
                else
                {
                    a = sub(registersList[rd], registersList[immediate], carry);
                }
                editCPSRFlags(a);
                C_FLAG = carry;
            }
            else if (instructionTypes == dataProcessingInstructions::ORR)
            {
                // cout << (uint32_t)(904 | immediate) << endl;
                if (iBit == 1)
                {
                    registersList[rd] = (int)registersList[rn] | immediate;
                }
                else
                {
                    registersList[rd] = (int)registersList[rn] | registersList[immediate];
                }
            }
            else if (instructionTypes == dataProcessingInstructions::AND)
            {

                if (iBit == 1)
                {
                    registersList[rd] = (int)registersList[rn] & immediate;
                }
                else
                {
                    registersList[rd] = (int)registersList[rn] & registersList[immediate];
                }
            }
            else if (instructionTypes == dataProcessingInstructions::ADC)
            {

                if (iBit == 1)
                {
                    registersList[rd] = (int)add(registersList[rn], immediate, carry) + C_FLAG;
                }
                else
                {
                    registersList[rd] = (int)add(registersList[rn], registersList[immediate], carry) + C_FLAG;
                }
                if (SBit == 1)
                {
                    editCPSRFlags(registersList[rd]);
                    C_FLAG = carry;
                }
            }
            else if (instructionTypes == dataProcessingInstructions::SBC)
            {
                if (iBit == 1)
                {
                    registersList[rd] = (int)sub(registersList[rn], immediate, carry) - C_FLAG;
                }
                else
                {
                    registersList[rd] = (int)sub(registersList[rn], registersList[immediate], carry) - C_FLAG;
                }
                if (SBit == 1)
                {
                    editCPSRFlags(registersList[rd]);
                    C_FLAG = carry;
                }
            }
            cycles += 3;
        }
#pragma endregion

#pragma region MULTIPLY decoding
        else if (encodedInstrtions[i]->instructionType == InstructionType::MULTIPLY)
        {
            uint32_t aBit = (instruction >> 21) & 1;
            uint32_t sBit = (instruction >> 20) & 1;
            uint32_t rd = (instruction >> 16) & 0x0F;
            uint32_t rn = (instruction >> 12) & 0x0F;
            uint32_t rs = (instruction >> 8) & 0x0F;
            uint32_t rm = instruction & 0x0F;
            cycles = 0;
            cout << rd << endl;
            cout << rn << endl;
            cout << rm << endl;

            if (rd > 12)
            {
                cout << "rd to big" << endl;
                // continue;
                exit(EXIT_FAILURE);
                return;
            }
            if (aBit == 1)
            {
                int c = (registersList[rm] * registersList[rn]) + registersList[rs];
                registersList[rd] = c;
            }
            else
            {
                registersList[rd] = mul(registersList[rn], registersList[rm]);
            }
            if (sBit == 1)
            {
                editCPSRFlags(registersList[rd]);
            }
            cycles += 2;
        }
#pragma endregion

#pragma region BRANCHING decoding

        else if (encodedInstrtions[i]->instructionType == InstructionType::JMPBRANCH)
        {

            uint32_t LBit = (instruction >> 23) & 1;

            uint32_t pcNewValue = instruction & 0x7FFFFF;
            cycles = 0;

            i = (int)pcNewValue;
            cycles += 3;
        }
        else if (encodedInstrtions[i]->instructionType == InstructionType::BRANCH)
        {
        }
        total_cycles += cycles;
#pragma endregion
    }
#pragma region REGISTERS
    cout << "" << endl;
    cout << "REGISTERS FILE" << endl;
    cout << "===============" << endl;
    cout << "" << endl;

    for (int i = 0; i < 16; i++)
    {
        cout << "R" << i << " value: " << registersList[i] << endl;
    }
    cout << "" << endl;
    cout << "===============" << endl;
    cout << "" << endl;
    cout << "number of cycles took: " << total_cycles << endl;
    cout << "===============" << endl;
    cout << "CPSR conditions" << endl;
    cout << "N: " << N_FLAG << endl;
    cout << "V: " << V_FLAG << endl;
    cout << "C: " << C_FLAG << endl;
    cout << "Z: " << Z_FLAG << endl;
    cout << "===============" << endl;
    cout << "" << endl;
    cout << "code complete" << endl;
#pragma endregion
}

#pragma endregion