#include <node.h>
#include "sql/api.h"
#include "graph.h"
#include "path.h"
#include "string"
#include <unordered_set>
#include <typeinfo>

namespace demo{

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Number;
using v8::Array;


using namespace std;

class REST_API{
public: 

  // not a currently used function
  static void Exchange(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    cout<<"Number of args: "<<args.Length()<<endl;
    
    if (args.Length() != 1) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong number of arguments"));
      return;
    }

    if (!args[0]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param1(args[0]->ToString());
    string id = string(*param1);
    
    cout<<"Hello World!"<<endl;

    
    unordered_map<string, double> *rows = db->selectExchangeWithID(id);

    int i = 0;

    Local<Array> result = Array::New(isolate);
    
    for(unordered_map<string, double>::iterator itr = rows->begin(); itr != rows->end(); itr++, i++){
      cout<< itr->first << "\t" << itr->second << endl;

      // Creates a new Object on the V8 heap
      Local<Object> obj = Object::New(isolate);

      //Can call a pack function here to be cleaner once more data
      // Transfers the data from result, to obj (see below)
      obj->Set(String::NewFromUtf8(isolate, "id"),
  	     String::NewFromUtf8(isolate, itr->first.data()));
      obj->Set(String::NewFromUtf8(isolate, "close"),
  	     Number::New(isolate, itr->second));

      result->Set(i, obj); 
    }

    args.GetReturnValue().Set(result);
    //args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    
  }

    
  //not a currently used function
  static void Table(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    cout<<"Number of args: "<<args.Length()<<endl;

    
    if (args.Length() != 1) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong number of arguments"));
      return;
    }

    if (!args[0]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param1(args[0]->ToString());
    string tableName = string(*param1);

    
    unordered_map<string, double> *rows = db->selectAllFromTable(tableName);

    int i = 0;

    Local<Array> result = Array::New(isolate);
    
    for(unordered_map<string, double>::iterator itr = rows->begin(); itr != rows->end(); itr++, i++){
      cout<< itr->first << "\t" << itr->second << endl;

      // Creates a new Object on the V8 heap
      Local<Object> obj = Object::New(isolate);

      //Can call a pack function here to be cleaner once more data
      // Transfers the data from result, to obj (see below)
      obj->Set(String::NewFromUtf8(isolate, "id"),
  	     String::NewFromUtf8(isolate, itr->first.data()));
      obj->Set(String::NewFromUtf8(isolate, "close"),
  	     Number::New(isolate, itr->second));

      result->Set(i, obj); 
    }

    args.GetReturnValue().Set(result);
    //args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));

  }


  //calls db api to get all forex data
  //then formats and sends to frontend
  static void TickerData(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    cout<<"Number of args: "<<args.Length()<<endl;

    
    if (args.Length() != 0) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong number of arguments"));
      return;
    }
    
    unordered_set<std::string> majorTickers ({
      "USDCAD=X", "EURJPY=X",
      "EURUSD=X", "EURCHF=X",
      "USDCHF=X", "EURGBP=X",
      "GBPUSD=X", "AUDCAD=X",
      "NZDUSD=X", "GBPCHF=X",
      "AUDUSD=X", "GBPJPY=X",
      "USDJPY=X", "CHFJPY=X",
      "EURCAD=X", "AUDJPY=X",
      "EURAUD=X", "AUDNZD=X"
    });
    
    unordered_map<std::string, double> *rows = db->selectAllTickerData();   //Get data

    int i = 0;

    Local<Array> result = Array::New(isolate);    //JSON results Array
    
    //Loop through C++ hashmap
    for(unordered_map<std::string, double>::iterator itr = rows->begin(); itr != rows->end(); itr++){
      cout<< itr->first << "\t" << itr->second << endl;

      // Add to return array if it is a major ticker
      if (majorTickers.find(itr->first) != majorTickers.end()){

        // Creates a new JSON Object on the V8 heap
        Local<Object> obj = Object::New(isolate);

        //Can call a pack function here to be cleaner once more data
        // Transfers the data from result, to obj (see below)
        obj->Set(String::NewFromUtf8(isolate, "ticker"),
           String::NewFromUtf8(isolate, itr->first.data()));
        obj->Set(String::NewFromUtf8(isolate, "percentChange"),
           Number::New(isolate, itr->second));

        result->Set(i, obj); 

        i++;
      }
    }

    args.GetReturnValue().Set(result);
    //args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    
  }


  //Used to send request to database api and recieve data for a ticker in a timespan
  //Then formats and sends data back up to the frontend
  static void ChartData(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    cout<<"Number of args: "<<args.Length()<<endl;

    //Make sure 4 arguments.  Cast to string
    if (args.Length() != 4) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong number of arguments"));
      return;
    }

    if (!args[0]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param1(args[0]->ToString());
    std::string ticker = std::string(*param1);
    
    if (!args[1]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param2(args[1]->ToString());
    std::string timeframe = std::string(*param2);
    
    v8::String::Utf8Value param3(args[2]->ToString());
    std::string startDate = std::string(*param3);

    v8::String::Utf8Value param4(args[3]->ToString());
    std::string endDate = std::string(*param4);

    //call db api to get data
    vector<API::chart_info> *rows = db->selectHistoricalTickerData(ticker, timeframe, startDate, endDate);    //Database query

    //format data to be sent to front end
    int i = 0;

    Local<Array> result = Array::New(isolate);

    for(vector<API::chart_info>::iterator itr = rows->begin(); itr != rows->end(); itr++, i++){
      cout << itr->timestamp << "\t" << itr->ticker << "\t" << itr->high << "\t" << itr->volume << "\t"
            << itr->open << "\t" << itr->low << "\t" << itr->close << endl;

      // Creates a new Object on the V8 heap
      Local<Object> obj = Object::New(isolate);

      //Can call a pack function here to be cleaner once more data
      // Transfers the data from result, to obj (see below)
      obj->Set(String::NewFromUtf8(isolate, "date"),
         String::NewFromUtf8(isolate, itr->date.data()));
      obj->Set(String::NewFromUtf8(isolate, "timestamp"),
         Number::New(isolate, itr->timestamp));
      obj->Set(String::NewFromUtf8(isolate, "ticker"),
         String::NewFromUtf8(isolate, itr->ticker.data()));
      obj->Set(String::NewFromUtf8(isolate, "high"),
         Number::New(isolate, itr->high));
      obj->Set(String::NewFromUtf8(isolate, "volume"),
         Number::New(isolate, itr->volume));
      obj->Set(String::NewFromUtf8(isolate, "open"),
         Number::New(isolate, itr->open));
      obj->Set(String::NewFromUtf8(isolate, "low"),
         Number::New(isolate, itr->low));
      obj->Set(String::NewFromUtf8(isolate, "close"),
         Number::New(isolate, itr->close));
      
      result->Set(i, obj); 
    }

    args.GetReturnValue().Set(result);
    //args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
  }


  //takes in 4 arguments: string starting currency, string ending currency,
  //array of strings of currencies to exclude, and int maximum number of exchanges in the path
  //if no currencies should be excluded, give an empty array
  //if there is no limit to the number of exchanges, give a value of 0
  //returns 2 parameters: array for strings of the currencies that form the optimal path, and
  //double total rate over the path
  static void ArbitrageData(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    cout<<"Number of args: "<<args.Length()<<endl;

    //Make sure 4 arguments.  Cast to string
    if (args.Length() != 4) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong number of arguments"));
      return;
    }

    if (!args[0]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param1(args[0]->ToString());
    string startCurr = string(*param1);
    
    if (!args[1]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param2(args[1]->ToString());
    string endCurr = string(*param2);

    if (!args[2]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param3(args[2]->ToString());
    std::string param3Str = std::string(*param3);

    int maxNumberExchanges = std::stoi(param3Str.data());


    if (!args[3]->IsArray()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    Local<Array> param4 = Local<Array>::Cast(args[3]);

    std::cout << "Array cast" << std::endl;

    vector<std::string> currenciesToExclude;

    std::cout<< "Param 4 Length: " << param4->Length() << std::endl;
    for(int i=0; i < param4->Length(); i++) {
      Local<Value> ele = param4->Get(i);

      if(!ele->IsString()){
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments in array"));
        return;
      }

      v8::String::Utf8Value arrEle(ele->ToString());
      std::string tmpStr = std::string(*arrEle);

      currenciesToExclude.push_back(tmpStr);
    }


    unordered_set<string> excludeCurrs(currenciesToExclude.begin(), currenciesToExclude.end());

    Path path = Path(*g, startCurr, endCurr, excludeCurrs, maxNumberExchanges);
    vector<string> *p = path.GetPath();
    double totalRate = path.GetTotalRate();

    

    /*for (unsigned int i = 0; i < path.size(); i++) {
      Local<Object> obj = Object::New(isolate);
      obj->Set(String::NewFromUtf8(isolate, "currency"), String::NewFromUtf8(isolate, p[i].data()));
      result->Set(i, obj);
    }*/


    Local<Object> result = Object::New(isolate);
    result->Set(String::NewFromUtf8(isolate, "totalRate"), Number::New(isolate, totalRate));

    Local<Array> currencies = Array::New(isolate);

    int i = 0;
    for (vector<string>::iterator itr = p->begin(); itr != p->end(); itr++, i++) {

      currencies->Set(i, String::NewFromUtf8(isolate, itr->data()));
    }

    result->Set(String::NewFromUtf8(isolate, "currencies"), currencies);

    args.GetReturnValue().Set(result);

  }


  static void CalculatorData(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    cout<<"Number of args: "<<args.Length()<<endl;

    //Make sure 2 arguments.
    if (args.Length() != 2) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong number of arguments"));
      return;
    }

    if (!args[0]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param1(args[0]->ToString());
    string startCurr = string(*param1);
    
    if (!args[1]->IsString()) {
      isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
      return;
    }

    v8::String::Utf8Value param2(args[1]->ToString());
    string endCurr = string(*param2);
    

    double rate = db->GetForexRate(startCurr + endCurr + "=X");
    Local<Object> result = Object::New(isolate);
    result->Set(String::NewFromUtf8(isolate, "rate"), Number::New(isolate, rate));

    args.GetReturnValue().Set(result);

  }

  static void shutdown(const FunctionCallbackInfo<Value>& args){
    std::cout << "Shutting down REST_API and DB API" << std::endl;

    // Clear memory
    delete db;
    delete g;
  }

  // statis member variables
  static API *db;
  static Graph *g;

  static void init(Local<Object> exports) {
    // Establish Node.js addon functions
    NODE_SET_METHOD(exports, "exchange", REST_API::Exchange);
    NODE_SET_METHOD(exports, "table", REST_API::Table);
    NODE_SET_METHOD(exports, "tickerData", REST_API::TickerData);
    NODE_SET_METHOD(exports, "chartData", REST_API::ChartData);
    NODE_SET_METHOD(exports, "arbitrageData", REST_API::ArbitrageData);
    NODE_SET_METHOD(exports, "calculatorData", REST_API::CalculatorData);
    NODE_SET_METHOD(exports, "shutdown", REST_API::shutdown);

    // Connect to Database
    REST_API::db = new API();
    int retVal = REST_API::db->connect();
    if(retVal != 0){
      cout<<"db.connect() failed with exit code "<<retVal<<endl;
      return;
    }

    // Create graph
    vector<string> currencies = db->GetAllCurrencies();

    REST_API::g = new Graph(currencies);

    std::cout << "Made graph" << std::endl;

    for (auto it = currencies.begin(); it != currencies.end(); ++it) {
      //iterate through "to nodes"
      for (auto it2 = currencies.begin(); it2 != currencies.end(); ++it2) {
        //don't store reflex edges
        if (*it != *it2) {
          //all edges are initialized to infinity
          cout << *it << *it2 << endl;
          double rate = -log(db->GetForexRate(*it+*it2+"=X"));
          g->SetEdgeWeight(*it, *it2, rate);
        }
      }
    }


  }





};





// Define the static member variables
API *REST_API::db = NULL;
Graph *REST_API::g = NULL;

// Expose Node Module
NODE_MODULE(addon, REST_API::init)

} // end namespace demo
