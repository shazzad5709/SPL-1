#include<iostream>
#include<map>
#include<fstream>

using namespace std;

fstream code("code.txt", ios::in | ios::out);

bool isDelimiter(char x)
{
    if(x==' ' || x=='(' || x==')' || x=='{'
    || x=='}' || x=='['|| x==']'|| x=='\n')
        return true;
    return false;
}

void printFunction(string line)
{
    int i=7;
    code<<"    cout <<";
    while(i<line.length())
    {
        if(line[i]=='"')
        {
            i++;
            //code<<"    cout << \"";
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
            //code<<"    cout<< ";
            while((line[i]>='a' && line[i]<='z')
                || (line[i]>='A' && line[i]<='Z') || line[i]=='_')
                code<<line[i++];
        }
        
    }
    code<<";";
    return;
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
        }
    }

}

void code_output()
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
    parse();
    code<<"\n    return 0;\n}";
    code.close();
    code_output();
    //cout<<code.rdbuf();
    //code.close();
    return 0;
}
