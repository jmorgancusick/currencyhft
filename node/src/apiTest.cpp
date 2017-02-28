#include "sql/api.h"
#include "string"

using namespace std;

int main(){

  cout<<"Hello World!"<<endl;
  
  API *db = new API();
  int retVal = db->connect();
  unordered_map<string, double> *rows = db->selectAllFromTable("stock");

  for(unordered_map<string, double>::iterator itr = rows->begin(); itr != rows->end(); itr++){
    cout<< itr->first << "\t" << itr->second << endl;

  }

  delete db;
}
