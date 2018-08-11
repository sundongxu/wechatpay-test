#ifndef REDIS_HANDLER_H
#define REDIS_HANDLER_H

#include "hiredis.h"
#include <string>
#include <sstream>

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

  // 私有构造，单例模式
  RedisHandler(){};

  bool IsError();
  void SetString(const string &data);

public:
  static RedisHandler *GetInstance();
  static void DestoryInstance();
  int Connect();
  void DisConnect();
  void FreeReply();

  void SetString(const string &key, const string &value);
  string &GetString(const string &key);
};

#endif // REDIS_HANDLER_H