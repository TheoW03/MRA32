#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>
#include <map>

using namespace std;

#ifndef TYPE_H
#define TYPE_H
enum class type
{
    INSTRUCTION,
    REGISTER,
    NUMBER,
    COMMA,
    END_OF_LINE,
    END_OF_PROGRAM

};
#endif
#ifndef TOKENS_H
#define TOKENS_H
struct Tokens
{
    string buffer;
    type id;
    map<type, string> dictionary;
};
#endif
vector<string> readFile(string file);
vector<Tokens> lex(vector<string> lines);
void printList(vector<Tokens> a);