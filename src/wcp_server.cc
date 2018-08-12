#include "entity.h"
#include "comm_def.h"
#include "BCrypt.hpp"

Status WechatPayServiceImpl::Register(ServerContext *context,
                                      const Request *request,
                                      Response *reply)
{
    cout << "Register Invoked!" << endl;
    // 注册逻辑
    string username = request->username();
    string password = request->password();

    cout << "用户名：" << username << ", 密码：" << password << endl;

    // 查询是否已存在相同用户名
    cout << "Redis Server IP(Default):" << REDIS_SERVER_IP << ", Port(Default):" << REDIS_SERVER_PORT << endl; 
    handler = RedisHandler::GetInstance(REDIS_SERVER_IP, REDIS_SERVER_PORT);
    cout << "RedisHandler Got" << endl;
    int ret = handler->Connect();
    if (ret != 0)
    {
        // redis连接失败
        reply->set_status_code(DB_NOT_CONNECTED);
    }
    else
    {
        cout << "Redis Connected!" << endl;
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
    cout << "Register Finished!" << endl;
    return Status::OK;
}

Status WechatPayServiceImpl::Login(ServerContext *context,
                                   const Request *request,
                                   Response *reply)
{
    // 登录逻辑
    string username = request->username();
    string password = request->password();
    string deviceId = request->device_id();

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
                    reply->set_status_code(USER_KICK_OTHERS_OFF);
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

Status WechatPayServiceImpl::Logout(ServerContext *context,
                                    const Request *request,
                                    Response *reply)
{
    // 登出逻辑
    string username = request->username();
    string deviceId = request->device_id();

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
    return Status::OK;
}

Status WechatPayServiceImpl::Interact(ServerContext *context,
                                      const Request *request,
                                      Response *reply)
{
    // 交互逻辑
    string username = request->username();
    string deviceId = request->device_id();
    string echoContent = request->echo_content();

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

    return Status::OK;
}