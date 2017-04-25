{
  "targets": [
    {
      "target_name": "addon",
      "include_dirs": ["/usr/include", "/usr/include/cppconn"],
      "libraries": [ "/usr/lib/libmysqlcppconn.a","/usr/lib/x86_64-linux-gnu/libmysqlclient.so.20", "/usr/lib/x86_64-linux-gnu/libboost_date_time.so.1.58.0"],
      "sources": [ "rest_api.cpp" , "cycle.cpp", "path.cpp", "graph.cpp"],
      "cflags_cc": ["-fexceptions", "-frtti", "-std=c++11"]
    }
  ]
}