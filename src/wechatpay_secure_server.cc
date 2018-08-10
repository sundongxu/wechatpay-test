#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <string>

#include "src/demo.grpc.pb.h"
#include <grpc++/grpc++.h>

#define SUCCESS 0
#define FAILURE -1

using demo::BasicService;
using demo::Request;
using demo::Response;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::SslServerCredentialsOptions;
using grpc::Status;

void read(const std::string &filename, std::string &data)
{
    std::ifstream file(filename.c_str(), std::ios::in);

    if (file.is_open())
    {
        std::stringstream ss;
        ss << file.rdbuf();

        file.close();

        data = ss.str();
    }

    return;
}

class WechatPayServiceImpl final : public BasicService::Service
{
    Status Register(ServerContext *context, const Request *request,
                    Response *reply) override
    {
        // 注册逻辑

        return Status::OK;
    }

    Status Login(ServerContext *context, const Request *request,
                 Response *reply) override
    {
        // 登录逻辑
        int code = SUCCESS;
        std::string suffix(" 登录成功!");
        reply->set_status_code(code);
        reply->set_message(request->username() + suffix);

        return Status::OK;
    }

    Status Logout(ServerContext *context, const Request *request,
                  Response *reply) override
    {
        // 登出逻辑

        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("localhost:50051");

    std::string key;
    std::string cert;
    std::string root;

    // you need to specify the crt/key file path
    // for example,
    read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/server.crt", cert);
    read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/server.key", key);
    read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/ca.crt", root);

    ServerBuilder builder;

    SslServerCredentialsOptions::PemKeyCertPair keycert = {key, cert};
    SslServerCredentialsOptions sslOps;
    sslOps.pem_root_certs = root;
    sslOps.pem_key_cert_pairs.push_back(keycert);

    builder.AddListeningPort(server_address, grpc::SslServerCredentials(sslOps));
    WechatPayServiceImpl service;
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer();

    return 0;
}
