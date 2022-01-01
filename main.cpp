#include <bits/stdc++.h>
using namespace std;

fstream schemafile;

void fetchSchema(string tableName, vector<string> &schema);

bool checkTable(string tableName){
    schemafile.open("Schema.txt", ios::in);
    string line;
    bool flag=false;
    while(getline(schemafile,line)){
        if(line.substr(0, line.find('#'))==tableName){
            flag=true;
            break;
        }
    }
    schemafile.close();
    return flag;
}

void createTable(vector<string> cmd){

    string table_name = cmd[2];

    if(checkTable(table_name)){
        cout<<"Table Already Exists"<<endl;
        return;
    }

    schemafile.open("Schema.txt", ios::app);
    schemafile << table_name;


    int start = -1, end = -1;
    for (int i = 3; i < cmd.size(); i++)
    {
        if (cmd[i] == "(")
        {
            start = i;
        }
        if (cmd[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        cout << "Error" << endl;
        return;
    }

    while (start < end - 1)
    {
        start++;
        if (cmd[start] != ",")
        {
            schemafile << "#" << cmd[start];
        }
    }
    schemafile << endl;
    cout << "Table created successfully" << endl;

    schemafile.close();
}

void dropTable(vector<string> cmd){
    
    string table_name = cmd[2];
    if(!checkTable(table_name)){
        cout<<"Table Not Exists"<<endl;
        return;
    }

    schemafile.open("Schema.txt", ios::in);

    fstream temp;
    temp.open("temp.txt", ios::out);

    string line;

    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (table_name != l1)
        {
            temp << line << endl;
        }
    }

    schemafile.close();
    temp.close();

    remove("Schema.txt");
    rename("temp.txt", "Schema.txt");

    string f = table_name + ".txt";
    char fileName[f.length()];
    
    for(int i=0;i<f.length();i++){
        fileName[i] = f[i];
    }
    fileName[f.length()]='\0';

    // cout<<fileName<<endl;
    remove(fileName);
    
    cout << "Table dropped successfully" << endl;
}

void describe(vector<string> cmd){
    schemafile.open("Schema.txt", ios::in);

    if (cmd.size() > 1)
    {
        string table_name = cmd[1];
        string line;

        while (getline(schemafile, line))
        {
            string l1 = line.substr(0, line.find('#'));

            if (table_name == l1)
            {
                string l2 = line.substr(line.find('#') + 1, line.size());
                int cnt = 1;

                for (auto i : l2)
                {
                    if (i == '#')
                    {
                        if (cnt % 2 != 0)
                        {
                            cout << "--";
                        }
                        else
                        {
                            cout << endl;
                        }
                        cnt++;
                    }
                    else
                    {
                        cout << i;
                    }
                }
            }
        }
    }
    else
    {
        cout << "please mention table name" << endl;
    }
    schemafile.close();
}

void insert(vector<string> cmd){
    string table_name = cmd[2];
    if(!checkTable(table_name)){
        cout<<"Table Not Exists"<<endl;
        return;
    }
    fstream table;
    table.open(table_name + ".txt", ios::app);

    int start = -1, end = -1;
    for (int i = 4; i < cmd.size(); i++)
    {
        if (cmd[i] == "(")
        {
            start = i;
        }
        if (cmd[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        cout << "Error" << endl;
        return;
    }
    while (start < end - 1)
    {
        start++;
        if (cmd[start] != ",")
        {
            table << cmd[start];
            if (start != end - 1)
            {
                table << "#";
            }
            else
            {
                table << "\n";
            }
        }
    }
}

void table_number(map<string, int>& table, vector<string> schema){
    int cnt = 0;
    for(int j=1; j<schema.size(); j+=2){
        table[schema[j]] = cnt;
        cnt++;
    }
}

//Check column of table
void checkCommand(map<int, string>& column_change, vector<string> cmd, vector<string> schema){
    int i;
    map<string, int> table;
    table_number(table, schema);
    for(i=3; i<cmd.size() && cmd[i]!="where"; i+=3){
        int col = table[cmd[i]];
        column_change[col] = cmd[i+2];
    }
}

//Update function
void update(vector<string> cmd) {

    if(!checkTable(cmd[1])){
        cout<<"Table Not Exists"<<endl;
        return;
    }

    vector<string> schema;
    fetchSchema(cmd[1], schema);

    if(!schema.empty()){
        string table_name = cmd[1] + ".txt";
        fstream table;
        const char *c = table_name.c_str();
        table.open(c, ios::in);
        vector<string>::iterator it;
        it = find(cmd.begin(), cmd.end(), "where");
        map<int, string> column_change;
        checkCommand(column_change, cmd, schema);

        string line;
        vector<string> lineVec;
        int i, j=0;
        int flag = 0;
        int count = 0;
        fstream temp;
        temp.open("temp.txt", ios::out);
        //where condition is not in command
        if(it == cmd.end()){
            while(getline(table, line)){
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == '#'){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                string new_line = "";
                map<int, string>::iterator itr;
                for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                    lineVec[itr->first] = itr->second;
                }

                for(i=0; i<lineVec.size()-1; i++){
                    new_line += lineVec[i]; 
                    new_line += "#";
                }
                new_line += lineVec[i];
                temp << new_line << endl;
                lineVec.clear();
            }
        }
        //where condition is in command
        else{
            while (getline(table, line)) {
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == '#'){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                int idx = 0;
                idx = it - cmd.begin();
                j=0;
                for(i=1; i<schema.size(); i+=2){
                    if(cmd[idx+1] == schema[i]){
                        //Equality condition
                        if(cmd[idx+2] == "="){
                            if(lineVec[j] == cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than condition
                        if(cmd[idx+2] == ">"){
                            if(lineVec[j] > cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than or equal to
                        if(cmd[idx+2] == ">="){
                            if(lineVec[j] >= cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than condition
                        if(cmd[idx+2] == "<"){
                            if(lineVec[j] < cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than and equal to
                        if(cmd[idx+2] == "<="){
                            if(lineVec[j] <= cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Not equal to condition
                        if(cmd[idx+2] == "!="){
                            if(lineVec[j] != cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                } 
                //Not making any changes in the row
                if(flag != 1){
                    temp << line << endl;
                    lineVec.clear();
                }
                //make changes in a row 
                else{
                    string new_line = "";
                    map<int, string>::iterator itr;
                    for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                        lineVec[itr->first] = itr->second;
                    }

                    for(i=0; i<lineVec.size()-1; i++){
                        new_line += lineVec[i]; 
                        new_line += "#";
                    }
                    new_line += lineVec[i];
                    temp << new_line << endl;
                    lineVec.clear();
                }
                flag = 0;
            }  
        }
        table.close();
        temp.close();
        remove(c);
        rename("temp.txt", c);
        cout<<"Rows updated successfully"<<endl;
    }
    cout << "\n" << endl;
}

void delete_(vector<string> cmd){

    vector<string> schema;
    if(!checkTable(cmd[2])){
        cout<<"Table Not Exists"<<endl;
        return;
    }
    fetchSchema(cmd[2], schema);
    int count = 0;

    if (!schema.empty())
    {
        if (cmd.size() == 3)
        {
            string table = cmd[2] + ".txt";
            char c[table.size() + 1];
            strcpy(c, table.c_str());
            remove(c);
        }
        else if (cmd[3] == "where")
        {
            string table_name = cmd[2];
            fstream table;
            table.open(table_name + ".txt", ios::in);
            string line;

            int i, j;
            j = 0;
            int flag = 0;
            vector<string> lineVec;

            fstream temp;
            temp.open("temp.txt", ios::out);

            while (getline(table, line))
            {
                stringstream ss(line);
                while (ss.good())
                {
                    string substr;
                    getline(ss, substr, '#');
                    lineVec.push_back(substr);
                }

                for (i = 1; i < schema.size(); i += 2)
                {
                    if (cmd[4] == schema[i])
                    {
                        if (cmd[5] == "=")
                        {
                            if (cmd[6] == lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == ">")
                        {
                            if (lineVec[j] < cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "<")
                        {
                            if (lineVec[j] < cmd[6] )
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == ">=")
                        {
                            if ( lineVec[j] >= cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "<=")
                        {
                            if (lineVec[j] <= cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "!=")
                        {
                            if (cmd[6] != lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }
                if (flag != 1)
                {
                    temp << line << endl;
                }
                flag = 0;
            }
            table.close();
            temp.close();
            string table1 = table_name + ".txt";
            char c[table1.size() + 1];
            strcpy(c, table1.c_str());
            remove(c);
            rename("temp.txt", c);
        }
        cout << " rows deleted" << endl;
    }
}

void fetchTable(string& tableName,vector<string>& att,unordered_map<string,int>& mp,
    vector<vector<string>>& d){
    fstream table(tableName+".txt",ios::in);
    string line;

    cout<<"\n";
    for(auto x:att) cout<<x<<"\t";
    cout<<"\n\n";

    while(getline(table,line)){
        stringstream ss(line);
        vector<string> data;

        while (ss.good()){
            string substr;
            getline(ss, substr, '#');
            data.push_back(substr);
        } 

        d.push_back(data);
        
    }

}
 
void select(vector<string> cmd){
  
    vector<string> attr_list;
   
    auto it = find(cmd.begin(), cmd.end(),"from");
    int index = 0; // finding the index of from clause

    if(it != cmd.end()){
        index = it - cmd.begin();
    }else{
        cout << "Syntax error" << endl;
    }
    
     int tablename_pos = index + 1;  // position of table_name

    if(!checkTable(cmd[tablename_pos])){
        cout<<"Table Not Exists"<<endl;
        return;
    }

    for(int i=1;i<index;i+=2){
        attr_list.push_back(cmd[i]);
    }


   

    unordered_map<string,int> table_attr;
    vector<string> schema;

    fetchSchema(cmd[tablename_pos],schema);

    int k=0;
    for(int i=1;i<schema.size();i+=2){
        table_attr[schema[i]]=k++;
    }

    bool atterror=false;
    bool attError=false;
    for(auto x:attr_list){
        if(table_attr.find(x)==table_attr.end()){
            cout<<"Attribute Error"<<endl;
            attError=true;
            break;
        }
    }

    if(attError){
        cout<<" Attribute Error"<<endl;
        return;
    } 
    
    vector<vector<string>> data;
    fetchTable(cmd[tablename_pos],attr_list,table_attr,data);

    auto itw = find(cmd.begin(), cmd.end(),"where");
    int indexofwhere = 0; // finding the index of where clause
    if (itw != cmd.end()){
        indexofwhere = itw - cmd.begin();
        string cnd_att=cmd[indexofwhere+1];
        string cnd=cmd[indexofwhere+2];
        string cnd_value=cmd[indexofwhere+3];

        int att_ind=table_attr[cnd_att];

        for(int k=0;k<data.size();k++){
            if((cnd=="=" && data[k][att_ind]==cnd_value)
            || (cnd==">" && data[k][att_ind]>cnd_value)
            || (cnd=="<" && data[k][att_ind]<cnd_value)
            || (cnd==">=" && data[k][att_ind]>=cnd_value)
            || (cnd=="<=" && data[k][att_ind]<=cnd_value)
            || (cnd=="!=" && data[k][att_ind]!=cnd_value)){
                for(int i=0;i<attr_list.size();i++){
                    cout<<data[k][table_attr[attr_list[i]]]<<"\t";
                }
                cout<<endl;
            }
        }
        
    }else {
        for(int k=0;k<data.size();k++){
            for(int i=0;i<attr_list.size();i++){
                cout<<data[k][table_attr[attr_list[i]]]<<"\t";
            }
            cout<<endl;
        }
    }
     
}

void helpTable(){
    string line;

    schemafile.open("Schema.txt");

    int count = 0;
    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find('#'));
        cout << l1 << endl;
        count++;
    }
    if (!count)
    {
        cout << "No tables found" << endl;
    }
}

void helpCmd(vector<string> cmd){
    cout << "\n------------HELP----------------" << endl;
    map<string, int> help{{"createtable", 1}, {"droptable", 2}, {"select", 3}, {"insert", 4}, {"delete", 5}, {"update", 6}};
    map<string, int>::iterator it1;
    string command = cmd[1] + (cmd.size() > 2 ? cmd[2] : "");
    it1 = help.find(command);

    switch (it1->second)
    {
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
        cout << "Syntax Error";
    }
}

void convertToVector(string input, vector<string> &v){
    string word = "";
    for (auto x : input)
    {
        if (x == ' ')
        {
            v.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    v.push_back(word);
}

void fetchSchema(string tableName, vector<string> &schema){
    schemafile.open("Schema.txt", ios::in);
    string line;
    int flag = 0;

    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (tableName == l1){
            flag = 1;
            stringstream ss(line);

            while (ss.good()){
                string substr;
                getline(ss, substr, '#');
                schema.push_back(substr);
            }
            break;
        }
    }
    if (flag == 0){
        cout << "table not found" << endl;
    }

    schemafile.close();
}

void handleCmd(vector<string> cmd){
    if (cmd[0] == "create" && cmd[1] == "table")
    {
        createTable(cmd);
    }
    else if (cmd[0] == "drop" && cmd[1] == "table")
    {
        dropTable(cmd);
    }
    else if (cmd[0] == "help" && cmd[1] == "tables")
    {
        helpTable();
    }
    else if (cmd[0] == "help" && cmd[1] != "tables")
    {
        helpCmd(cmd);
    }
    else if (cmd[0] == "insert" && cmd[1] == "into")
    {
        insert(cmd);
    }
    else if (cmd[0] == "describe")
    {
        describe(cmd);
    }
    else if(cmd[0] == "select"){
        select(cmd);
    }
    else if (cmd[0] == "delete" && cmd[1] == "from")
    {
        delete_(cmd);
    }
    else if(cmd[0] == "update" && cmd[2]== "set"){
        update(cmd);
    }
  
    else
    {
        cout << "Syntax Errors";
    }
}

int main(){

    vector<string> cmd;
    string input;

    cout << "Grp14-DMS>";

    getline(cin, input);


    while (input != "q")
    {

        convertToVector(input, cmd);
        handleCmd(cmd);

        cmd.clear();
        cout << "\nGrp14-DMS>";

        getline(cin, input);


    }

    return 0;
}