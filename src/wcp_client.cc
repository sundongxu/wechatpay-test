#include "src/include/entity.h"
#include "src/include/comm_def.h"

WechatPayClient::WechatPayClient()
{
    string cert;
    string key;
    string root;
    string server{"localhost:50051"};

    util::read(PATH_CLIENT_CERT, cert);
    util::read(PATH_CLIENT_KEY, key);
    util::read(PATH_CA_CERT, root);

    grpc::SslCredentialsOptions opts = {root, key, cert};
    stub_ = ServerService::NewStub(CreateChannel(
        server, SslCredentials(opts)));
    util::generateDeviceId(deviceId); // 一台终端唯一标识
    user = nullptr;
    isLogin = false;
    listener = 0;
}

WechatPayClient *WechatPayClient::client = nullptr;

WechatPayClient *WechatPayClient::GetInstance()
{
    if (client == nullptr)
    {
        client = new WechatPayClient();
    }
    return client;
}

void WechatPayClient::DestroyInstance()
{
    if (client != nullptr)
    {
        delete client;
        client = nullptr;
    }
}

WechatPayClient::~WechatPayClient()
{
    if (user != nullptr)
    {
        delete user;
        user = nullptr;
    }
    DestroyInstance();
}

int WechatPayClient::Login(const string &username, const string &password)
{
    // 客户端登录逻辑
    // 判断当前是否登录
    if (isLogin && user != nullptr)
    {
        // 已登录，请先注销
        cout << "您已登录，请先注销！" << endl;
        cout << "当前用户：" << user->getName() << endl;
        return -1;
    }
    else
    {
        // 未登录，发起登录请求
        // 构造请求
        Request request;
        request.set_username(username);
        request.set_password(password);
        request.set_device_id(deviceId);

        Response reply;
        ClientContext context;
        Status status = stub_->Login(&context, request, &reply); // 被踢下线重新登录会崩！
        if (status.ok())
        {
            // RPC调用成功，根据返回状态码确定登录结果
            switch (reply.status_code())
            {
            case USER_REQUEST_SUCCESS:
                cout << "登录成功！" << endl;
                isLogin = true;
                user = new User(username, password);
                break;
            case USER_KICK_OTHERS_OFF_SUCCESS:
                cout << "登录成功：踢原终端下线！" << endl;
                isLogin = true;
                user = new User(username, password);
                break;
            case USER_NOT_FOUND:
                cout << "登录失败：用户不存在！" << endl;
                return -1;
            case USER_PASSWORD_WRONG:
                cout << "登录失败：密码错误！" << endl;
                return -1;
            case DB_NOT_CONNECTED:
                cout << "登录失败：Redis服务连接失败！" << endl;
                return -1;
            }
        }
        else
        {
            // RPC调用失败
            cout << "登录失败：RPC调用失败！" << endl;
            cout << status.error_code() << ": " << status.error_message()
                 << endl;
            return -1;
        }
    }
    return 0;
}

int WechatPayClient::Register(const string &username, const string &password)
{
    // 无论是否登录，都可注册
    // 构造请求
    Request request;
    request.set_username(username);
    request.set_password(password);

    // 客户端注册逻辑
    Response reply;
    ClientContext context;
    Status status = stub_->Register(&context, request, &reply);
    if (status.ok())
    {
        // RPC调用成功。根据返回状态码确定注册结果
        switch (reply.status_code())
        {
        case USER_REQUEST_SUCCESS:
            cout << "注册成功！" << endl;
            break;
        case USER_ALREADY_EXIST:
            cout << "注册失败：用户名已存在！" << endl;
            return -1;
        case DB_NOT_CONNECTED:
            cout << "注册失败：Redis服务连接失败！" << endl;
            return -1;
        }
    }
    else
    {
        // RPC调用失败
        cout << "注册失败：RPC调用失败！" << endl;
        cout << status.error_code() << ": " << status.error_message()
             << endl;
        return -1;
    }
    return 0;
}

int WechatPayClient::Logout()
{
    // 客户端下线逻辑
    if (isLogin && user != nullptr)
    {
        // 已登录
        // 构造请求
        Request request;
        request.set_username(user->getName());
        request.set_device_id(deviceId);
        Response reply;
        ClientContext context;
        Status status = stub_->Logout(&context, request, &reply);
        if (status.ok())
        {
            // RPC调用成功，分两种情况(通过reply的状态码区分)：
            // (1) 正常下线
            // (2) 本用户在别处登录，本客户端早已被踢下线
            switch (reply.status_code())
            {
            case USER_REQUEST_SUCCESS:
                cout << "下线成功！" << endl;
                ResetLoginState();
                break;
            case USER_LOGIN_ANOTHER_PLACE:
                cout << "下线失败：用户已在别处登录，本客户端已强制下线！" << endl;
                ResetLoginState();
                return -1;
            }
        }
        else
        {
            // RPC调用失败
            cout << "下线失败：RPC调用失败！" << endl;
            cout << status.error_code() << ": " << status.error_message()
                 << endl;
            return -1;
        }
    }
    else
    {
        // 未登录
        cout << "您尚未登录！" << endl;
        return -1;
    }
    return 0;
}

int WechatPayClient::Interact(const string &msg)
{
    // 客户端交互逻辑(登录态)
    if (isLogin && user != nullptr)
    {
        // 已登录
        // 构造请求
        Request request;
        request.set_username(user->getName());
        request.set_device_id(deviceId);
        request.set_echo_content(msg);
        Response reply;
        ClientContext context;
        Status status = stub_->Interact(&context, request, &reply);
        if (status.ok())
        {
            // RPC调用成功，分两种情况(通过reply的状态码区分)：
            // (1) 正常，回显内容
            // (2) 本用户在别处登录，本客户端早已被踢下线
            switch (reply.status_code())
            {
            case USER_REQUEST_SUCCESS:
                cout << "交互成功！" << endl;
                cout << "服务端回复内容：" << reply.echo_content() << endl;
                break;
            case USER_LOGIN_ANOTHER_PLACE:
                cout << "交互失败：用户已在别处登录，请登录后重试！" << endl;
                ResetLoginState();
                return -1;
            }
        }
        else
        {
            // RPC调用失败
            cout << "交互失败：RPC调用失败！" << endl;
            cout << status.error_code() << ": " << status.error_message()
                 << endl;
            return -1;
        }
    }
    else
    {
        // 尚未登录
        cout << "请先登录！" << endl;
        return -1;
    }
    return 0;
}

int WechatPayClient::StartListen()
{
    //
    int rc = pthread_create(&listener, nullptr, &WechatPayClient::ListenKickOff, nullptr);
    if (rc == 0)
    {
        // cout << "监听线程创建成功！" << endl;
    }
    else
    {
        cout << "监听线程创建失败" << endl;
        return -1;
    }
    return 0;
}

void *WechatPayClient::ListenKickOff(void *arg)
{
    // 获取单例
    WechatPayClient *c = WechatPayClient::GetInstance();
    // 创建监听server，在50052端口上监听来自服务端的强制下线请求
    string listen_address("0.0.0.0:50052");

    string key;
    string cert;
    string root;

    util::read(PATH_CLIENT_CERT, cert);
    util::read({PATH_CLIENT_KEY}, key);
    util::read(PATH_CA_CERT, root);

    ServerBuilder builder;

    SslServerCredentialsOptions::PemKeyCertPair keycert = {key, cert};
    SslServerCredentialsOptions sslOps;
    sslOps.pem_root_certs = root;
    sslOps.pem_key_cert_pairs.push_back(keycert);

    builder.AddListeningPort(listen_address, grpc::SslServerCredentials(sslOps));
    WechatPayClientServiceImpl service;
    builder.RegisterService(&service);

    // 绑定客户端与客户端服务对象
    service.BindClient(c);

    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Client listening on " << listen_address << " for Kick-Off message from server！" << endl;

    server->Wait(); // 阻塞提供服务，grpc实现多路复用
    return nullptr;
}

Status WechatPayClientServiceImpl::KickOff(ServerContext *context,
                                           const Request *request,
                                           Response *reply)
{
    // 客户端提供给服务端的强制下线接口
    if (wcp_client->IsLogin() && wcp_client->GetUser() != nullptr)
    {
        // 已登录，则当前客户端下线
        string username = request->username();
        string deviceId = request->device_id();
        // cout << "当前客户端设备ID：" << wcp_client->GetDeviceId() << endl;
        // cout << "服务端想踢下线的客户端设备ID：" << deviceId << endl;
        if (username == wcp_client->GetUser()->getName() && deviceId == wcp_client->GetDeviceId())
        {
            // 验证用户名及客户端设备ID
            // 还原客户端登录态
            wcp_client->ResetLoginState();
            // 取消监听消息线程
            pthread_cancel(wcp_client->GetListener());
            void *ret = nullptr;
            // 回收线程资源
            pthread_join(wcp_client->GetListener(), &ret);
            reply->set_status_code(USER_KICKED_OFF_SUCCESS);
            cout << "用户已在别处登录，本客户端强制下线！" << endl
                 << "请继续输入：" << endl;
        }
        else
        {
            // 正常不会走到这
            reply->set_status_code(USER_KICKED_OFF_FAILURE);
        }
    }
    else
    {
        // 正常不会走到这
        reply->set_status_code(USER_KICKED_OFF_FAILURE);
    }
    return Status::OK;
}
