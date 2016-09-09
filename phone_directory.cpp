#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <bits/stdc++.h>    //for map and vector
using namespace std;

struct node{
    string name;
    string email;
};
map<string, node> phone_record;
map<string, vector<string>> name_record;
map<string, vector<string>> mail_record;

void add_record();
void add_file();

void search_record();
void search_phone(string);
void search_name(string);
void search_mail(string);

void delete_record();
void delete_phone(string);
void delete_name(string);
void delete_mail(string);

void load_file();
void file_update();
void index_record();

regex exp_mail ("\^[a-z0-9][a-z0-9\_.]+@[a-z0-9_]+.[a-z0-9_.]{2,4}\$");
regex exp_phone ("\^[1-9]+[0-9]*\$");
regex exp_name ("\[a-zA-Z]+( [a-zA-Z]+)*\$");

int main()
{
    load_file();
    index_record();

    int choice;
    while(1)
    {
        cout << "\n\n\t\t\t\t\t- - - - - Phone Directory - - - - -";
        cout << "\n1. Add new contact \n2. Search in contact list \n3. Delete contact \n4. Exit from phone directory \t";
        cin >> choice;
        if(choice == 1)
            add_record();
        else if(choice == 2)
            search_record();
        else if(choice == 3)
            delete_record();
        else if(choice == 4)
            return 0;
        else
            cout <<"\nInvalid choice! Try again.";
    }
    return 0;
}
void add_record()
{
    string mob, nm, em, a;
    cin.clear();
    cin.sync();

    cout << "\nEnter name : ";
    getline(cin, nm);
    while(!regex_match(nm, exp_name))   {
        cout << "Invalid name format! \nEnter space separated strings only : \t";
        getline(cin, nm);
    }
    cout << "\nEnter phone number : ";
    cin >> mob;
    while( !regex_match(mob, exp_phone) && (mob.length()<10 || mob.length()>12) )   {
        cout << "Invalid phone number! \nEnter 10-12 digits : \t";
        cin >> mob;
    }
    cout << "\nEnter email address : ";
    cin >> em;
    while(!regex_match(em, exp_mail))   {
        cout << "Invalid email address! \nEnter in this format \"[a-z,0-9]@[a-z,0-9].[a-z,0-9]\" : \t";
        cin >> em;
    }

    node temp;
    temp.name = nm;
    temp.email = em;
    phone_record[mob] = temp;

    mail_record[em].push_back(mob);
    name_record[nm].push_back(mob);

    ofstream file("phone_directory.csv", ios::app);
    file << mob <<','<< em <<','<< nm <<'\n';
    file.close();
}
void load_file()
{
    ifstream file("phone_directory.csv", ios::in);
    string record;
    while(getline(file, record))
    {
        stringstream lineStream(record);
        string mobile, email, name;
        getline(lineStream, mobile, ',');
        getline(lineStream, email, ',');
        getline(lineStream, name);

        node temp;
        temp.name = name;
        temp.email = email;
        phone_record[mobile] = temp;
    }
    file.close();
}
void index_record()
{
    map<string, node> :: iterator it;
    for(it= phone_record.begin(); it!=phone_record.end(); it++)
    {
        mail_record[(it->second).email].push_back(it->first);
        name_record[(it->second).name].push_back(it->first);
    }
}
void file_update()
{
    ofstream file("phone_directory.csv", ios::trunc);
    map<string, node> :: iterator it= phone_record.begin();

    while(it!=phone_record.end())
    {
        file << it->first <<','<< (it->second).email <<','<< (it->second).name <<'\n';
        it++;
    }
    file.close();
}


/// SEARCHING FUNCTIONS

void search_record()
{
    int choice;
    string s_num, s_name, s_mail;
    cout << "\n\n1. Search by phone number\n2. Search by name\n3. Search by email address\t";
    cin >> choice;
    if(choice == 1)
    {
        cout << "\nEnter phone number : ";
        cin >> s_num;
        while( !regex_match(s_num, exp_phone) && (s_num.length()<10 || s_num.length()>12) )   {
            cout << "Invalid phone number! \nEnter 10-12 digits : \t";
            cin >> s_num;
        }
        search_phone(s_num);
    }
    else if(choice == 2)
    {
        cout << "\nEnter name : ";
        cin.clear(); cin.sync();
        getline(cin, s_name);

        while(!regex_match(s_name, exp_name))   {
            cout << "Invalid name format! \nEnter space separated strings only : \t";
            getline(cin, s_name);
        }
        search_name(s_name);
    }
    else if(choice == 3)
    {
        cout << "\nEnter email address : ";
        cin >> s_mail;
        while(!regex_match(s_mail, exp_mail))   {
            cout << "Invalid email address! \nEnter in this format \"[a-z,0-9]@[a-z,0-9].[a-z,0-9]\" : \t";
            cin >> s_mail;
        }
        search_mail(s_mail);
    }
    else
        cout << "\nInvalid Choice!";
}
void search_phone(string num)
{
    if(phone_record.find(num)==phone_record.end())
        cout << "\nContact does not exist!";
    else
        cout <<"\nContact Found\n"<< num <<'\t'<< phone_record[num].name <<'\t'<< phone_record[num].email;
    cout<<endl;
}
void search_name(string name)
{
    if(name_record.find(name)==name_record.end())
        cout << "\nContact does not exist!";
    else
    {
        cout <<"\nContact(s) Found";
        for (int i=0; i<name_record[name].size(); ++i)
            cout <<'\n'<< name_record[name][i] <<'\t'<< name <<'\t'<< phone_record[name_record[name][i]].email;
    }
    cout<<endl;
}
void search_mail(string mail)
{
    if(mail_record.find(mail)==mail_record.end())
        cout << "\nContact does not exist!";
    else
    {
        cout <<"\nContact(s) Found";
        for (int i=0; i<mail_record[mail].size(); ++i)
            cout <<'\n'<< mail_record[mail][i] <<'\t'<< phone_record[mail_record[mail][i]].name <<'\t'<< mail ;
    }
    cout<<endl;
}



/// DELETION FUNCTIONS

void delete_record()
{
    int choice;
    string s_num, s_name, s_mail;
    cout << "\n\n1. Delete by phone number\n2. Delete by name\n3. Delete by email address\t";
    cin >> choice;
    if(choice == 1)
    {
        cout << "\nEnter phone number : ";
        cin >> s_num;
        while( !regex_match(s_num, exp_phone) && (s_num.length()<10 || s_num.length()>12) )   {
            cout << "Invalid phone number! \nEnter 10-12 digits.\t";
            cin >> s_num;
        }
        delete_phone(s_num);
    }
    else if(choice == 2)
    {
        cout << "\nEnter name : ";
        cin.clear(); cin.sync();
        getline(cin, s_name);

        while(!regex_match(s_name, exp_name))   {
            cout << "Invalid name format! \nEnter space separated strings only : \t";
            getline(cin, s_name);
        }
        delete_name(s_name);
    }
    else if(choice == 3)
    {
        cout << "\nEnter email address : ";
        cin >> s_mail;
        while(!regex_match(s_mail, exp_mail))   {
            cout << "Invalid email address! \nEnter in this format \"[a-z,0-9]@[a-z,0-9].[a-z,0-9]\" : \t";
            cin >> s_mail;
        }
        delete_mail(s_mail);
    }
    else
        cout << "\nInvalid Choice!";
}
void delete_phone(string num)
{
    if(phone_record.find(num)==phone_record.end())
        cout << "\nContact does not exist!\n";
    else
    {
        cout <<"\nAre you sure you want to delete this contact? (Y/N)\n"<< num <<'\t'<< phone_record[num].name <<'\t'<< phone_record[num].email<<'\n';
        char c; cin >> c;
        if(c=='Y' || c=='y')    {
            name_record.erase(phone_record[num].name);
            mail_record.erase(phone_record[num].email);
            phone_record.erase(num);
            file_update();
        }
    }
}
void delete_name(string name)
{
    if(name_record.find(name)==name_record.end())
        cout << "\nContact does not exist!";
    else
    {
        cout <<"\nAre you sure you want to delete these contacts? (Y/N)";
        for (int i=0; i<name_record[name].size(); ++i)
            cout <<'\n'<< name_record[name][i] <<'\t'<< name <<'\t'<< phone_record[name_record[name][i]].email;
        cout <<'\n';
        char c; cin >> c;
        if(c=='Y' || c=='y')
        {
            for (int i=0; i<name_record[name].size(); ++i)
            {
                mail_record.erase(phone_record[name_record[name][i]].email);
                phone_record.erase(name_record[name][i]);
                name_record.erase(name);
            }
            file_update();
        }
    }
}
void delete_mail(string mail)
{
    if(mail_record.find(mail)==mail_record.end())
        cout << "\nContact does not exist!";
    else
    {
        cout <<"\nAre you sure you want to delete these contacts? (Y/N)";
        for (int i=0; i<mail_record[mail].size(); ++i)
            cout <<'\n'<< mail_record[mail][i] <<'\t'<< phone_record[mail_record[mail][i]].name <<'\t'<< mail <<'\n';
        cout <<'\n';
        char c; cin >> c;
        if(c=='Y' || c=='y')
        {
            for (int i=0; i<mail_record[mail].size(); ++i)
            {
                name_record.erase(phone_record[mail_record[mail][i]].name);
                phone_record.erase(mail_record[mail][i]);
                mail_record.erase(mail);
            }
            file_update();
        }
    }
}
