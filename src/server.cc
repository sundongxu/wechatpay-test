#include "entity.h"

void RunServer()
{
    std::string server_address("localhost:50051");

    std::string key;
    std::string cert;
    std::string root;

    util::read("crt/server.crt", cert);
    util::read("crt/server.key", key);
    util::read("crt/ca.crt", root);

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

    server->Wait(); // 阻塞提供服务，grpc实现多路复用
}

int main(int argc, char **argv)
{
    RunServer();
    return 0;
}
