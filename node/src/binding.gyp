{
  "targets": [
    {
      "target_name": "addon",
      "include_dirs": ["/usr/include", "/usr/include/cppconn"],
      "libraries": [ "/usr/lib/libmysqlcppconn.a","/usr/lib/x86_64-linux-gnu/libmysqlclient.so.20"],
      "sources": [ "query.cpp" ],
      "cflags_cc": ["-fexceptions", "-frtti", "-std=c++11"]
    }
  ]
}