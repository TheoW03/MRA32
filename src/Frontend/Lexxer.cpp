#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>
#include <map>

using namespace std;

enum class type
{
    INSTRUCTION,
    REGISTER,
    NUMBER,
    COMMA,
    END_OF_LINE,
    END_OF_PROGRAM

};
struct Tokens
{
    string buffer;
    type id;
    map<type, string> dictionary;
};
vector<string> readFile(string file)
{
    vector<string> vec;
    ifstream infile(file); // Open the file for reading
    string line;
    // cout << "input: " << endl;
    while (getline(infile, line))
    {
        // cout << line + "\n";
        vec.push_back(line);
    }

    infile.close();
    return vec;
}

vector<Tokens> lex(vector<string> lines)
{

    vector<Tokens> a;
    int state = 1;
    map<string, type> instructions;
    instructions["ADD"] = type::INSTRUCTION;
    instructions["SUB"] = type::INSTRUCTION;
    instructions["MUL"] = type::INSTRUCTION;
    instructions["AND"] = type::INSTRUCTION;
    instructions["MOV"] = type::INSTRUCTION;

    instructions["OR"] = type::INSTRUCTION;

    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        int isComment = 0;
        string stringBuffer = "";

        regex numReg("[0-9]");         // num regex
        regex AlphaBetReg("[a-zA-Z]"); // num regex

        std::smatch myMatch;
        for (int i2 = 0; i2 < line.length(); i2++)
        {
            char current = line.at(i2);
            string str(1, current);
            if ((int)current == 13)
            {
                continue;
            }
            if (current != ' ' && current != '\t' && current != '\0')
            {

                if (state == 1)
                {

                    if (regex_search(str, myMatch, numReg))
                    {
                        stringBuffer += str;
                    }
                    else if (str == ",")
                    {
                        if (stringBuffer != "")
                        {
                            Tokens token;
                            token.buffer = stringBuffer;
                            token.id = type::NUMBER;
                            a.push_back(token);
                            stringBuffer = "";
                            Tokens commaToken; // Create a separate token for the comma

                            commaToken.buffer = str;
                            commaToken.id = type::COMMA;
                            a.push_back(token);
                            state = 2;
                        }
                    }
                    else
                    {
                        state = 2;
                        stringBuffer += str;
                    }
                }
                else if (state == 2)
                {
                    if (str == ",")
                    {
                        if (stringBuffer != "")
                        {
                            Tokens token;
                            token.buffer = stringBuffer;
                            token.id = (instructions.find(stringBuffer) != instructions.end()) ? type::INSTRUCTION : type::REGISTER;
                            a.push_back(token);
                            stringBuffer = "";
                            Tokens commaToken; // Create a separate token for the comma
                            commaToken.buffer = str;
                            commaToken.id = type::COMMA;
                            a.push_back(commaToken);
                            state = 1;
                        }
                    }
                    else
                    {
                        stringBuffer += str;
                    }
                }
            }
            else
            {
                if (stringBuffer != "")
                {

                    Tokens token;
                    token.buffer = stringBuffer;
                    token.id = (instructions.find(stringBuffer) != instructions.end())                                              ? type::INSTRUCTION
                               : (regex_search(stringBuffer, myMatch, numReg) && !regex_search(stringBuffer, myMatch, AlphaBetReg)) ? type::NUMBER
                                                                                                                                    : type::REGISTER;
                    a.push_back(token);
                    stringBuffer = "";

                    state = 1;
                }
            }
            
        }

        if (!stringBuffer.empty() && stringBuffer.size() != 0 && stringBuffer != "")
        {

            Tokens token;
            token.buffer = stringBuffer;
            token.id = (instructions.find(stringBuffer) != instructions.end())                                              ? type::INSTRUCTION
                       : (regex_search(stringBuffer, myMatch, numReg) && !regex_search(stringBuffer, myMatch, AlphaBetReg)) ? type::NUMBER
                                                                                                                            : type::REGISTER;
            a.push_back(token);
            stringBuffer = "";
            state = 1;
        }

        Tokens token;
        token.buffer = ";";
        token.id = type::END_OF_LINE;
        a.push_back(token);
    }
    Tokens token;
    token.buffer = "";
    token.id = type::END_OF_PROGRAM;
    a.push_back(token);
    return a;
}

void printList(vector<Tokens> a)
{
    map<type, string> out;
    out[type::INSTRUCTION] = "INSTRUCTION";
    out[type::NUMBER] = "NUMBER";
    out[type::REGISTER] = "REGISTER";
    out[type::COMMA] = "COMMA";
    out[type::END_OF_LINE] = "END_OF_LINE";
    out[type::END_OF_PROGRAM] = "END_OF_PROGRAM";
    for (int i = 0; i < a.size(); i++)
    {
        if (out.find(a[i].id) != out.end())
        {
            cout << out[a[i].id] + " (" + a[i].buffer + ") " << endl;
        }
    }
}