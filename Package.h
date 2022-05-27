#pragma once

#include <vector>
#include "Rectangle.h"
#include "ClassInfo.h"

#include "tinyxml2.h"

struct Package
{
    std::string name;
    Rectangle coords;
    std::vector<Package> subPackages;

    bool insert(const Package& sub)
    {
        if(!coords.contains(sub.coords))
            return false;

        for(auto& subsub : subPackages)
            if(subsub.insert(sub))
                return true;

        subPackages.push_back(sub);
        return true;
    }

    void makeDirectories(const std::string& dest)
    {
        std::string packageDest = dest + name + '/';

        mkdir(packageDest.c_str());

        for(auto& sub : subPackages)
            sub.makeDirectories(packageDest);
    }

    std::string getDestination(const ClassInfo& classInfo) const
    {
        if(!coords.contains(classInfo.coords))
            throw std::runtime_error("cannot parse destination!");

        for(auto& sub : subPackages)
            if(sub.coords.contains(classInfo.coords))
                return name + '/' + sub.getDestination(classInfo);

        return name + '/';
    }

    int getDepth(const ClassInfo& classInfo) const
    {
        for(auto& sub : subPackages)
            if(sub.coords.contains(classInfo.coords))
                return sub.getDepth(classInfo) + 1;

        return 0;
    }

    std::string getPath(const ClassInfo& from, const ClassInfo& to) const
    {
        for(auto& sub : subPackages)
            if( sub.coords.contains(from.coords)
            &&  sub.coords.contains(to.coords) )
                return sub.getPath(from, to);

        std::string back;

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