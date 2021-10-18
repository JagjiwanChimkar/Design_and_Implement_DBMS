#include<bits/stdc++.h>
using namespace std;

fstream schemafile;

void createTable(vector<string> cmd){

    schemafile.open("Schema.txt",ios::app);

    string table_name=cmd[2];
    schemafile<<table_name;

    int start=-1,end=-1;
    for(int i=3;i<cmd.size();i++){
            if(cmd[i]=="("){
                start=i;
            }
            if(cmd[i]==")"){
                end=i;
            }
    }

    if(start==-1 || end==-1){
        cout<<"Error"<<endl;
        return;
    }
   
    while(start<end-1){
        start++;
        if(cmd[start]!=","){
            schemafile<<"#"<<cmd[start];
        }
    }
    schemafile<<endl;
    cout<<"Table created successfully"<<endl; 

    schemafile.close();     
}

void dropTable(vector<string> cmd){
    schemafile.open("Schema.txt");

    ofstream temp("temp.txt");

    string table_name=cmd[3];
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

void helpCmd(vector<string> cmd){
    cout << "\n------------HELP----------------" << endl;
    map<string, int> help{{"createtable",1},{"droptable",2},{"select",3},{"insert",4},{"delete",5},{"update",6}};
    map<string, int>::iterator it1;
    string command=cmd[1]+(cmd.size()>2?cmd[2]:"");
    it1 = help.find(command);
    
    switch (it1->second){
    case 1:
        
        cout << "\nCommand : CREATE TABLE" << endl;
        cout << "Info: Creates a new table" << endl;
        cout << "\nFormat: \nCREATE TABLE table_name ( attribute_1 attribute1_type CHECK (constraint1), \nattribute_2 attribute2_type, …, PRIMARY KEY ( attribute_1, attribute_2 ), \nFOREIGN KEY ( attribute_y ) REFERENCES table_x ( attribute_t ), \nFOREIGN KEY ( attribute_w ) REFERENCES table_y ( attribute_z )...);" << endl;
        break;

    case 2:
        cout << "\nCommand : DROP TABLE" << endl;
        cout << "Info: Deletes a table" << endl;
        cout << "\nFormat: \nDROP TABLE table_name;" << endl;
        break;
    
    case 3:
        cout << "\nCommand : SELECT" << endl;
        cout << "Info: Extracts data from a database" << endl;
        cout << "\nFormat: \nSELECT attribute_list FROM table_list WHERE condition_list;" << endl;
        break;
    
    case 4:
        cout << "\nCommand : INSERT" << endl;
        cout << "Info: Inserts new data into a database" << endl;
        cout << "\nFormat: \nINSERT INTO table_name VALUES ( val1, val2, ... );" << endl;
        break;
    
    case 5:
        cout << "\nCommand : DELETE" << endl;
        cout << "Info: Deletes data from a database" << endl;
        cout << "\nFormat: \nDELETE FROM table_name WHERE condition_list;" << endl;
        break;
    
    case 6:
        cout << "\nCommand : UPDATE" << endl;
        cout << "Info: Updates data in a database" << endl;
        cout << "\nFormat: \nUPDATE table_name SET attr1 = val1, attr2 = val2… WHERE condition_list;" << endl;
        break;
    
    default:
        cout<<"Syntax Error";
    }
}

void convertToVector(string input,vector<string> &v){
    string word = "";
    for (auto x : input) 
    {
        if (x == ' ')
        {
            v.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    v.push_back(word);
}

void handleCmd(vector<string> cmd){
    if(cmd[0]=="create" && cmd[1]=="table"){
        createTable(cmd);
    }else if(cmd[0]=="drop" && cmd[1]=="table"){
        dropTable(cmd);
    }
    else if(cmd[0]=="help" && cmd[1]=="table"){
        helpTable();
    }
    else if(cmd[0]=="help" && cmd[1]!="table"){
        helpCmd(cmd);
    }
    else{
        cout<<"Syntax Error";
    }
}

int main() {

    vector<string> cmd;
    string input;
    
    cout<<"Grp14-DMS>";
    
    getline(cin,input);
   
    cout<<"input:"<<input<<endl;

    while(input!="quit"){

        convertToVector(input,cmd);
        handleCmd(cmd);
        

        cmd.clear();
        cout<<"\nGrp14-DMS>";
    
        getline(cin,input);
   
        cout<<"input:"<<input<<endl;
    }
    
    return 0;
}