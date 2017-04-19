// hello.cc

#include <node.h>
#include "sql/api.h"
#include "graph.h"
#include "path.h"
#include "string"
#include <unordered_set>

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Number;
using v8::Array;

using namespace std;

void Exchange(const FunctionCallbackInfo<Value>& args) {
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

  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
    return;
  }

  
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
  
  delete db;
}

  
void Table(const FunctionCallbackInfo<Value>& args) {
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
  
  
  //TODO: move this to initis
  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
    return;
  }
  
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
  
  delete db;
}

void TickerData(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  cout<<"Number of args: "<<args.Length()<<endl;

  
  if (args.Length() != 0) {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong number of arguments"));
    return;
  }
  
  
  //TODO: move this to init
  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
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
  
  delete db;
}


void ChartData(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  cout<<"Number of args: "<<args.Length()<<endl;

  //Make sure 2 arguments.  Cast to string
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

  //TODO: move this to init
  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
    return;
  }

  vector<API::chart_info> *rows = db->selectHistoricalTickerData(ticker, timeframe, startDate, endDate);    //Database query


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
       String::NewFromUtf8(isolate, itr->timestamp.data()));
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
  
  delete db;
}


//takes in 4 arguments: string starting currency, string ending currency,
//array of strings of currencies to exclude, and int maximum number of exchanges in the path
//if no currencies should be excluded, give an empty array
//if there is no limit to the number of exchanges, give a value of 0
//returns 2 parameters: array for strings of the currencies that form the optimal path, and
//double total rate over the path
void ArbitrageData(const FunctionCallbackInfo<Value>& args) {
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

  if (!args[2]->IsArray()) {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
    return;
  }

  v8::Array param3 = *(Array::Cast(*args[2]));

  vector<std::string> currenciesToExclude;
  for(int i=0; i < param3.Length(); i++) {
    v8::String::Utf8Value temp(param3.CloneElementAt(i)->ToString());
    string temp2 = string(*temp);
    currenciesToExclude.push_back(temp2);
  }

  if (!args[3]->IsNumber()) {
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong arguments"));
    return;
  }

  double maxNumberExchanges = args[3]->NumberValue();
  
  //TODO: move this to init
  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
    return;
  }

  vector<string> currencies = db->GetAllCurrencies();

  Graph g = Graph(currencies);

  for (auto it = currencies.begin(); it != currencies.end(); ++it) {
    //iterate through "to nodes"
    for (auto it2 = currencies.begin(); it2 != currencies.end(); ++it2) {
      //don't store reflex edges
      if (*it != *it2) {
        //all edges are initialized to infinity
        cout << *it << *it2 << endl;
        double rate = -log(db->GetForexRate(*it+*it2+"=X"));
        g.SetEdgeWeight(*it, *it2, rate);
      }
    }
  }

  unordered_set<string> excludeCurrs(currenciesToExclude.begin(), currenciesToExclude.end());

  Path path = Path(g, startCurr, endCurr, excludeCurrs, maxNumberExchanges);
  vector<string> *p = path.GetPath();
  double totalRate = path.GetTotalRate();

  Local<Array> result = Array::New(isolate);

  /*for (unsigned int i = 0; i < path.size(); i++) {
    Local<Object> obj = Object::New(isolate);
    obj->Set(String::NewFromUtf8(isolate, "currency"), String::NewFromUtf8(isolate, p[i].data()));
    result->Set(i, obj);
  }*/

  int i = 0;
  for (vector<string>::iterator it = p->begin(); it != p->end(); ++it) {
    Local<Object> obj = Object::New(isolate);
    obj->Set(String::NewFromUtf8(isolate, "currency"), String::NewFromUtf8(isolate, it->data()));
    result->Set(i, obj);
    ++i;
  }

  i = 0;
  Local<Object> obj = Object::New(isolate);
  obj->Set(String::NewFromUtf8(isolate, "totalRate"), Number::New(isolate, totalRate));
  result->Set(i, obj);
  args.GetReturnValue().Set(result);

  delete db;
}


void CalculatorData(const FunctionCallbackInfo<Value>& args) {
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
  
  //TODO: move this to init
  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
    return;
  }

  //ector<string> currencies = db->GetAllCurrencies();

  int i=0;
  Local<Array> result = Array::New(isolate);

  double rate = db->GetForexRate(startCurr + endCurr + "=X");
  Local<Object> obj = Object::New(isolate);
  obj->Set(String::NewFromUtf8(isolate, "rate"), Number::New(isolate, rate));
  result->Set(i, obj);

/*
  for (auto it = currencies.begin(); it != currencies.end(); ++it) {
    //iterate through "to nodes"
    for (auto it2 = currencies.begin(); it2 != currencies.end(); ++it2) {
      //don't store reflex edges
      if (*it != *it2) {
        //all edges are initialized to infinity
        cout << *it << *it2 << endl;
        double rate = db->GetForexRate(*it+*it2+"=X");

        Local<Object> obj = Object::New(isolate);
        obj->Set(String::NewFromUtf8(isolate, "rate"), Number::New(isolate, rate));
        result->Set(i, obj);
        i++;
      }
    }
  }
*/
  args.GetReturnValue().Set(result);

  delete db;
}


void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "exchange", Exchange);
  NODE_SET_METHOD(exports, "table", Table);
  NODE_SET_METHOD(exports, "tickerData", TickerData);
  NODE_SET_METHOD(exports, "chartData", ChartData);
  NODE_SET_METHOD(exports, "arbitrageData", ArbitrageData);
  NODE_SET_METHOD(exports, "calculatorData", CalculatorData);
}

NODE_MODULE(addon, init)

}  // namespace demo
