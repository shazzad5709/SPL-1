#include<iostream>
#include<map>
#include<stack>
#include<fstream>

using namespace std;

fstream code("code.txt", ios::in | ios::out);
map<string, bool> variableDeclared;
stack<string> indent;
stack<int> algoIndent;

bool isDelimiter(char x)
{
    if(x==' ' || x=='(' || x==')' || x=='{'
    || x=='}' || x=='['|| x==']'|| x=='\n')
        return true;
    return false;
}

void printFunction(string line)
{
    int i=7+algoIndent.top();
    code<<"    cout <<";
    while(i<line.length())
    {
        if(line[i]=='"')
        {
            i++;
            code<<" \"";
            while(line[i]!='"')
                code<<line[i++];
            code<<"\"";
            i++;
        }
        else if(line[i]==' ' || line[i]==',' || line[i]==')')
        {   
            if(line[i]==',')
                code<<" << ";
            i++;
        }
        else
        {
            while((line[i]>='a' && line[i]<='z')
                || (line[i]>='A' && line[i]<='Z') || line[i]=='_')
                code<<line[i++];
        }
        
    }
    code<<";";
    return;
}

void inputFunction(string line)
{
    variableDeclared["mahin"]=true;
    variableDeclared["abc"]=true;
    variableDeclared["x"]=variableDeclared["y"]=true;
    string x;
    int i=7+algoIndent.top();
    while(i<line.length())
    {
        while(i<line.length())
        {
            if(line[i]==',' || line[i]==')')
            {
                i++;
                break;
            }
            else if(line[i]==' ')
                i++;
            else
            {
                x.push_back(line[i]);
                i++;
            }
        }
        // cout<<x<<endl;
        // cout<<variableDeclared[x]<<endl;
        if(variableDeclared[x]==1)
        {
            if(i-x.length()-1==7)
                code<<indent.top()<<"cin >> "<<x;
            else if(i!=line.length()-2)
                code<<" >> "<<x;
        }
        x.erase();
    }
    code<<";\n";
}

string extractKeyword(string line)
{
    
    string x;
    int len=0;
    for(int i=0; i<line.length(); i++)
        if(isDelimiter(line[i])==false)
            len++;
        else
            break;
    x=line.substr(0, len);
    return x;
}

void declareVariable(string line)
{

}

void parse()
{
    string line;
    ifstream f("huh.txt");
    if(f.is_open())
    {
        while(getline(f, line))
        {
            string x=extractKeyword(line);;
            
            if(x=="print")
                printFunction(line);
            else if(x=="input")
                inputFunction(line);
            else
                declareVariable(line);
        }
    }

}

void codeOutput()
{
    ifstream aaaa("code.txt");
    if(aaaa.is_open())
        cout<<aaaa.rdbuf();
    aaaa.close();
}

int main()
{
    cout<<"Algorithm to C++"<<endl;
    cout<<"-----------------"<<endl;
    string line;
    fstream f;
    // f.open("huh.txt");
    // getline(cin, line);
    // f<<line;
    // f.close();
    // code.open("code.txt");
    cout<<"*reading algorithm from text file*\n";
    cout<<"---------------------------------------------------" << endl;
    code<<"#include<bits/stdc++.h>\n";
    code<<"using namespace std;\n";
    code<<"int main()\n{\n";
    indent.push("    ");
    algoIndent.push(0);
    parse();
    code<<indent.top()<<"return 0;\n}";
    code.close();
    codeOutput();
    return 0;
}
