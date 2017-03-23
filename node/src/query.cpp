// hello.cc

#include <node.h>
#include "sql/api.h"
#include "string"

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
  std::string id = std::string(*param1);
  
  cout<<"Hello World!"<<endl;

  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
    return;
  }

  long date = 20170323033730;
  unordered_map<std::string, double> *rows = db->selectExchangeWithID(id,date,1);
  
  //unordered_map<std::string, double> *rows = db->selectExchangeWithID(id);

  int i = 0;

  Local<Array> result = Array::New(isolate);
  
  for(unordered_map<std::string, double>::iterator itr = rows->begin(); itr != rows->end(); itr++, i++){
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
  std::string tableName = std::string(*param1);
  
  
  //TODO: move this to init
  API *db = new API();
  int retVal = db->connect();
  if(retVal != 0){
    cout<<"db.connect() failed with exit code "<<retVal<<endl;
    isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Failed to connect to database"));
    return;
  }
  
  unordered_map<std::string, double> *rows = db->selectAllFromTable(tableName);

  int i = 0;

  Local<Array> result = Array::New(isolate);
  
  for(unordered_map<std::string, double>::iterator itr = rows->begin(); itr != rows->end(); itr++, i++){
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

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "exchange", Exchange);
  NODE_SET_METHOD(exports, "table", Table);
}

NODE_MODULE(addon, init)

}  // namespace demo
