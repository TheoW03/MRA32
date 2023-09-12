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
    END_OF_PROGRAM,
    WORD,
    CONDITION

};
struct Tokens
{
    string buffer;
    type id;
    map<type, string> dictionary;
};
void printList(vector<Tokens> a)
{
    map<type, string> out;
    out[type::INSTRUCTION] = "INSTRUCTION";
    out[type::NUMBER] = "NUMBER";
    out[type::REGISTER] = "REGISTER";
    out[type::COMMA] = "COMMA";
    out[type::END_OF_PROGRAM] = "END_OF_PROGRAM";
    out[type::WORD] = "WORD";
    out[type::CONDITION] = "CONDITION";

    for (int i = 0; i < a.size(); i++)
    {
        if (out.find(a[i].id) != out.end())
        {
            cout << out[a[i].id] + " (" + a[i].buffer + ") " << endl;
        }
    }
}
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
    instructions["MULA"] = type::INSTRUCTION;

    instructions["AND"] = type::INSTRUCTION;
    instructions["MOV"] = type::INSTRUCTION;

    instructions["OR"] = type::INSTRUCTION;
    instructions["B"] = type::INSTRUCTION;
    instructions["CMP"] = type::INSTRUCTION;

    map<string, type> conditions;
    conditions["EQ"] = type::CONDITION;
    conditions["NE"] = type::CONDITION;

    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        int isComment = 0;
        string stringBuffer = "";

        regex numReg("[0-9]");           // num regex
        regex AlphaBetReg("[a-zA-Z]");   // num regex
        regex registerReg("R[0|1[0-5]"); // num regex

        int comment = 0;
        std::smatch myMatch;
        for (int i2 = 0; i2 < line.length(); i2++)
        {
            if (comment == 1)
            {
                continue;
            }
            char current = line.at(i2);
            if (current == '#')
            {
                if (!stringBuffer.empty() && stringBuffer.size() != 0 && stringBuffer != "")
                {

                    Tokens token;
                    token.buffer = stringBuffer;
                    token.id = (instructions.find(stringBuffer) != instructions.end())                                              ? type::INSTRUCTION
                               : (regex_search(stringBuffer, myMatch, numReg) && !regex_search(stringBuffer, myMatch, AlphaBetReg)) ? type::NUMBER
                               : (regex_search(stringBuffer, myMatch, registerReg))                                                 ? type::REGISTER
                                                                                                                                    : type::WORD;
                    a.push_back(token);
                    stringBuffer = "";
                    state = 1;
                }
                comment = 1;
                continue;
            }
            string str(1, current);
            if ((int)current == 13)
            {
                continue;
            }
            if (current != ' ' && current != '\t' && current != '\0')
            {
                // cout << "state: " << state << endl;
                // cout << "buffer: " << stringBuffer << endl;
                // // cout << "str: " << str << endl;

                if (state == 1)
                {
                    if (instructions.find(stringBuffer) != instructions.end())
                    {
                        Tokens token;
                        token.buffer = stringBuffer;
                        token.id = type::INSTRUCTION;
                        a.push_back(token);
                        stringBuffer = "";
                        stringBuffer += str;
                        state = 3;
                    }
                    else if (regex_search(str, myMatch, numReg))
                    {
                        stringBuffer += str;
                    }
                    else if (str == ",")
                    {
                        if (stringBuffer != "")
                        {
                            Tokens token;
                            token.buffer = stringBuffer;
                            token.id = (regex_search(stringBuffer, myMatch, numReg) && !regex_search(stringBuffer, myMatch, AlphaBetReg)) ? type::NUMBER
                                                                                                                                          : type::REGISTER;

                            a.push_back(token);

                            // cout << "" << endl;
                            stringBuffer = "";
                            Tokens commaToken; // Create a separate token for the comma
                            commaToken.buffer = str;
                            commaToken.id = type::COMMA;
                            a.push_back(commaToken);
                            state = 2;
                        }
                    }
                    else if (str == ":")
                    {
                        if (stringBuffer != "")
                        {
                            Tokens token;
                            token.buffer = stringBuffer;
                            token.id = type::WORD;
                            a.push_back(token);
                            stringBuffer = "";
                        }
                    }
                    else
                    {
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
                            token.id = (instructions.find(stringBuffer) != instructions.end())                                              ? type::INSTRUCTION
                                       : (regex_search(stringBuffer, myMatch, numReg) && !regex_search(stringBuffer, myMatch, AlphaBetReg)) ? type::NUMBER
                                                                                                                                            : type::REGISTER;
                            a.push_back(token);
                            // cout << "where repetitive chars are" << endl;
                            stringBuffer = "";
                            Tokens commaToken; // Create a separate token for the comma
                            commaToken.buffer = str;
                            commaToken.id = type::COMMA;
                            a.push_back(commaToken);
                            state = 1;
                        }
                    }
                    else if (str == ":")
                    {
                        if (stringBuffer != "")
                        {
                            Tokens token;
                            token.buffer = stringBuffer;
                            token.id = type::WORD;
                            a.push_back(token);
                            stringBuffer = "";
                            state = 1;
                        }
                    }
                    else
                    {
                        stringBuffer += str;
                    }
                }
                else if (state == 3)
                {
                    if (conditions.find(stringBuffer) != conditions.end())
                    {
                        Tokens token;
                        token.buffer = stringBuffer;
                        token.id = type::CONDITION;
                        a.push_back(token);
                        stringBuffer = "";
                        state = 1;
                    }
                    else
                    {
                        // cout << "condition " << endl;

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
                               : (regex_search(stringBuffer, myMatch, registerReg))                                                 ? type::REGISTER
                               : (conditions.find(stringBuffer) != conditions.end())                                                ? type::CONDITION
                                                                                                                                    : type::WORD;
                    a.push_back(token);
                    // cout << "a: " << stringBuffer << endl;

                    stringBuffer = "";

                    state = 1;
                }
            }
        }

        if (!stringBuffer.empty() && stringBuffer.size() != 0 && stringBuffer != "")
        {
            // cout << "end" << endl;

            Tokens token;
            token.buffer = stringBuffer;
            token.id = (instructions.find(stringBuffer) != instructions.end())                                              ? type::INSTRUCTION
                       : (regex_search(stringBuffer, myMatch, numReg) && !regex_search(stringBuffer, myMatch, AlphaBetReg)) ? type::NUMBER
                       : (regex_search(stringBuffer, myMatch, registerReg))                                                 ? type::REGISTER
                       : (conditions.find(stringBuffer) != conditions.end())                                                ? type::CONDITION

                                                                             : type::WORD;
            a.push_back(token);
            stringBuffer = "";
            state = 1;
        }
    }
    Tokens token;
    token.buffer = "";
    token.id = type::END_OF_PROGRAM;
    a.push_back(token);
    cout << "lexxing complete" << endl;
    return a;
}
