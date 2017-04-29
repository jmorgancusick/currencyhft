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

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/date_clock_device.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

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
vector<chart_info> * selectHistoricalTickerData(string ticker, string timeframe){
    vector<chart_info> *rows = new vector<chart_info>();

    try{
      //mysql query
      string table;

      //====TIME LOGIC====
      //get todays date
      boost::gregorian::date endDate(boost::gregorian::day_clock::universal_day());
      //start date (init before subtracting)
      boost::gregorian::date startDate(endDate);

      int subYears = 0;
      int subMonths = 0;
      int subDays = 0;


      //set database
      if(timeframe == "1d"){           // 1 min interval
        table = "forexDashMinute";

        // Initialize days object with 1 day   
        //TODO: change back to 1                                
        subDays = 2;


      } else if(timeframe == "5d"){    // 5 min interval
        table = "forexDashMinute";

        // Initialize days object with 5 days 
        //TODO: change back to 5                                  
        subDays = 6;


      } else if(timeframe == "ytd"){   // 1 day interval
        table = "forexDashDaily";

        subMonths = startDate.month()-1;
        subDays = startDate.day()-1;

      } else if(timeframe == "1y"){    // 1 day interval
        table = "forexDashDaily";

        // Initialize days object with 1 Year                                  
        subYears = 1;


      } else if(timeframe == "2y"){    // 1 day interval
        table = "forexDashDaily";

        // Initialize days object with 2 Years                                   
        subYears = 2;


      } else if(timeframe == "5y"){    // 7 day interval
        table = "forexDashDaily";

        // Initialize days object with 5 Years                                   
        subYears = 5;

      } else{
        cout << "# ERR: Improper input for variable interval in selectHistoricalTickerData" << endl;
        return NULL;
      }

      // Calculate start date
      boost::gregorian::years yearsObj(subYears);
      boost::gregorian::months monthsObj(subMonths);
      boost::gregorian::days daysObj(subDays);

      startDate = startDate - yearsObj;
      startDate = startDate - monthsObj;
      startDate = startDate - daysObj;

      std::cout<<"Start Date: " << startDate << std::endl;
      std::cout<<"End Date: " << endDate << std::endl;
      std::cout<<"table: " << table << std::endl;

      // Convert to POSIX
      boost::posix_time::ptime const time_epoch(boost::gregorian::date(1970, 1, 1));
      boost::posix_time::ptime startTimePOSIX(startDate);
      boost::posix_time::ptime endTimePOSIX(endDate);

      auto startTimestamp = (startTimePOSIX-time_epoch).total_seconds();
      auto endTimestamp = (endTimePOSIX-time_epoch).total_seconds();

      std::cout<< "start ts: "<< startTimestamp << std::endl;
      std::cout<< "end ts: "<<endTimestamp << std::endl;

      // Query MySQL
      pstmt.reset(con->prepareStatement("select * from "+table+" where ticker=? and timestamp<? and timestamp>=? order by timestamp"));
      pstmt->setString(1,ticker);
      pstmt->setInt(2,endTimestamp); 
      pstmt->setInt(3,startTimestamp);

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

  //retrieves all forex rates
  unordered_map<string, double> GetForexRates() {
    unordered_map<string, double> rates;
    try{
      pstmt.reset(con->prepareStatement("select ticker, current_rate from forex"));

      res.reset(pstmt->executeQuery());
      while (res->next()) {
        string ticker = res->getString("ticker");
        double rate = res->getDouble("current_rate");
        rates[ticker] = rate;
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
    }
    return rates;
  }

  //retrieves all forex rates (close) at time
  unordered_map<string, double> GetForexDayRates(long timestamp) {
    unordered_map<string, double> rates;
    try{
      pstmt.reset(con->prepareStatement("select ticker, close from forexDashDaily where timestamp=?"));
      pstmt->setString(1,std::to_string(timestamp));

      res.reset(pstmt->executeQuery());
      while (res->next()) {
        string ticker = res->getString("ticker");
        double rate = res->getDouble("close");
        rates[ticker] = rate;
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
    }
    return rates;
  }


  //retrieves all forex rates (close) at time
  unordered_map<string, double> GetForexMinRates(long timestamp) {
    unordered_map<string, double> rates;
    try{
      pstmt.reset(con->prepareStatement("select ticker, close from forexDashMinute where timestamp=?"));
      pstmt->setString(1,std::to_string(timestamp));

      res.reset(pstmt->executeQuery());
      while (res->next()) {
        string ticker = res->getString("ticker");
        double rate = res->getDouble("close");
        rates[ticker] = rate;
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
    }
    return rates;
  }


  //retrieves all forex rates from day db
  map< long, unordered_map<string, double> > GetRecentForexDayRates(unsigned int numDays) {
    map<long, unordered_map<string, double> > dayRates;

    vector<long> timestamps;

    try{
      pstmt.reset(con->prepareStatement("select distinct timestamp from forexDashDaily order by timestamp desc limit ?"));
      pstmt->setString(1, std::to_string(numDays));

      res.reset(pstmt->executeQuery());
      while (res->next()) {
        timestamps.push_back(res->getInt("timestamp"));
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
    }

    //reverse order, put oldest data in index 0 (newest timestamp is in last index)
    for(int i = timestamps.size()-1; i >= 0; i--){
      unordered_map<string, double> rates = GetForexDayRates(timestamps[i]);
      dayRates[timestamps[i]] = rates;
    }



    return dayRates;
  }


  //retrieves all forex rates from minute db
  map< long, unordered_map<string, double> > GetCurrentForexDayRates() {
    map<long, unordered_map<string, double> > dayRates;
    vector<long> timestamps;

    //get todays date
    boost::gregorian::date endDate(boost::gregorian::day_clock::universal_day());
    //start date (init before subtracting)
    boost::gregorian::date startDate(endDate);

    // Calculate start date
    // TODO: CHANGE BACK TO ONE DAY
    boost::gregorian::days daysObj(2);
    startDate = startDate - daysObj;

    std::cout<<"Start Date: " << startDate << std::endl;
    std::cout<<"End Date: " << endDate << std::endl;

    // Convert to POSIX
    boost::posix_time::ptime const time_epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime startTimePOSIX(startDate);
    boost::posix_time::ptime endTimePOSIX(endDate);

    auto startTimestamp = (startTimePOSIX-time_epoch).total_seconds();
    auto endTimestamp = (endTimePOSIX-time_epoch).total_seconds();

    std::cout<< "start ts: "<< startTimestamp << std::endl;
    std::cout<< "end ts: "<<endTimestamp << std::endl;

    try{
      pstmt.reset(con->prepareStatement("select distinct timestamp from forexDashMinute where timestamp<? and timestamp>=? order by timestamp desc"));
      pstmt->setInt(1,endTimestamp); 
      pstmt->setInt(2,startTimestamp);

      res.reset(pstmt->executeQuery());
      while (res->next()) {
        timestamps.push_back(res->getInt("timestamp"));
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
    }

    std::cout<< "timestamps length: "<<timestamps.size()<<std::endl;

    //reverse order, put oldest data in index 0 (newest timestamp is in last index)
    for(int i = timestamps.size()-1; i >= 0; i--){
      unordered_map<string, double> rates = GetForexMinRates(timestamps[i]);
      dayRates[timestamps[i]] = rates;
      if(i%10 == 0){
        std::cout<<"queried "<<i<<" of "<<timestamps.size()<<std::endl;
      }
    }



    return dayRates;
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

  //retrieves all bank rates
  unordered_map<string, double> GetBankRates() {
    unordered_map<string, double> rates;
    try{
      pstmt.reset(con->prepareStatement("select start, end, bank, rate from bankRates"));

      res.reset(pstmt->executeQuery());
      while (res->next()) {
        string start = res->getString("start");
        string end = res->getString("end");
        string bank = res->getString("bank");
        double rate = res->getDouble("rate");
        rates[start+end+bank] = rate;
      }
    }
    catch(sql::SQLException &e) {
      printError(e);
    }
    return rates;
  }

  //updates the profitable paths
  void UpdateProfitablePath(const string& expath, const int& length, const double& rate) {
    try{
      pstmt.reset(con->prepareStatement("insert into profitableTrends (expath, length, profit, frequency) values (?, ?, ?, 1) on duplicate key update profit = ?, frequency = frequency+1"));
      pstmt->setString(1,expath);
      pstmt->setInt(2,length);
      pstmt->setDouble(3,rate);
      pstmt->setDouble(4,rate);

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
