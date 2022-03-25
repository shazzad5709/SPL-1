#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

vector<string> classNames;
vector<vector<string>> classAttributes;
vector<vector<string>> classMethods;

void classInput()
{
    cout<<"Name of the class: ";
    
    string tempName;
    cin>>tempName;
    classNames.push_back(tempName);
    
    int attributesNo;
    cout<<"No. of attributes of each objects: ";
    cin>>attributesNo;
    string tempAttr;
    cout<<"Attributes: \n";
    cout<<"(format) Encapsulation AttributeName: DataType\n";
    vector<string> attributes;
    cin.clear();
    cin.sync();
    for(int i=0; i<attributesNo; i++)
    {
        getline(cin, tempAttr);
        attributes.push_back(tempAttr);
    }
    classAttributes.push_back(attributes);

    int methodNo;
    cout<<"No. of methods: ";
    cin>>methodNo;
    string tempMethod;
    vector<string> methods;
    cout<<"Methods: \n";
    cout<<"(format) Encapsulation MethodName: ReturnType\n";
    cin.clear();
    cin.sync();
    for(int i=0; i<methodNo; i++)
    {
        getline(cin, tempMethod);
        methods.push_back(tempMethod);
    }
    classMethods.push_back(methods);

}

void generateClass()
{
    fstream c("class.txt");
    for(int i=0; i<classNames.size(); i++)
    {
        c<<classNames[i]<<endl;
        c<<"---------------------\n";
        for(int j=0; j<classAttributes[i].size(); i++)
            c<<classAttributes[i][j]<<endl;
        c<<"---------------------\n";
        for(int j=0; j<classMethods[i].size(); i++)
            c<<classMethods[i][j]<<endl;
        c<<"\n---------------------\n";
        c<<"---------------------\n";
    }
}

int main()
{
    int choice=1;
    while(choice!=3)
    {
        classInput();
        cout<<"1. Create new class\n";
        cout<<"2. Create new interface\n";
        cout<<"3. Terminate\n";
        cin>>choice;
    }
    generateClass();
}