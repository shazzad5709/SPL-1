#include "tinyxml2.cpp"

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <io.h>
#include <limits>

#include "UI.cpp"

using namespace std;
using namespace tinyxml2;

//vector<ClassData> classData;

Info info;

int main()
{
    info = consoleUI();

    XMLDocument doc;
    doc.LoadFile(info.umlPath.c_str());
    auto elementList = doc.FirstChildElement("diagram");

    globalPackage.coordinates = Rectangle (
        
    )
}