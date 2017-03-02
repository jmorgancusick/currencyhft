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

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  cout<<"Hello World!"<<endl;

  API *db = new API();
  int retVal = db->connect();
  unordered_map<std::string, double> *rows = db->selectAllFromTable("stock");

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
  NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(addon, init)

}  // namespace demo
