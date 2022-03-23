#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

vector<string> classNames;

void classInput()
{
    cout<<"Name of the class: ";
    
    string tempName;
    cin>>tempName;
    classNames.push_back(tempName);

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
}