#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <string>

#include "src/demo.grpc.pb.h"
#include <grpc++/grpc++.h>

using demo::BasicService;
using demo::Request;
using demo::Response;
using grpc::Channel;
using grpc::ClientContext;
using grpc::CreateChannel;
using grpc::SslCredentials;
using grpc::Status;

class WechatPayClient
{
  public:
    WechatPayClient(const std::string &cert,
                    const std::string &key,
                    const std::string &root,
                    const std::string &server)
    {
        grpc::SslCredentialsOptions opts = {root, key, cert};
        stub_ = BasicService::NewStub(CreateChannel(
            server, SslCredentials(opts)));
    }

    std::string Login(const std::string &user, const std::string &password)
    {
        Request request;
        request.set_username(user);
        request.set_password(password);
        Response reply;
        ClientContext context;
        Status status = stub_->Login(&context, request, &reply);
        if (status.ok())
        {
            return reply.message();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

  private:
    std::unique_ptr<BasicService::Stub> stub_;
};

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

int main(int argc, char **argv)
{
    std::string cert;
    std::string key;
    std::string root;
    std::string server{"localhost:50051"};

    read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/client.crt", cert);
    read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/client.key", key);
    read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/ca.crt", root);

    WechatPayClient client(cert, key, root, server);

    std::string user("roysun");
    std::string password("123456");
    std::string reply = client.Login(user, password);

    std::cout << "Client received: " << reply << std::endl;

    return 0;
}
