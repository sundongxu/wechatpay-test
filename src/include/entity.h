#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <condition_variable>

#include "src/demo.grpc.pb.h"
#include <grpc++/grpc++.h>
#include "src/include/redis_handler.h"
#include "src/include/util.h"
#include "src/include/comm_def.h"

using demo::Request;
using demo::Response;
using demo::ServerService;
using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::SslCredentials;
using grpc::Status;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::SslServerCredentialsOptions;

// server-side streaming rpc
using grpc::ClientReader;
using grpc::ServerWriter;

using namespace std;

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

// 客户端
class WechatPayClient
{
private:
  static WechatPayClient *client; // 单例
  unique_ptr<ServerService::Stub> stub_;
  string deviceId;
  User *user;
  bool isLogin;
  bool isListening; // 监听线程是否已创建

public:
  WechatPayClient();
  ~WechatPayClient();
  int Register(const string &username, const string &password);
  int Login(const string &username, const string &password);
  int Logout();
  int Interact(const string &msg);
  int KeepAlive(const string &username);

  // 单例
  static WechatPayClient *GetInstance();
  static void DestroyInstance();

  inline void ResetLoginState()
  {
    // 还原登录态
    isLogin = false;
    if (user != nullptr)
    {
      delete user;
      user = nullptr;
    }
  }
  inline bool IsLogin() { return isLogin; }
  inline bool IsListening() { return isListening; }
  inline void SetListeningState(bool state) { isListening = state; }
  inline User *GetUser()
  {
    return user;
  }
  inline string GetDeviceId() { return deviceId; }
};

// 服务端服务类
class WechatPayServerServiceImpl final : public ServerService::Service
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

  Status KeepAlive(ServerContext *context, const Request *request,
                   ServerWriter<Response> *writer) override;

private:
  map<string, string> active_users;            // 在线用户列表(用户名->设备ID)
  mutex cv_m;                                  // 互斥锁，用于条件变量
  bool shouldKickOff;                          // 区分Login与Logout中唤醒观察者的逻辑
  map<string, condition_variable *> observers; // 观察者列表(用户名 -> 观察者(条件变量))，每个用户名最多对应一个观察者，KeepAlive中注册新观察者，Login中注销已有观察者
  RedisHandler *handler;                       // Redis句柄
};

#endif // ENTITY_H