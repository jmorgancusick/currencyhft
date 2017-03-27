#ifndef API_H
#define API_H

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <math.h>

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
	  return 1;
	}
    }
    else{
      cout << "Unable to open file" << endl;
      return 2;
    }
    return 0;
  }


  struct chart_info {
    long timestamp;
    string ticker;
    double high;
    double volume;
    double open;
    double low;
    double close;
  };


  //only works for string, double pairs
  unordered_map<string, double> * SelectExchangeDailyHistory(string id, long date, int years){
    unordered_map<string, double> *rows = new unordered_map<string, double>();

    cout<<"id: "<<id<<endl;
    try{

      pstmt.reset(con->prepareStatement("select * 
                                        from forexDashDaily 
                                        where id=?, and timestamp>?, and timestamp<=?"));
      pstmt->setString(1,id);
      pstmt->setString(2,date - years*pow(10,10));
      pstmt->setString(3,date)
      
      res.reset(pstmt->executeQuery());

      for(;;){
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


  //only works for string, double pairs
  unordered_map<string, double> * selectExchangeWithID(string id){
    unordered_map<string, double> *rows = new unordered_map<string, double>();

    cout<<"id: "<<id<<endl;
    try{

      pstmt.reset(con->prepareStatement("select * from stock where id=?"));
      pstmt->setString(1,id);
      
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

    //only works for string, double pairs
  unordered_map<string, double> * selectAllFromTable(string table){
    unordered_map<string, double> *rows = new unordered_map<string, double>();

    try{

      if(!hasTable(table)){
	cout << "No table named: " << table << " Exists in tablesWhiteList" << endl;
	return NULL; 
      }

      
      pstmt.reset(con->prepareStatement("select * from "+table));
      
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


unordered_map<string, double> * selectAllTickerData(){
    unordered_map<string, double> *rows = new unordered_map<string, double>();

    try{
      
      pstmt.reset(con->prepareStatement("select * from forex"));
      
      //res now has return data
      res.reset(pstmt->executeQuery());

      for(;;)
  {
    while (res->next()) {
      (*rows)[res->getString("ticker")] = res->getDouble("change_day");
    }
    if (pstmt->getMoreResults())
      {
        res.reset(pstmt->getResultSet());
        continue;
      }
    break;  //No more results
  }
    } catch(sql::SQLException &e){
      printError(e);
      delete rows;
      return NULL;
    }
    return rows;
  }

vector<chart_info> * selectHistoricalTickerData(string ticker, string interval){
    vector<chart_info> *rows = new vector<chart_info>();

    try{
      //mysql query
      pstmt.reset(con->prepareStatement("select * from forexDashMinute"));
      
      //res now has return data
      res.reset(pstmt->executeQuery());

      for(;;)
  {
    while (res->next()) {
      chart_info rowData;
      rowData.timestamp = res->getInt("timestamp");
      rowData.ticker = res->getString("ticker");
      rowData.high = res->getDouble("high");
      rowData.volume = res->getDouble("volume");
      rowData.open = res->getDouble("open");
      rowData.low = res->getDouble("low");
      rowData.close = res->getDouble("close");
      (*rows).push_back(rowData);
    }
    if (pstmt->getMoreResults())
      {
        res.reset(pstmt->getResultSet());
        continue;
      }
    break;  //No more results
  }
    } catch(sql::SQLException &e){
      printError(e);
      delete rows;
      return NULL;
    }
    return rows;
  }

  //retrieves all currencies to store in graph
  vector<string> GetAllCurrencies() {
    vector<string> currencies;
    try{
      //placeholder until db is filled
      pstmt.reset(con->prepareStatement("select distinct start from forex order by start"));
      res.reset(pstmt->executeQuery());

      while (res->next()) {
        currencies.push_back(res->getString("start"));
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
      return {};
    }
    return currencies;
  }

  //retrieves forex rate of a particular ticker
  double GetForexRate(string ticker) {
    //initialize as NaN
    double rate = numeric_limits<double>::quiet_NaN();
    try{
      //placeholder until db is filled
      pstmt.reset(con->prepareStatement("select rate from forex where ticker = ?"));
      pstmt->setString(1,ticker);

      res.reset(pstmt->executeQuery());
      res->next();

      rate = res->getDouble("rate");
    }
    catch(sql::SQLException &e) {
      printError(e);
    }
    return rate;
}


 private:
  string host;
  string user;
  string pass;
  string db;


  int numTables = 1;
  string tablesWhiteList[1] = {"stock"};


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

  bool hasTable(string tableName){
    for(int i = 0; i < numTables; i++){
      if(tableName.compare(tablesWhiteList[i]) == 0){
	return true;
      }
    }
    return false;
  }
  
};


#endif
