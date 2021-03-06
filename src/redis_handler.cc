#include "src/include/redis_handler.h"
#include <iostream>

using namespace std;

RedisHandler *RedisHandler::handler = nullptr;

RedisHandler *RedisHandler::GetInstance(const char *ip, const int port)
{
    if (handler == nullptr)
    {
        handler = new RedisHandler(ip, port);
    }
    return handler;
}

void RedisHandler::DestoryInstance()
{
    if (handler != nullptr)
    {
        delete handler;
        handler = nullptr;
    }
}

int RedisHandler::Connect()
{
    struct timeval timeout = {1, 500000};
    context = redisConnectWithTimeout(svrIP, svrPort, timeout); // docker部署redis服务并映射到外部端口6379
    if (context != nullptr && context->err != 0)
    {
        cout << "连接到Redis服务时出错" << endl;
        return -1;
    }
    // cout << "Redis连接成功！" << endl;
    return 0;
}

void RedisHandler::DisConnect()
{
    redisFree(context);
    cout << "Redis连接断开！" << endl;
}

void RedisHandler::FreeReply()
{
    if (reply != nullptr)
    {
        freeReplyObject(reply);
        reply = nullptr;
    }
}

void RedisHandler::SetString(const string &key, const string &value)
{
    SETSTRING(key, value);
}

void RedisHandler::SetString(const string &data)
{
    FreeReply();
    reply = (redisReply *)redisCommand(context, data.c_str());
    if (!IsError())
    {
        if (!(reply->type == REDIS_REPLY_STATUS && strcmp(reply->str, "OK") == 0))
        {
            cout << "执行SET时出错！" << endl;
            freeReplyObject(reply);
            redisFree(context);
            return;
        }
    }
}

void RedisHandler::GetString(const string &key, string &value)
{
    FreeReply();
    reply = (redisReply *)redisCommand(context, "GET %s", key.c_str());
    if (!IsError() && reply->type == REDIS_REPLY_STRING)
    {
        value = reply->str;
    }
}

bool RedisHandler::IsError()
{
    if (nullptr == reply)
    {
        // 连接异常，断开重连
        cout << "Redis连接异常，尝试重连..." << endl;
        FreeReply();
        DisConnect();
        Connect();
        return true;
    }
    return false;
}