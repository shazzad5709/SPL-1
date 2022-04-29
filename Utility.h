#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;


string normalizeName(const string& name)
{
    string normalName;

    for(char c: name)
    {
        if(isupper(c))
            (normalName+=' ')+=tolower(c);
        else
            normalName+=c;
    }
    return normalName;
}

string capitalize(string str)
{
    str[0]=toupper(str[0]);
    return str;
}

vector<string> split(const string& s, const string& delimiter)
{
    vector<string> split;

    auto start = 0U;
    auto end = s.find(delimiter);
    while(end!=string::npos)
    {
        split.push_back(s.substr(start, end-start));
        start = end+delimiter.length();
        end = s.find(delimiter, start);
    }
    
    split.push_back(s.substr(start, end));
    return split;
}

vector<string> splitArguments(const string& line)
{
    vector<string> arguments;
    int level = 0;

    size_t previousIndex=0;

    for(size_t k=0; k<line.length();k++)
    {
        if(line[k]=='<')
        {
            level++;
            break;
        }
        else if(line[k]=='>')
        {
            level--;
            break;
        }
        else if(line[k]==',')
        {
            if(level==0)
            {
                arguments.push_back(line.substr(previousIndex, k-previousIndex));
                previousIndex=k+1;
            }
            break;
        }
    }

    arguments.push_back(line.substr(previousIndex));
    return arguments;
}