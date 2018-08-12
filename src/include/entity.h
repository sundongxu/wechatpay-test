#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "src/demo.grpc.pb.h"
#include <grpc++/grpc++.h>
#include "src/include/redis_handler.h"
#include "src/include/util.h"

using demo::BasicService;
using demo::Request;
using demo::Response;
using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::SslCredentials;
using grpc::Status;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::SslServerCredentialsOptions;

using namespace std;

class User;

// 客户端
class WechatPayClient
{
public:
  WechatPayClient(const string &cert,
                  const string &key,
                  const string &root,
                  const string &server);
  ~WechatPayClient();
  int Register(const string &username, const string &password);
  int Login(const string &username, const string &password);
  int Logout();
  int Interact(const string &msg);

private:
  unique_ptr<BasicService::Stub> stub_;
  string deviceId;
  User *user;
  bool isLogin;
};

// 服务端
class WechatPayServiceImpl final : public BasicService::Service
{
public:
  Status Register(ServerContext *context, const Request *request,
                  Response *reply) override;

  Status Login(ServerContext *context, const Request *request,
               Response *reply) override;

  Status Logout(ServerContext *context, const Request *request,
                Response *reply) override;

  Status Interact(ServerContext *context, const Request *request,
                  Response *reply) override;

private:
  map<string, string> active_users; // 在线用户列表
  RedisHandler *handler;            // Redis句柄
};

// 用户
class User
{
public:
  User() {}
  User(const string &strName, const string &strPassword)
  {
    name = strName;
    password = strPassword;
  }
  ~User() {}
  inline string &getName() { return name; }
  inline void setName(const string &strName) { name = strName; }
  inline string &getPassWord() { return password; }
  inline void setPassWord(const string &strPassword) { password = strPassword; }

private:
  string name;
  string password;
};

#endif // ENTITY_H