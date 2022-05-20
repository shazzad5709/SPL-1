#pragma once

#include <vector>
#include "Rectangle.h"
#include "ClassData.h"

#include "tinyxml2.h"

using namespace std;

struct Package
{
    string name;
    Rectangle coords;
    vector<Package> subPackages;

    bool insert(const Package& sub)
    {
        if(!coords.contains(sub.coords))
            return false;

        for(auto& subPac : subPackages)
            if(subPac.insert(sub))
                return true;

        subPackages.push_back(sub);
        return true;
    }

    void makeDirectories(const string& dest)
    {
        string packageDest = dest + name + '/';

        mkdir(packageDest.c_str());

        for(auto& sub : subPackages)
            sub.makeDirectories(packageDest);
    }

    string getDestination(const ClassData& classData) const
    {
        if(!coords.contains(classData.coords))
            throw runtime_error("cannot parse destination!");

        for(auto& sub : subPackages)
            if(sub.coords.contains(classData.coords))
                return name + '/' + sub.getDestination(classData);

        return name + '/';
    }

    int getDepth(const ClassData& classData) const
    {
        for(auto& sub : subPackages)
            if(sub.coords.contains(classData.coords))
                return sub.getDepth(classData) + 1;

        return 0;
    }

    string getPath(const ClassData& from, const ClassData& to) const
    {
        for(auto& sub : subPackages)
            if( sub.coords.contains(from.coords)
            &&  sub.coords.contains(to.coords) )
                return sub.getPath(from, to);

        string back;

        for(size_t k=0 ; k<getDepth(from); k++)
            back += "../";

        for(auto& sub : subPackages)
            if(sub.coords.contains(to.coords))
                return back + sub.getDestination(to); 

        return back;
    }
};

Package parsePackage(const tinyxml2::XMLElement* xmlElement)
{
    Package package;

    xmlElement->FirstChildElement("coordinates") >> package.coords;
    package.name = xmlElement->FirstChildElement("panel_attributes")->GetText();
    package.name = package.name.substr(0, package.name.find('\n'));

    return package;
}