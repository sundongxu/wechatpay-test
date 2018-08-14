#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <pthread.h>

#include "src/demo.grpc.pb.h"
#include <grpc++/grpc++.h>
#include "src/include/redis_handler.h"
#include "src/include/util.h"
#include "src/include/comm_def.h"

using demo::ClientService;
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
  pthread_t listener; // 初始化？
  string deviceId;
  User *user;
  bool isLogin;

public:
  WechatPayClient();
  ~WechatPayClient();
  int Register(const string &username, const string &password);
  int Login(const string &username, const string &password);
  int Logout();
  int Interact(const string &msg);

  // 单例
  static WechatPayClient *GetInstance();
  static void DestroyInstance();

  int StartListen();                  // 启动新线程监听来自服务端的强制下线消息，主函数为ListenKickOff
  static void *ListenKickOff(void *); // 静态成员，作为监听线程的主函数

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
  inline User *GetUser() { return user; }
  inline string GetDeviceId() { return deviceId; }
  inline pthread_t &GetListener() { return listener; }
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

  int KickOff(const string &peerUri, const string &username, const string &deviceId);

private:
  map<string, string> active_users;      // 在线用户列表
  RedisHandler *handler;                 // Redis句柄
  unique_ptr<ClientService::Stub> stub_; // 用于调用客户端下线接口
};

// 客户端服务类，仅提供强制下线接口给服务端
class WechatPayClientServiceImpl final : public ClientService::Service
{
public:
  Status KickOff(ServerContext *context, const Request *request,
                 Response *reply) override;
  inline void BindClient(WechatPayClient *client) { wcp_client = client; }

private:
  WechatPayClient *wcp_client;
};

#endif // ENTITY_H