#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Info
{
    string umlPath = "";
    string srcCodePath = "";

};

bool hasEnding (string const &fullString, string const &ending) {
    if (fullString.length() >= ending.length())
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    
    else
        return false;
}

string selectUmlFile()
{
    cout<<"Path of UML Diagram:"<<endl;

    string filePath;
    cin>>filePath;

    filePath.substr(0, filePath.find('\n'));

    ifstream f(filePath.c_str());
    
    while(!f.good() || !hasEnding(filePath, ".uxf"))
    {
        if(!f.good())
            cout<<"Error! The file could not be found!"<<endl;
        else
            cout<<"The file not an uxf file"<<endl;

        cout<<"Please try again"<<endl;
        cin>>filePath;
    }
    
    return filePath;
}

string selectFolderPath()
{
    cout<<"Folder path of generated codes:"<<endl;
    string folderPath;
    cin>>folderPath;

    if(folderPath.back()!='/')
        folderPath+='/';

    return folderPath;
}

const char* onOrOff(bool b) {
    return b ? "[ON]" : "[OFF]";
}

Info consoleUI()
{
    Info info;
    int x;
    cout<<"0. Exit\n";
    cin>>x;
    if(x==0) exit(1);
    info.umlPath=selectUmlFile();
    info.srcCodePath=selectFolderPath();
    return info;
}