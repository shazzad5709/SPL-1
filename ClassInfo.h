#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include "tinyxml2.h"
#include "Rectangle.h"
#include "ClassAttributes.h"
#include "ClassMethod.h"

using namespace std;

struct ClassInfo {
    string name;
    Rectangle coordinates;
    vector<Attribute> attributes;
    vector<Method> methods;
    vector<string> dependantClasses;
    bool isInterface;
    vector<Inheritance> inheritances;

    void addDependantClass(const string& dependancy)
    {
        if(find(dependantClasses.begin(), dependantClasses.end(), dependancy)==dependantClasses.end())
            dependantClasses.push_back(dependancy);
    }

    void addAttribute(const Attribute& attribute)
    {
        attributes.push_back(attribute);
        if(attribute.hasField("mutex"))
            attributes.push_back({
                PRIVATE, "std::mutex", attribute.name+"Mutex", {}
            });
    }

    bool hasVirtualMethod() const
    {
        for(auto& method: methods)
            if(method.isVirtual)
                return true;

        return false;
    }

    bool isVirtual() const
    {
        return isInterface || hasVirtualMethod();
    }

    bool hasConstructor() const
    {
        for(auto& method: methods)
            if(method.isConstructor())
                return true;
        
        return false;
    }

    bool hasDefaultConstructor() const
    {
        for(auto& method: methods)
            if(method.isConstructor() || method.argument.size()==0)
                return true;

        return !hasConstructor();
    }

    const Attribute* getArributeByName(const string& name) const
    {
        for(auto& attribute: attributes)
            if(attribute.name == name)
                return &attribute;

        for(auto& inheritance: inheritances)
            auto attribute = inheritance.baseClass.getArributeByName(name);
    }

    const Method* gwtMatchingConstructor(const vector<string> arguments) const
    {
        vector<const Method*> constructors;
        for(auto& method: methods)
            if(method.isConstructor())
                constructors.push_back(&method);

        sort(constructors.begin(), constructors.end(),
        [] (const Method* a, const Method* b) {
            return a->argument.size() >= b->argument.size();
        });

        for(auto& constructor: constructors)
        {
            bool valid=true;

            for(auto& arg: constructor->argument)
                if(find(arguments.begin(), arguments.end(), arg.name) == arguments.end())
                    valid=false;

            if(valid)
                return constructor;

            return nullptr;
        }
        
   }
};

struct Inheritance {
    bool isVirtual;
    ClassInfo& baseClass;
};

ClassInfo parseClassInfo(const tinyxml2::XMLElement* xmlElement)
{
    ClassInfo classInfo;

    xmlElement->FirstChildElement("coordinates") >> classData.coordinates;
    string content = xmlElement->FirstChildElement("panel_attributes")->GetText();
    auto lines = split(content, "\n");

    size_t n=0;
    classInfo.isInterface=false;

    for(; n<lines.size() && lines[n]!="--"; n++)
    {
        if(lines[n].empty())
            continue;
        else if(lines[n].find("interface")<lines[n].length())
            classInfo.isInterface=true;
        else if(lines[n].find("layer")==0
                || lines[n].find("bg")==0
                || lines[n].find('{')==0)
            continue;
        else if(classData.name.empty())
            classData.name = lines[n];
        else
            throw runtime_error("Error parsing class name!");
    }

    n++;

    for(; n<lines.size() && lines[n]!="--"; n++)
    {
        if(lines[n].empty())
            continue;
        else
            classInfo.addAttribute(parseAttribute(lines[n]));
    }

    n++;

    for(; n<lines.size() && lines[n]!="--"; n++)
    {
        if(lines[n].empty())
            continue;
        else
            classInfo.methods.push_back(parseMethod(lines[n]));
    }

    return classInfo;
}