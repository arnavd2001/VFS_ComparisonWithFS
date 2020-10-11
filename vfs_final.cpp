#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <string>
#include <math.h>
using namespace std;
 
#define MAX_FILES 16
#define MAX_FILE_LEN 1000
 
class file
{
    public:
    char name[64];
    long int len;
    int startpos;
   
    char *get_file_name();
    long int get_file_length();
    int get_startpos();
};
 
char *file::get_file_name()
{
    return name;
}
 
long int file::get_file_length()
{
    return len;
}
 
int file::get_startpos()
{
    return startpos;
}
 
class filesys
{
    file files[MAX_FILES];
    char file_system_name[20];
 
    public:
    filesys()
    {
        strcpy(file_system_name, " ");
    }
 
    filesys(char f_name[]);
    void initialize();
    void read_from_file_system();
    void write_to_file_system();
    void set_file_system_name();
    void list_files();
    void search_file(char *f_name);
    void search_keyword(char *f_name, char *keyword);
    void delete_file(char *f_name);
    void create_file(char *f_name);
    void write_file(char *f_name);
    void read_file(char *f_name);
};
 
filesys::filesys(char f_name[])
{
    if(f_name == NULL)
        set_file_system_name();
    else
    {
        cout<<"\nExisting file system";
        strcpy(file_system_name, f_name);
        read_from_file_system();
    }
}
 
void filesys::set_file_system_name()
{
    char temp[80];
    cout<<"\nEnter file system name:\n";
    cin>>temp;
    strcpy(file_system_name, temp);
    initialize();
}
 
void filesys::initialize()
{
    fstream myfile(file_system_name, ios::out);
    int i;
    myfile.seekp(0, ios::beg);
    for(i = 0; i < MAX_FILES; i++)
    {
        strcpy(files[i].name, "\0");
        files[i].len = 0;
        files[i].startpos = 0;
    }
 
    myfile.close();
    write_to_file_system();
}
 
void filesys::list_files()
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (!strcmp(files[i].get_file_name(), "\0"))
            break;
 
        cout<<"\n"<<files[i].get_file_name();
    }
}
 
void filesys::search_file(char *f_name)
{
    for(int i = 0; i < MAX_FILES; i++)
    {
        if(!strcmp(files[i].get_file_name(), f_name))
        {
            cout<<"\nFile found";
            cout<<"\nFile name: "<<files[i].get_file_name();
            return;
        }
    }
    cout<<"\nFile not found!";
}
 
void filesys::search_keyword(char *f_name, char *keyword)
{
    fstream myfile(f_name, ios::in);
    for(int i = 0; i < MAX_FILES; i++)
    {
        if(!strcmp(f_name, files[i].get_file_name()))
        {
            char *file_content = new char[MAX_FILE_LEN];
            myfile.seekg(files[i].get_startpos());
            myfile.read(file_content, files[i].get_file_length());
            char *p = strstr(file_content, keyword);
            if(p == NULL)
            {
                cout<<"\nKeyword not Found!\n";
                myfile.close();
                return;
            }
            int pos = p - file_content + 1; // The difference between the address of substring in the string and
                                            // the base address plus 1
            cout<<"\nKeyword Found!\nPosition of keyword:\t"<<pos;
            myfile.close();
            return;
        }
    }
    cout<<"\nFile not found!";
    myfile.close();
}
 
void filesys::delete_file(char *f_name)
{
    int i, j;
    char *file_content = new char[MAX_FILE_LEN];
    for (i = 0; i < MAX_FILES; i++)
    {
        if (!strcmp(files[i].get_file_name(), f_name))
        {
            int del_len;
            strcpy(files[i].name, "\0");
            del_len = files[i].get_file_length();
            files[i].len = 0;
            files[i].startpos = 0;
            write_to_file_system(); //Will skip the content of the file to be deleted
 
            for (j = i + 1; j < MAX_FILES; j++)
            {
                strcpy(files[j - 1].name, files[j].get_file_name());
                files[j - 1].len = files[j].get_file_length();
                files[j - 1].startpos = files[j].get_startpos() - del_len;
            }
 
            write_to_file_system();
            cout<<"\nFile deleted!";
            return;
        }
    }
    cout<<"\nFile not found!";
}
 
void filesys::create_file(char *f_name)
{
    int i;
    for(i = 0; i < MAX_FILES; i++)
    {
        if(!strcmp(files[i].get_file_name(), "\0"))
            break;
    }
 
    if(i == MAX_FILES)
        cout<<"\nNo space available";
    else
    {
        strcpy(files[i].name, f_name);  
        files[i].len = 0;
        files[i].startpos = 0; 
    }
}
 
void filesys::write_file(char *f_name)
{
    int pos = -1;
    long file_len = 0;
    char c = ' ';
    for(int i = 0; i < MAX_FILES; i++)
    {
        if(!strcmp(files[i].get_file_name(),f_name))
        {
            pos = i;
            break;
        }
    }
 
    if(pos == -1)
    {
        cout<<"\nFile does not exist. Please create a file.";
        return;
    }
 
    ofstream fout;
    fout.open(f_name, ios::app);
    if(fout.is_open())
    {
        cout<<"\nEnter content to store in file (enter # to stop):\n";
        cin.ignore();
        while(cin.get(c) && (file_len <= MAX_FILE_LEN))
        {
            if(c == '#')
                break;
 
            file_len += 1;
            fout.put(c);
        } 
 
        files[pos].len += file_len;
        fout.seekp(0, ios::end);
        files[pos].startpos = fout.tellp();
        cout<<"\nFile length = "<<files[pos].get_file_length();
        cout<<"\nFile startpos = "<<files[pos].get_startpos();          
    }
    else
        cout<<"\nError opening file"; 
    
    fout.close();    
}
 
void filesys::read_file(char *f_name)
{
    int flag = 0;
    char ch;
    for(int i = 0; i < MAX_FILES; i++)
    {
        if(!strcmp(files[i].get_file_name(), f_name))
        {
            flag = 1;
            break;
        }
    }
 
    if(!flag)
    {
        cout<<"\nFile does not exist. Please create a file.";
        return;
    }
 
    ifstream fin;
    fin.open(f_name, ios::in);
    if(fin.is_open())
    {
        fin.seekg(0, ios::beg);
        cout<<"\n";
        while(fin >> noskipws >> ch)
        {
            cout<<ch;
        }
    }
}
 
void filesys::read_from_file_system()
{
    fstream myfile(file_system_name, ios::in);
    int i;
    myfile.seekg(0, ios::beg);
 
    for(i = 0; i < MAX_FILES; i++) //read already created files till null string is encountered or max limit
    {
        myfile.read((char *)&(files[i].name), sizeof(files[i].name));
        if (!strcmp(files[i].name, "\0"))
            break;
 
        myfile.read((char *)&files[i].len, sizeof(long int));
        myfile.read((char *)&files[i].startpos, sizeof(int));
    }
 
    while(i < MAX_FILES)
    {
        strcpy(files[i].name, "\0");
        files[i].len = 0;
        files[i].startpos = 0;
        i++;
    }
 
    myfile.close();
}
 
void filesys::write_to_file_system()
{
    fstream myfile(file_system_name, ios::in);
    fstream newfile("temp.txt", ios::out);
    int i;
    char file_content[MAX_FILE_LEN];
    newfile.seekp(0, ios::beg);
 
    for(i = 0; i < MAX_FILES; i++)
    {
        newfile.write((char *)&files[i].name, sizeof(files[i].name));
        newfile.write((char *)&files[i].len, sizeof(long int));
        newfile.write((char *)&files[i].startpos, sizeof(int));
    }
 
    for(i = 0; i < MAX_FILES; i++)
    {
        if(files[i].get_file_length())
        {
            myfile.seekg(files[i].get_startpos(), ios::beg);
            myfile.read((char *)&file_content, sizeof(char) * files[i].len);
            newfile.seekp(0, ios::end);
            newfile.write((char *)&file_content, sizeof(char) * files[i].len);
        }
    }
 
    newfile.close();
    myfile.close();
    remove(file_system_name);
    rename("temp.txt", file_system_name);
}
 
int main(int argc, char *argv[]) //Enter command line arguments to open a previously created file system
                                 // or leave blank to create a new one.
{
    char name[80], keyword[60], *c = NULL;
    cout<<"-----------------------------------------------------------------------------\n";
    cout<<"******** Virtual File System ***********\n";
    cout<< "-----------------------------------------------------------------------------\n";
    int choice;
    filesys f1(argv[1]);
 
    while(1)
    {
        cout<<"\nEnter your choice:\n";
        cout<<"1. List files in the file system\n";
        cout<<"2. Search for a file\n";
        cout<<"3. Search for a keyword in a file\n";
        cout<<"4. Create a new file\n";
        cout<<"5. Delete a file\n";
        cout<<"6. Write into a file\n";
        cout<<"7. Read from a file\n";
        
        cin>>choice;
        switch(choice)
        {
            case 1:
                f1.list_files();
                break;
            case 2:
                cout<<"\nEnter file name\n";
                cin>>name;
                f1.search_file(name);
                break;
            case 3:
                cout<<"\nEnter file name\n";
                cin>>name;
                cout<<"\nEnter the keyword to be searched\n";
                cin>>keyword;
                f1.search_keyword(name, keyword);
                break;
            case 4:
                cout<<"\nEnter the name of new file\n";
                cin>>name;
                f1.create_file(name);
                break;
            case 5:
                cout<<"\nEnter the name of the file to be deleted\n";
                cin>>name;
                f1.delete_file(name);
                break;
            case 6:
                cout<<"\nEnter file name to write:\n";
                cin>>name;
                f1.write_file(name);
                break;
            case 7:
                cout<<"\nEnter file name to read:\n";
                cin>>name;
                f1.read_file(name);
                break;
            default:
                cout<<"\nEnter a valid option!\n";
        }
 
        cout<<"\n\nEnter 0 to exit; 1 to continue\n";
        cin>>choice;
        if(!choice)
            break;
    }
 
    return 0;
}

