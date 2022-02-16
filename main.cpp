#include<iostream>
#include<fstream>
#include<string>
#include<stack>

using namespace std;

struct node
{
    string data;
    struct node *next;
};

void addInStart (string text)
{

}

void addAfter (int linenumber, string text)
{

}

void addInBetween (int linenumber, string text)
{
    
}

int main()
{
    struct node *head;
    struct node *tail;
    int totallines=0;
    int next = 1;
    ofstream outfile;

    int choice =- 1;
    head = tail = NULL;


    while(choice != 0)
    {
        cout<<"Please choose what you want to do: "<<endl;
        cout<<"1. Insert text in Line N\n";
        cout<<"2. Delete line N\n";
        cout<<"3. Move line N into line M\n";
        cout<<"4. Replace text in line N\n";
        cout<<"5. Print all\n";
        cout<<"6. Save into existing .txt file\n";
        cout<<"7. Undo\n";
        cout<<"8. Create a new .txt file\n";
        cout<<"9. Print the next page\n";
        cout<<"10. Print the previous page\n";
        cin>>choice;
        if(choice == 1)
        {
            int linenumber;
            string text;
            cout<<"Line number where the text will be placed into: ";
            cin>>linenumber;
            cout<<"Enter text: ";
            cin.clear();
            cin.sync();
            getline(cin, text);
            text+="\n";
            if(linenumber == 1)
                addInStart(text);
            else if(linenumber > totallines)
                addAfter(linenumber, text);
            else
                addInBetween(linenumber, text);
        }
        if(choice == 2)
        {
            
        }

    }
    return 0;
}
