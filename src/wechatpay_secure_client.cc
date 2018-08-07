#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "demo.grpc.pb.h"
#endif

using demo::BasicService;
using demo::Request;
using demo::Response;
using grpc::Channel;
using grpc::ClientContext;
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
        stub_ = BasicService::NewStub(grpc::CreateChannel(
            server, grpc::SslCredentials(opts)));
    }

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string Login(const std::string &user, const std::string &password)
    {
        // Data we are sending to the server.
        Request request;
        request.set_username(user);
        request.set_password(password);

        // Container for the data we expect from the server.
        Response reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->Login(&context, request, &reply);

        // Act upon its status.
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

    read("../crt/client.crt", cert);
    read("../crt/client.key", key);
    read("../crt/ca.crt", root);

    WechatPayClient client(cert, key, root, server);

    std::string user("roysun");
    std::string password("123456");
    std::string reply = client.Login(user, password);

    std::cout << "Client received: " << reply << std::endl;

    return 0;
}
