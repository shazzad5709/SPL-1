#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// enum DefaultReturnType
// {
//     Empty,
//     DEFAULT_VALUE,
//     THROW_EXCEPTION
// };

struct Info
{
    string umlPath = "";
    string sourcePath = "";

    // bool generateDocument = true;
    // bool generateClassDescription = true;
    // bool generateAttributeProperties = true;
    // bool generateConstructors = true;
    // string notice = "";

    // bool generateImplAttributes = true;
    // bool generateImplConstructors = true;
    // bool generateImplEmptyMethods = true;
    // DefaultReturnType defaultReturnType = THROW_EXCEPTION;
};

bool hasEnding(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
        return (fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0);

    else
        return false;
}

string selectDiagram()
{
    cout << "Directory of the Diagram (.uxf file): " << endl;

    string filePath;
    cin >> filePath;

    filePath.substr(0, filePath.find('\n'));

    ifstream f(filePath.c_str());
    
    while(!f.good() || !hasEnding(filePath, ".uxf"))
    {
        if(!f.good())
            cout << "The file could not be found" << endl;
        else
            cout << "The file not an uxf file" << endl;

        cout << "Please try again" << endl;
        cin >> filePath;
    }
    
    return filePath;
}

string selectSourceCodePath()
{
    cout << "Directory in which the source code will be generated: " << endl;

    string folderPath;
    cin >> folderPath;

    if(folderPath.back() != '/')
        folderPath += '/';

    return folderPath;
}

Info consoleUI()
{
    Info info;

    info.umlPath = selectDiagram();
    info.sourcePath = selectSourceCodePath();

    return info;
}