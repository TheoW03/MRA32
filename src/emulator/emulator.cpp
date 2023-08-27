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
Instructions::~Instructions()
{
    // implementation of the destructor goes here
}
vector<uint64_t> encode(vector<Instructions *> InstructionsList)
{
    for (int i = 0; i < InstructionsList.size(); i++)
    {
        // i then encode it
        if (instanceof <AddInstruct *>(InstructionsList[i]))
        {
            
        }
    }
}
void emulate(vector<Instructions *> InstructionsList)
{
    vector<uint64_t> encodedInstrtions = encode(InstructionsList);
}