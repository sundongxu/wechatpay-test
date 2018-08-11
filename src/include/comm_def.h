#ifndef COMM_DEF_H
#define COMM_DEF_H

// Redis地址
#define REDIS_SERVER_IP "127.0.0.1"
#define REDIS_SERVER_PORT 6379

// 用户操作类型
#define CMD_LOGIN 1
#define CMD_REGISTER 2
#define CMD_INTERACT 3
#define CMD_LOGOUT 4
#define CMD_QUIT 5

// 操作处理结果
#define SUCCESS 0
#define FAILURE -1

// 服务端响应(Response)状态码
// 请求处理成功
#define USER_REQUEST_SUCCESS 200 // 请求处理成功
#define USER_KICK_OTHERS_OFF 201 // 本终端端成功登录，将之前登录的终端踢下线
// 客户端错误
#define USER_ALREADY_EXIST 400       // 注册：用户已存在
#define USER_NOT_FOUND 401           // 登录：用户不存在
#define USER_PASSWORD_WRONG 402      // 登录：密码错误
#define USER_LOGIN_ANOTHER_PLACE 403 // 交互：用户已在别处登录
// 服务端错误
#define DB_NOT_CONNECTED 500 // 数据库连接失败

#endif // COMM_DEF_H