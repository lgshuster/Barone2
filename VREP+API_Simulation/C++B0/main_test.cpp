// programming/remoteApiBindings/b0Based/cpp/b0RemoteApi.h
// programming/remoteApiBindings/b0Based/cpp/b0RemoteApi.cpp

// include the path to programming/remoteApiBindings/b0Based/cpp/msgpack-c/include
// include the path to programming/blueZero/include/b0/bindings
// link the blueZero library (e.g. b0.dll), and do not forget that the blueZero library itself 
// has dependencies (e.g. libzmq, boost_chrono, boost_system, boost_thread, etc.).

// g++ -Iprogramming/b0RemoteApiBindings/cpp/msgpack-c/include -Iprogramming/bluezero/include/b0/bindings -Iprogramming/bluezero/include/ -Iprogramming/include main_test.cpp


// #include <iostream>
#include "./programming/b0RemoteApiBindings/cpp/msgpack-c/include/msgpack.hpp"
#include "./programming/bluezero/include/b0/bindings/c.h"

#include "./programming/b0RemoteApiBindings/cpp/b0RemoteApi.h"
// #include "./programming/b0RemoteApiBindings/cpp/b0RemoteApi.cpp"
#include "./programming/remoteApi/extApi.h"

//#include "extApi.h"a667p[8]
int main()
{
  // starts a connection
  b0RemoteApi:b0RemoteApi client("b0RemoteApi_c++Client", "b0RemoteApi", 60, false, 3);


  return 0;
}