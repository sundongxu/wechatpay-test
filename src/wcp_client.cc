#include "src/include/entity.h"
#include "src/include/comm_def.h"

WechatPayClient::WechatPayClient(const string &cert,
                                 const string &key,
                                 const string &root,
                                 const string &server)
{
    grpc::SslCredentialsOptions opts = {root, key, cert};
    stub_ = BasicService::NewStub(CreateChannel(
        server, SslCredentials(opts)));
    util::generateDeviceId(deviceId); // 一台终端唯一标识
    user = nullptr;
    isLogin = false;
}

WechatPayClient::~WechatPayClient()
{
    if (user != nullptr)
    {
        delete user;
        user = nullptr;
    }
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
        Status status = stub_->Login(&context, request, &reply);
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
            case USER_KICK_OTHERS_OFF:
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
                isLogin = false;
                delete user;
                user = nullptr;
                break;
            case USER_LOGIN_ANOTHER_PLACE:
                cout << "下线失败：用户已在别处登录，本客户端已强制下线！" << endl;
                isLogin = false;
                delete user;
                user = nullptr;
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
                isLogin = false;
                delete user;
                user = nullptr;
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