#include "src/include/entity.h"
#include "src/include/comm_def.h"

// 服务端在50051端口上监听来自客户端的服务请求
void RunServer()
{
    string server_address("0.0.0.0:50051");

    string key;
    string cert;
    string root;

    util::read(PATH_SERVER_CERT, cert);
    util::read(PATH_SERVER_KEY, key);
    util::read(PATH_CA_CERT, root);

    ServerBuilder builder;

    SslServerCredentialsOptions::PemKeyCertPair keycert = {key, cert};
    SslServerCredentialsOptions sslOps;
    sslOps.pem_root_certs = root;
    sslOps.pem_key_cert_pairs.push_back(keycert);

    builder.AddListeningPort(server_address, grpc::SslServerCredentials(sslOps));
    WechatPayServerServiceImpl service;
    builder.RegisterService(&service);

    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on " << server_address << " for requests from clients！" << endl;

    server->Wait(); // 阻塞提供服务，grpc实现多路复用
}

int main(int argc, char **argv)
{
    RunServer();
    return 0;
}
