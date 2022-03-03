#include<iostream>
#include<map>
#include<stack>
#include<fstream>
#include<vector>
#include<iterator>
#include<algorithm>

using namespace std;

fstream code("code.txt", ios::in | ios::out);
map<string, bool> variableDeclared;
map<string, string> varType;
stack<string> indent;
stack<int> algoIndent;
map<string, bool> dataType;

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
    code<<indent.top()<<"cout <<";
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
                || (line[i]>='A' && line[i]<='Z') || line[i]=='_'
                || line[i]=='+' || line[i]=='-' || line[i]=='*' 
                || line[i]=='/' || line[i]=='%' )
                code<<line[i++];
        }
        
    }
    code<<";\n";
    return;
}

void inputFunction(string line)
{
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
            if(i-x.length()-1==7)
                code<<indent.top()<<"cin >> "<<x;
            else if(i!=line.length()-2)
                code<<" >> "<<x;
        x.erase();
    }
    code<<";\n";

    return;
}

string extractKeyword(string line)
{
    string x;
    int len=0;
    for(int i=algoIndent.top(); i<line.length(); i++)
        if(isDelimiter(line[i])==false)
            len++;
        else
            break;
    x=line.substr(0, len);
    return x;
}

bool isDataType(string line)
{
    string p;
    
    dataType["int"]=true;
    dataType["char"]=true;
    dataType["long"]=true;
    dataType["long long"]=true;
    dataType["float"]=true;
    dataType["double"]=true;
    dataType["string"]=true;
    int len=0;
    for(int i=algoIndent.top(); i<line.length(); i++)
        if(isDelimiter(line[i])==false)
            len++;
        else
            break;
    p=line.substr(0, len);
    return dataType[p];
    
}

void declareVariable(string line)
{
    vector<string> variables;
    int i=0+algoIndent.top();
    string x;
    while(line[i]!=' ')
    {
        x.push_back(line[i++]);
    }
    i++;
    if(dataType[x]==true)
    {
        code<<indent.top()<<x<<" ";
        while(i<line.length())
        {
            string var;
            while(i<line.length() && line[i]!=',')
                var.push_back(line[i++]);
            i++;
            variableDeclared[var]=true;
            varType[var]=x;
            variables.push_back(var);
            var.erase();            
        }
        vector<string>::iterator it;
        int j=0;
        for(it=variables.begin(); it!=variables.end(); it++, j++)
        {
            code<<*it;
            if(j<variables.size()-1)
                code<<", ";
            else
                code<<";\n";
        }
    }    
    return;
}

void statement(string line)
{
    int i=0+algoIndent.top();
    code<<indent.top();
    while(i<line.length())
        code<<line[i++];
    code<<";\n";
    return;
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
            else if(isDataType(line)==true)
                declareVariable(line);
            else
                statement(line);
        }
    }
}

void codeOutput()
{
    ifstream aaaaa("code.txt");
    if(aaaaa.is_open())
        cout<<aaaaa.rdbuf();
    aaaaa.close();
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
