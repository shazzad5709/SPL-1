#include<iostream>
#include<map>
#include<stack>
#include<fstream>
#include<vector>
#include<iterator>
#include<algorithm>

using namespace std;

fstream code("code.txt", ios::in | ios::out | ios::trunc);
map<string, bool> variableDeclared;
map<string, string> varType;
stack<string> indent;
stack<int> algoIndent;
map<string, bool> dataType;
int curlCount=0;

int length(string line)
{
    int len=0;
    int i=0;
    while(line[i]!='\0')
    {
        len++;
        i++;
    }
    return len;
}
    //returns length of a string

int countSpace(string line)
{
    int c=0;
    for(int i=0; i<length(line); i++)
    {
        if(line[i]==' ')
            c++;
        else
            return c;
    }
}

bool isDelimiter(char x)
{
    if(x==' ' || x=='(' || x==')' || x=='{'
    || x=='}' || x=='['|| x==']'|| x=='\n')
    {
        return true;
    }
    return false;
}

string extractKeyword(string line)
{
    string x;
    int len=0;
    for(int i=countSpace(line); i<length(line); i++)
    {
        if(isDelimiter(line[i])==false)
            len++;
        else
            break;
    }
    x=line.substr(countSpace(line), len);
    return x;
}
    /*
    determines whether a line of the algorithm
    is a general statement or it performs a 
    specific function
    if it performs a function, it returns which
    function it performs
    */

void printFunction(string line)
{
    int i=7+algoIndent.top();
    code<<indent.top()<<"cout <<";
    while(i<length(line))
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
            {
                code<<line[i++];
            }
        }
        
    }
    code<<";\n";
    return;
}
    //handles output stream

void inputFunction(string line)
{
    string x;
    int i=7+algoIndent.top();
    while(i<length(line))
    {
        while(i<length(line))
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
            if(i-length(x)-1==7)
                code<<indent.top()<<"cin >> "<<x;
            else if(i!=length(line)-2)
                code<<" >> "<<x;
        x.erase();
    }
    code<<";\n";

    return;
}
    //handles input stream

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
    for(int i=algoIndent.top(); i<length(line); i++)
    {
        if(isDelimiter(line[i])==false)
            len++;
        else
            break;
    }
    p=line.substr(0, len);
    return dataType[p];
    
}
    //determines what datatype a variable is

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
        while(i<length(line))
        {
            string var;
            while(i<length(line) && line[i]!=',')
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
                code<<",";
            else
                code<<";\n";
        }
    }    
    return;
}
    //declares variable by acknowledging datatype

void statement(string line)
{
    int i=0+algoIndent.top();
    code<<indent.top();
    while(i<length(line))
        code<<line[i++];
    code<<";\n";
    return;
}
    //handles a general statement

void ifFunction(string line)
{
    int i=2+algoIndent.top();
    string c;
    for(; i<length(line)-4; i++)
    {
        if(line[i]=='a' && line[i+1]=='n' && line[i+2]=='d')
        {
            c+="&&";
            i+=2;
        }
        else if(line[i]=='o' && line[i+1]=='r')
        {
            c+="||";
            i+=1;
        }
        else
            c.push_back(line[i]);
    }
    while(i<length(line))
    {
        c+=line[i++];
    }
    code<<indent.top()<<"if "<<c<<endl<<indent.top()<<"{\n";
    curlCount++;
    algoIndent.push(algoIndent.top()+4);
    //cout<<"if "<<algoIndent.top()<<endl;
    indent.top()+="    ";
}
    //handles if conditions

void elseFunction(string line)
{
    //cout<<"else "<<algoIndent.top()<<endl;
    
    if(length(line)-countSpace(line)>4)
    {
        string ln=line.substr(countSpace(line)+5, countSpace(line)+7);
        int i=2;
        string c, f;
        for(; i<ln.length()-4; i++)
        {
            if(ln[i]=='a' && ln[i+1]=='n' && ln[i+2]=='d')
            {
                c+="&&";
                i+=2;
            }
            else if(ln[i]=='o' && ln[i+1]=='r')
            {
                c+="||";
                i+=1;
            }
            else
                c.push_back(ln[i]);
        }
        while(i<length(line))
        {
            c+=line[i++];
        }
        code<<indent.top()<<"if "<<c<<endl<<indent.top()<<"{\n";
    }
    else
    {
        code<<indent.top()<<"else\n"<<indent.top()<<"{\n";
    }
    curlCount++;
    algoIndent.push(algoIndent.top()+4);
    indent.top()+="    ";
}
    //handles both 'else if' and 'else'

void parse()
{
    string line;
    int ln=1;
    ifstream f("algo.txt");
    if(f.is_open())
    {
        while(getline(f, line))
        {
            string x=extractKeyword(line);
            int gg=algoIndent.top()/(countSpace(line)+4);
            
            //this while loop implements nested blocks
            while(gg--)
            {
                // cout<<algoIndent.top()<<endl;
                // cout<<countSpace(line)<<endl;
                algoIndent.pop();
                indent.pop();
                string k(algoIndent.top()+4, ' ');
                indent.push(k);
                code<<indent.top()<<"}\n";
                curlCount--;
            }

            if(x=="print")
                printFunction(line);
            else if(x=="input")
                inputFunction(line);
            else if(x=="if")
                ifFunction(line);   //this function is for if
            else if(x=="else")
                elseFunction(line); //this function is for both else and else if
            else if(isDataType(line)==true)
                declareVariable(line);
            else
                statement(line);
            //cout<<ln++<<" done\n";
        }
        while(curlCount--)
        {
            algoIndent.pop();
            indent.pop();
            string k(algoIndent.top()+4, ' ');
            indent.push(k);
            code<<indent.top()<<"}\n";
        }
    }
}
    //categorize every line of the algorithm
    //and sends it to specific function

void codeOutput()
{
    ifstream aaaaa("code.txt");
    if(aaaaa.is_open())
    {
        cout<<aaaaa.rdbuf();
    }
    aaaaa.close();
}
    //prints the generated code in terminal
int main()
{
    cout<<"PseudoCoder"<<endl;
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
    code<<"    return 0;\n}";
    code.close();
    codeOutput();
    return 0;
}
