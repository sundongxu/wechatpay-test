#include "src/include/entity.h"
#include "src/include/comm_def.h"
#include "include/bcrypt/BCrypt.hpp"

Status WechatPayServerServiceImpl::Register(ServerContext *context,
                                            const Request *request,
                                            Response *reply)
{
    // 注册逻辑
    string username = request->username();
    string password = request->password();

    // grpc::string peerUri = context->peer();
    // cout << "ServerContext URI:" << peerUri << endl;

    cout << "注册服务，用户名：" << username << endl;

    // 查询是否已存在相同用户名
    handler = RedisHandler::GetInstance(REDIS_SERVER_IP, REDIS_SERVER_PORT);
    int ret = handler->Connect();
    if (ret != 0)
    {
        // redis连接失败
        reply->set_status_code(DB_NOT_CONNECTED);
    }
    else
    {
        string strVal;
        handler->GetString(username, strVal);
        if (strVal == "")
        {
            // 用户名不存在，可以注册
            string hash = BCrypt::generateHash(password);
            handler->SetString(username, hash);
            reply->set_status_code(USER_REQUEST_SUCCESS);
        }
        else
        {
            // 用户名已存在，不可注册
            reply->set_status_code(USER_ALREADY_EXIST);
        }
    }
    return Status::OK;
}

Status WechatPayServerServiceImpl::Login(ServerContext *context,
                                         const Request *request,
                                         Response *reply)
{
    // 登录逻辑
    string username = request->username();
    string password = request->password();
    string deviceId = request->device_id();

    // grpc::string peerUri = context->peer();
    // cout << "ServerContext URI:" << peerUri << endl;

    cout << "登录服务，用户名：" << username << endl;
    cout << "请求登录的客户端的设备ID：" << deviceId << endl;

    // 查询数据库，验证密码hash
    handler = RedisHandler::GetInstance(REDIS_SERVER_IP, REDIS_SERVER_PORT);
    int ret = handler->Connect();
    if (ret != 0)
    {
        // redis连接失败
        reply->set_status_code(DB_NOT_CONNECTED);
    }
    else
    {
        string strVal;
        handler->GetString(username, strVal);
        if (strVal == "")
        {
            // 用户名尚未注册，不可登录
            reply->set_status_code(USER_NOT_FOUND);
        }
        else
        {
            // 用户已注册，验证密码hash
            if (BCrypt::validatePassword(password, strVal))
            {
                // 密码正确，登录成功，且分两种情况：
                // (1) 之前无在线终端
                // (2) 之前有在线终端，需将其踢下线
                // 故还需查询在线用户列表
                auto iter = active_users.find(username);
                if (iter != active_users.end())
                {
                    // 之前已登录，仍需验证密码
                    // 密码正确则将原终端踢下线，即修改在线用户列表
                    reply->set_status_code(USER_KICK_OTHERS_OFF_SUCCESS);
                    // 调用KickOff接口，注意这里要传的参数是旧设备ID
                    int ret = KickOff(context->peer(), username, active_users[username]);
                    if (ret != 0)
                    {
                        cout << "踢下线时发生错误！" << endl;
                    }
                }
                else
                {
                    // 未登录，则登录
                    reply->set_status_code(USER_REQUEST_SUCCESS);
                }
                active_users[username] = deviceId;
            }
            else
            {
                // 密码错误
                reply->set_status_code(USER_PASSWORD_WRONG);
            }
        }
    }

    return Status::OK;
}

int WechatPayServerServiceImpl::KickOff(const string &peerUri, const string &username, const string &deviceId)
{
    // 踢设备ID为DeviceId的设备下线，客户端的IP:port信息存储在peerUrl
    // 使用stub调用客户端的KickOff方法令其下线
    string key;
    string cert;
    string root;

    util::read(PATH_SERVER_CERT, cert);
    util::read(PATH_SERVER_KEY, key);
    util::read(PATH_CA_CERT, root);

    grpc::SslCredentialsOptions opts = {root, key, cert};
    // string client{peerUri}; // IPv6地址有坑，先提取出端口号吧

    // size_t pos = peerUri.find_last_of(":");
    // string port = peerUri.substr(pos + 1);
    // cout << "客户端端口号：" << port << endl;
    string client{"localhost:50052"};
    // cout << "客户端地址：" << client << endl;
    unique_ptr<ClientService::Stub> stub = ClientService::NewStub(CreateChannel(
        client, SslCredentials(opts)));
    ;

    // 构造请求
    Request request;
    request.set_username(username);
    request.set_device_id(deviceId);

    Response reply;
    ClientContext context;

    Status status = stub->KickOff(&context, request, &reply);  // 让客户端崩了
    if (status.ok())
    {
        // RPC调用成功。根据返回状态码确定注册结果
        switch (reply.status_code())
        {
        case USER_KICKED_OFF_SUCCESS:
            cout << "旧设备挤下线成功！" << endl;
            break;
        case USER_KICKED_OFF_FAILURE:
            cout << "旧设备挤下线失败！" << endl;
            return -1;
        }
    }
    else
    {
        // RPC调用失败
        cout << "挤下线失败：RPC调用失败！" << endl;
        cout << status.error_code() << ": " << status.error_message()
             << endl;
        return -1;
    }
    return 0;
}

Status WechatPayServerServiceImpl::Logout(ServerContext *context,
                                          const Request *request,
                                          Response *reply)
{
    // 登出逻辑
    string username = request->username();
    string deviceId = request->device_id();

    cout << "登出服务，用户名：" << username << endl;

    // 先在在线用户列表中寻找
    auto iter = active_users.find(username);
    if (iter != active_users.end())
    {
        // 该用户之前已登录，但还需判断在线设备是否仍是本终端
        // 是本终端则下线，即删除在线用户列表对应项
        // 否则报错，提示该终端已经下线
        if (deviceId == active_users[username])
        {
            // 在线设备仍是本终端
            active_users.erase(iter);
            reply->set_status_code(USER_REQUEST_SUCCESS);
        }
        else
        {
            // 在线设备不是本终端
            reply->set_status_code(USER_LOGIN_ANOTHER_PLACE);
        }
    }
    else
    {
        // 该用户已在别处登录并登出，将本终端踢出，本终端登出请求失败
        reply->set_status_code(USER_LOGIN_ANOTHER_PLACE);
    }
    return Status::OK;
}

Status WechatPayServerServiceImpl::Interact(ServerContext *context,
                                            const Request *request,
                                            Response *reply)
{
    // 交互逻辑
    string username = request->username();
    string deviceId = request->device_id();
    string echoContent = request->echo_content();

    cout << "交互服务，用户名：" << username << endl;

    // 查询在线用户列表
    auto iter = active_users.find(username);
    if (iter != active_users.end())
    {
        // 该用户已登录，接下来判断在线设备是否是本终端
        if (deviceId == active_users[username])
        {
            // 在线设备是本终端
            reply->set_status_code(USER_REQUEST_SUCCESS);
            reply->set_echo_content(echoContent);
        }
        else
        {
            // 在线设备不是本终端
            reply->set_status_code(USER_LOGIN_ANOTHER_PLACE);
        }
    }
    else
    {
        // 该用户已在别处登录并登出，将本终端踢出，本终端交互请求失败
        reply->set_status_code(USER_LOGIN_ANOTHER_PLACE);
    }

    return Status::OK;
}