#include<bits/stdc++.h>
using namespace std;

fstream schemafile;

void createTable(vector<string> args){

    schemafile.open("Schema.txt",ios::app);

    string table_name=args[3];
    schemafile<<table_name;

    int start=-1,end=-1;
    for(int i=3;i<args.size();i++){
            if(args[i]=="("){
                start=i;
            }
            if(args[i]==")"){
                end=i;
            }
    }

    if(start==-1 || end==-1){
        cout<<"Error"<<endl;
        return;
    }
   
    while(start<end-1){
        start++;
        if(args[start]!=","){
            schemafile<<"#"<<args[start];
        }
    }
    schemafile<<endl;
    cout<<"Table created successfully"<<endl; 

    schemafile.close();     
}

void dropTable(vector<string> args){
    schemafile.open("Schema.txt");

    ofstream temp("temp.txt");

    string table_name=args[3];
    string line;

    while(getline(schemafile,line)){
        string l1=line.substr(0,line.find('#'));
        if(table_name!=l1){
            temp<<line<<endl;
        }
    }

    schemafile.close();
    temp.close();

    remove("Schema.txt");
    rename("temp.txt","Schema.txt");
    cout<<"Table dropped successfully"<<endl;
}

void helpTable(){
    string line;
    schemafile.open("Schema.txt");

    int count=0;
    while(getline(schemafile,line)){
        string l1=line.substr(0,line.find('#'));
        cout<<l1<<endl;
        count++;
    }
    if(!count){
        cout<<"No tables found"<<endl;
    }
}

int main( int argc, char **argv ) {

    vector<string> args(argv, argv+argc);

    if(args[1]=="create" && args[2]=="table"){
        createTable(args);
    }else if(args[1]=="drop" && args[2]=="table"){
        dropTable(args);
    }
    else if(args[1]=="help" && args[2]=="table"){
        helpTable();
    }
    else{
        cout<<"Syntax Error";
    }
}