#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif

#define SUCCESS 0
#define FAILURE -1

using demo::BasicService;
using demo::Request;
using demo::Response;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
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

// Logic and data behind the server's behavior.
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
    /**
	 * [!] Be carefull here using one cert with the CN != localhost. [!]
	 **/
    std::string server_address("localhost:50051");

    std::string key;
    std::string cert;
    std::string root;

    read("../crt/server.crt", cert);
    read("../crt/server.key", key);
    read("../crt/ca.crt", root);

    ServerBuilder builder;

    grpc::SslServerCredentialsOptions::PemKeyCertPair keycert = {key, cert};

    grpc::SslServerCredentialsOptions sslOps;
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
