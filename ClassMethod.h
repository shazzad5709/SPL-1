#pragma once

#include <vector>
#include <iostream>
#include "Encapsulation.h"
#include "Utility.h"
#include "ClassAttributes.h"
using namespace std;

struct Method {
    Encapsulation encapsulation;
    bool isVirtual;
    string returnType;
    string name;
    vector<Attribute> argument;

    bool isConstructor() const {
        return returnType.empty();
    }

    bool isDestructor() const {
        return returnType.empty() && name[0]=='~';
    }
};

Method parseMethod(string line)
{
    Method method;

    if(line[0]=='/')
    {
        if(line.back()!='/')
            throw runtime_error("Error! "+line+" is not well formatted (missing / in the end)");
        
        line = line.substr(1, line.length()-2);
        method.isVirtual = true;
    }

    method.encapsulation = getEncapsulation(line[0]);

    string str;
    for(size_t k=1; k<line.length(); k++)
        if(line[k]!=' ')
            str+=line[k];
    line = str;

    size_t beginParsing = line.find('(');
    size_t endParsing = line.find(')');

    method.name = line.substr(0, beginParsing);

    if(beginParsing+1 < endParsing)
    {
        auto arguments = splitArguments(line.substr(beginParsing+1, endParsing-beginParsing-1));
        
        for(auto& arg: arguments)
        {
            auto argumentInfo = split(arg, ":");
            method.argument.push_back({
                PRIVATE,
                (argumentInfo.size()==2)? checkStdNamespace(argumentInfo[1]) : "",
                argumentInfo[0]
            });
        }
    }

    auto methodInfo=split(line.substr(endParsing+1), ":");

    if(methodInfo.size()==2)
        method.returnType=checkStdNamespace(methodInfo[1]);

    return method;
}