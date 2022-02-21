#include<iostream>
#include<fstream>
#include<cstring>
#include<map>
using namespace std;

map<string, bool> keywords;
map<string, bool> varDec;

bool declaredVar(string s)
{
  
    if(!varDec[s])
        return varDec[s]=true;
    else
        return false;
}

int isKeywords(string s)
{
	keywords["for"]=true;
	keywords["if"]=true;
    keywords["else if"]=true;
    keywords["else"]=true;
    keywords["while"]=true;
    keywords["do"]=true;
    keywords["break"]=true;
    keywords["continue"]=true;
    // keywords["if"]=2;
    // keywords["if"]=2;
}
  


bool substring(string line)
{
    // string s="print";
    // for(int i=0; i<5; i++)
    //     if(s[i]!=line[i])
    //         return 0;
    return 1;
}

void parse()
{
    string line;
    ifstream f;
    int i=7;
    f.open("huh.txt");
    if (f.is_open())
    {
        while (getline(f, line) )
        {
        if(substring(line))
        {
            cout<<"    cout << \"";
            while(line[i]!='"')
            cout<<line[i++];
            cout<<"\";";
        }
        }
        f.close();
    }
}


int main()
{
    cout<<"Algorithm to C++"<<endl;
    cout<<"-----------------"<<endl;
    string line;
    char c;
    ofstream f;
    f.open("huh.txt");
    getline(cin, line);
    f<<line;
    f.close();
    cout <<"---------------------------------------------------" << endl;
    cout<<"#include<iostream>"<<endl;
    cout<<"using namespace std;"<<endl;
    cout<<"int main()"<<endl<<"{"<<endl;
    parse();
    cout<<"\n    return 0;\n}";
}
