#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include "Encapsulation.h"
#include "Utility.h"
using namespace std;

struct Attribute {
    Encapsulation encapsulation;
    string type;
    string name;
    vector<string> fields;

    bool hasField(const string& f) const
    {
        return find(fields.begin(), fields.end(), f) != fields.end();
    }

    string getSingularName() const
    {
        for(auto& field: fields)
        {
            if(field.find("singular")==0)
            {
                size_t begin = field.find('(');
                size_t end = field.find(')');
                return field.substr(begin+1, end-begin-1);
            }
        }

        return "";
    }

    string normalName() const {
        return normalizeName(name);
    }

    string normalSingularName() const {
        return normalizeName(getSingularName());
    }

    bool isContainer() const {
        return  type.find("std::vector") == 0
        ||      type.find("std::list") == 0;
    }
};

bool isStdType(const string& str)
{
    return
    str == "string" ||
    str == "size_t" ||
    str == "vector" ||
    str == "map" ||
    str.find("_ptr") < str.length();
}

string checkStdNamespace(string attrType)
{
    size_t begin = attrType.find('<');

    if(begin > attrType.length())
        if(isStdType(attrType))
            return "std::"+attrType;
        else
            return attrType;

    string str = checkStdNamespace(attrType.substr(0, begin)) + "<";

    size_t end = attrType.find_last_of('>');

    auto s = split(attrType.substr(begin+1, end-begin-1), ",");

    str += checkStdNamespace(s[0]);

    for(size_t k=1;k<s.size();k++)
    {
        str+=",";
        str+=checkStdNamespace(s[k]);
    }

    return str + attrType.substr(end);
}

Attribute parseAttribute(const string& line)
{
    Attribute attr;

    string str;
    for(size_t k=0;k<line.length();k++)
        if(line[k] !=' ')
            str += line[k];

    attr.encapsulation = getEncapsulation(str[0]);

    str = str.substr(1);

    if(str.find('[') < str.length())
    {
        auto begin = str.find_last_of('[');
        auto end = str.find_last_of(']');
        attr.fields = split(str.substr(begin+1, end-begin-1),"/");
        str = str.substr(0, begin);
    }

    auto attrInfo=split(str,":");
    attr.name=attrInfo[0];
    attr.type = checkStdNamespace(attrInfo[1]);

    return attr;
}