#ifndef REDIS_HANDLER_H
#define REDIS_HANDLER_H

#include "hiredis.h"
#include <string>
#include <sstream>
#include <iostream>

#define SETSTRING(key, value)          \
  stringstream ss;                     \
  ss << "SET " << key << " " << value; \
  string s;                            \
  getline(ss, s);                      \
  SetString(s);

using namespace std;

class RedisHandler
{
private:
  static RedisHandler *handler; // 单例指针
  redisContext *context;
  redisReply *reply;
  const char *svrIP;
  int svrPort;

  // 私有构造，单例模式
  RedisHandler(const char *ip, const int port)
  {
    cout << "RedisHandler Constructed!" << endl;
    svrIP = ip;
    svrPort = port;
  };

  bool IsError();
  void SetString(const string &data);

public:
  static RedisHandler *GetInstance(const char *ip, const int port);
  static void DestoryInstance();
  int Connect();
  void DisConnect();
  void FreeReply();

  void SetString(const string &key, const string &value);
  void GetString(const string &key, string &value);
};

#endif // REDIS_HANDLER_H