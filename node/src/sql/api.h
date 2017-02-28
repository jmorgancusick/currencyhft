#ifndef API_H
#define API_H

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <vector>
#include <unordered_map>
#include <string>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define HOST "localhost"
#define USER "worlduser"
#define PASS "worldpass"
#define DB "world"

using namespace std;

class API{
 public:
  //===========Constructors===========
  //API();
  ~API(){
    con.reset();
  }
  
  int connect(){
    //open credentials file
    ifstream myfile("../../credentials.txt");
    if (myfile.is_open()){
	getline(myfile,host);
	getline(myfile,user);
	getline(myfile,pass);
	getline(myfile,db);
	cout<<host<<" , "<<user<<" , "<<pass<<" , "<<db<<endl;

	
	myfile.close();

	cout << "Read credentials" << endl;

	try{
	  
	  driver = get_driver_instance();

	  con.reset(driver->connect(host, user, pass));
	  con->setSchema(db);

	  cout << "Connected to DB" << endl;
	  
	} catch(sql::SQLException &e){
	  printError(e);
	  con.reset();
	  return EXIT_FAILURE;
	}
    }
    else{
      cout << "Unable to open file" << endl;
    }
  }

  //only works for string, double pairs
  unordered_map<string, double> * selectAllFromTable(string table){
    unordered_map<string, double> *rows = new unordered_map<string, double>();

    cout<<"table: "<<table<<endl;
    try{
      
      pstmt.reset(con->prepareStatement("SELECT * from stock"));
      //sql::SQLString sqlStr("stock");
      //pstmt->setString(1,table);
      //pstmt->setString(1,"stock");
      //pstmt->setInt(1,10);
      
      res.reset(pstmt->executeQuery());

      for(;;)
	{
	  while (res->next()) {
	    (*rows)[res->getString("id")] = res->getDouble("close");
	  }
	  if (pstmt->getMoreResults())
	    {
	      res.reset(pstmt->getResultSet());
	      continue;
	    }
	  break;
	}
    } catch(sql::SQLException &e){
      printError(e);
      delete rows;
      return NULL;
    }
    return rows;
  }

 private:
  string host;
  string user;
  string pass;
  string db;


  sql::Driver * driver;
 
  std::auto_ptr< sql::Connection > con;
  
  std::auto_ptr< sql::PreparedStatement >  pstmt;
  std::auto_ptr< sql::ResultSet > res;
 
  void printError(sql::SQLException &e){
    /*
      MySQL Connector/C++ throws three different exceptions:

      - sql::MethodNotImplementedException (derived from sql::SQLException)
      - sql::InvalidArgumentException (derived from sql::SQLException)
      - sql::SQLException (derived from std::runtime_error)
    */
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    /* what() (derived from std::runtime_error) fetches error message */
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
};


#endif
