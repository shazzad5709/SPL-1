#pragma once
#include <string>

using namespace std;

string normalizeName(const string& name)
{
    string normal;

    for(char c : name)
        if(isupper(c))
            (normal+=' ')+=tolower(c);
        else
            normal+=c;

    return normal;
}

string capitalize(std::string str)
{
    str[0]=toupper(str[0]);
    return str;
}

vector<string> split(const string& s, const string& delim)
{
    vector<string> split;

    auto start=0U;
    auto end=s.find(delim);
    while (end!=string::npos)
    {
        split.push_back(s.substr(start, end-start));
        start=end+delim.length();
        end=s.find(delim, start);
    }

    split.push_back(s.substr(start, end));
    return split;
}

vector<string> splitArguments(const string& line)
{
    vector<string> arguments;
    int level=0;
    size_t previousIndex=0;

    for(size_t k=0; k<line.length();k++)
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

    arguments.push_back(line.substr(previousIndex));

    return arguments;
}