#ifndef API_H
#define API_H

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <stdio.h>

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
    	
    	myfile.close();

    	try{
    	  
    	  driver = get_driver_instance();

    	  con.reset(driver->connect(host, user, pass));
    	  con->setSchema(db);

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
    string date;
    long timestamp;
    string ticker;
    double high;
    double volume;
    double open;
    double low;
    double close;
  };

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

  //selects all rows from a table
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

//selects all data from the forex table
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
        break;  
        //No more results
      }
    } catch(sql::SQLException &e){
      printError(e);
      delete rows;
      return NULL;
    }
    return rows;
  }

//gets the historical data for a specified ticker
//in a certain timespan at a certain interval
vector<chart_info> * selectHistoricalTickerData(string ticker, string interval, string startDate, string endDate){
    vector<chart_info> *rows = new vector<chart_info>();

    try{
      //mysql query
      string table;
      if(interval == "minute"){
        table = "forexDashMinute";
      }
      else if(interval == "day"){
        table = "forexDashDaily";
      }
      else {
        cout << "# ERR: Improper input for variable interval in selectHistoricalTickerData" << endl;
        return NULL;
      }

      struct tm startTime = timeConversion(startDate);
      struct tm endTime = timeConversion(endDate);

      pstmt.reset(con->prepareStatement("select * from "+table+" where ticker=? and timestamp<=? and timestamp>? order by timestamp"));
      pstmt->setString(1,ticker);
      //converts the tm struct to an int
      pstmt->setInt(2,mktime(&endTime)); 
      pstmt->setInt(3,mktime(&startTime));

      //res now has return data
      res.reset(pstmt->executeQuery());

      for(;;){
        while (res->next()) {
          chart_info rowData;
          time_t t = res->getInt("timestamp");
          struct tm *date = gmtime(&t);
          char d[20];
          strftime(d, sizeof(d), "%m-%d-%Y+%H:%M:%S", date);
          rowData.date = d;
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
      pstmt.reset(con->prepareStatement("select distinct substring(ticker, 1, 3) as curr from forex"));
      res.reset(pstmt->executeQuery());

      while (res->next()) {
        currencies.push_back(res->getString("curr"));
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
      return {};
    }
    return currencies;
  }

  //retrieves all banks to store in graph
  vector<string> GetAllBanks() {
    vector<string> banks;
    try{
      pstmt.reset(con->prepareStatement("select distinct bank from bankRates"));
      res.reset(pstmt->executeQuery());
      while (res->next()) {
        banks.push_back(res->getString("bank"));
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
      return {};
    }
    return banks;
  }

  //retrieves forex rate of a particular ticker
  double GetForexRate(const string& ticker) {
    //initialize as NaN
    double rate = numeric_limits<double>::quiet_NaN();
    try{
      pstmt.reset(con->prepareStatement("select current_rate from forex where ticker = ?"));
      pstmt->setString(1,ticker);

      res.reset(pstmt->executeQuery());
      res->next();

      rate = res->getDouble("current_rate");
    }
    catch(sql::SQLException &e) {
      printError(e);
    }
    return rate;
  }

  //retrieves bank rate of two currencies
  double GetBankRate(const string& start, const string& end, const string& bank) {
    //initialize as NaN
    double rate = numeric_limits<double>::quiet_NaN();
    try{
      pstmt.reset(con->prepareStatement("select rate from bankRates where start = ? and end = ? and bank = ?"));
      pstmt->setString(1,start);
      pstmt->setString(2,end);
      pstmt->setString(3,bank);

      res.reset(pstmt->executeQuery());
      res->next();

      rate = res->getDouble("rate");
    }
    catch(sql::SQLException &e) {
      printError(e);
    }
    return rate;
  }

  //updates the profitable paths
  void UpdateProfitablePath(const string& expath, const int& length, const double& rate) {
    try{
      pstmt.reset(con->prepareStatement("insert into profitableTrends (expath, length, profit, frequency) values (?, ?, ?, 1) on duplicate key update frequency=frequency+1"));
      pstmt->setString(1,expath);
      pstmt->setString(2,to_string(length));
      pstmt->setString(3,to_string(rate));

      res.reset(pstmt->executeQuery());

    }
    catch(sql::SQLException &e) {
      printError(e);
    }
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

  //converts the time in string form to a unix timestamp
  tm timeConversion(string time){
    istringstream ss(time);
    struct tm timestamp = {};
    ss >> get_time(&timestamp, "%m-%d-%Y+%H:%M:%S");
    if (ss.fail()){
      cout << "# ERR: could not convert time" << endl;
    }
    return timestamp;
  }  
};

#endif
