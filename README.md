## 题目描述
---
请设计一套单终端登录系统，具备以下功能：
1. 注册、登录功能
2. 一个用户只能在一个设备上登录，切换终端登录时，其他已登录的终端会被踢出
3. 后台可以根据管理策略将已登录设备踢出

请实现该系统的客户端及后台，要求:
1. 采用C++实现命令行客户端
2. 后台采⽤grpc实现，使用C++实现，⽤户信息存储可以采⽤用sql或nosql实现，设计时需要考虑敏感数据的安全性及传输安全性
3. 后台项目使用bazel编译及管理项目依赖
4. 项目设计实现周期3天左右，交付物包括可运行的客户端及后台项目代码，设计文档，使用github管理相关资源

## 实现思路
---
### 客户端
客户端通过**UUID**唯一标识，并用本地标志位`isLogin`保持登录状态，远程调用服务端方法`Register`、`Login`、`Logout`与`Interact`分别执行注册、登录、登出及交互操作。
#### 注册
客户端发送注册请求无需提前登录，在请求中设置用户名与密码，根据返回状态码确定注册结果。
#### 登录
客户端发送登录请求前需先判断当前是否已登录，若已登录(`isLogin`为`true`)则不发送请求，未登录时才发送请求，并在请求中设置用户名、密码与设备ID(**UUID**)，根据返回状态码确定登录结果。
#### 登出
客户端发送登出请求前需先判断当前是否已登录，若未登录(`isLogin`为`false`)则不发送请求，已登录时才发送请求，并在请求中设置用户名与设备ID(**UUID**)，根据返回状态码确定登录结果。
#### 交互
客户端发送交互请求需先判断当前是否已登录，若未登录(`isLogin`为`false`)则不发送请求，已登录时才发送请求，并在请求中设置用户名、设备ID(**UUID**)与回显信息，根据返回状态码确定登录结果。

### 服务端
服务端采用**Redis**存储用户数据(用户名与密码hash值)，使用**Bcrypt**算法对密码加密后存储，定义并实现了注册、登录、登出以及交互四种服务供客户端调用。服务端在本地利用一个`map`变量存储在线用户列表(用户名到设备ID的映射)，用户在新终端登录后会修改该列表，从而将旧终端踢出，并在旧终端下次请求服务时通过返回状态码告知其用户已在别处登录，即已被踢出。
#### 注册
服务端接收到注册请求后，提取出用户名与密码，以用户名为**Key**查询Redis，若Redis连接失败或用户名已存在则注册失败，并通过状态码加以区分，否则将用户名及密码hash值存入Redis，返回注册成功。

#### 登录
服务端接收到登录请求后，提取出用户名、密码与设备ID，首先以用户名为**Key**查询Redis，若用户名存在则验证密码正确性。密码正确则再查询在线用户列表，若之前已登录则将旧终端踢出，未登录则正常登录，并修改在线用户列表。Redis连接失败、密码错误或用户名不存在则登录失败，并通过状态码加以区分。

#### 登出
服务端接收到登出请求后，提取出用户名与设备ID，查询在线用户列表，若用户已在线并且设备ID与请求终端的设备ID相同，则正常下线，删除在线用户列表中对应项。若设备ID与请求终端的设备ID不同，则用户已在别处登录，并v通过状态码告知请求终端。

#### 交互
服务端接收到交互请求后，提取出用户名、设备ID与回显内容，查询在线用户列表，若用户已在线并且设备ID与请求终端的设备ID相同，则正常回显。若设备ID与请求终端的设备ID不同，则用户已在别处登录，并通过状态码告知请求终端。

### 状态码
服务端通过在回复`Response`结构中设置状态码以区分不同类型的请求处理结果及对应原因。
类似于HTTP协议，状态码由三个十进制数字组成，第一个十进制数字定义了状态码的类型，后两个数字没有分类的作用，而用于区分导致该处理结果的不同原因。HTTP状态码共分为3种类型：

|分类|分类描述|
|:-:|:-|
|2xx|成功，请求被成功接收并处理|
|4xx|客户端错误|
|5xx|服务端错误|

状态码列表如下：

|状态码|常量|描述|   
|:-:|:-:|:-|
|200|USER_REQUEST_SUCCESS|用户请求处理成功|
|201|USER_KICK_OTHERS_OFF|用户请求处理成功，并将旧登录设备踢出，用于登录操作|
|400|USER_ALREADY_EXIST|用户名已存在，用于注册操作|
|401|USER_NOT_FOUND|用户名不存在，用于登录操作|
|402|USER_PASSWORD_WRONG|密码不正确，用于登录操作|
|403|USER_LOGIN_ANOTHER_PLACE|用户已在别处登录，用于登出及交互操作|
|500|DB_NOT_CONNECTED|数据库连接失败，服务端错误|

## 编译运行
---
### 依赖安装
源代码依赖库：
1. [**grpc**](https://github.com/grpc/grpc)
2. [**hiredis**](https://github.com/redis/hiredis)
3. [**libbcrypt**](https://github.com/trusch/libbcrypt)

以上依赖均以子模块(**submodules**)形式添加到本项目，执行以下命令将依赖下载到本地：
```shell
git clone 
git submodule update --init
```

执行以下命令配置**grpc**依赖：
```shell
cd third_party/grpc
git submodule update --init # 将grpc的依赖下载到本地
# 配置grpc的protobuf依赖
cd third_party/protobuf
make && sudo make install # 安装protobuf依赖
cd ..
make && sudo make install # 安装grpc依赖
```

执行以下命令配置**hiredis**依赖：
```shell
cd third_party/hiredis
make & sudo make install # 安装hiredis依赖
```

执行如下命令安装**libbcrypt**依赖：
```shell
cd third_party/libbcrypt
make & sudo make install # 安装libbcrypt依赖
```

### Redis部署
采用**Docker**部署**Redis**服务，安装**Docker**后执行如下命令：
```shell
docker pull redis
docker run -d -p 6379:6379 redis 
```
在Docker中以守护进程模式启动Redis，并映射到外部端口6379

### 证书密钥
已提供测试文件，也可通过执行以下命令以生成测试证书与密钥文件：
```shell
cd crt
./gen-crt.sh
```
ps.
> 特别注意`client.cc`与`server.cc`代码文件中使用的证书及密钥文件路径，此处采用绝对路径，需根据个人实际运行环境修改

### 编译
支持**make**与**bazel**两种构建方式，编译生成两个可执行文件：`server`和`client`
#### make
执行如下命令编译：
```shell
cd src
make -j
```
#### bazel
```shell
bazel build //src:all
```
### 运行
```shell
cd bazel-bin/src
# 第一个终端启动服务器
./server
# 其它终端启动客户端
./client
```
### 系统截图
#### 客户端运行功能测试
![客户端运行功能测试](https://github.com/sundongxu/wechatpay-test/raw/master/img/client_normal_test.png)

#### 踢下线功能测试
第一个登录的客户端：
![第一个登陆的客户端](https://github.com/sundongxu/wechatpay-test/raw/master/img/client_terminal_1.png)

另一个登录相同用户的客户端：
![另一个登录的客户端](https://github.com/sundongxu/wechatpay-test/raw/master/img/client_terminal_2.png)

#### 后端服务调用
服务器服务调用情况：
![服务端服务调用](https://github.com/sundongxu/wechatpay-test/raw/master/img/server.png)

## 完成进度
---
### Day 1(8月7日，周二)
1. 编译安装GRPC及其全部依赖(已完成)
2. 编写`demo.proto`协议文件，及对应client、server相关功能及交互代码(部分完成)
3. 基于openssl编写证书生成脚本`gen-crts.sh`，利用证书和GPRC提供的TLS/SSL支持实现客户端与服务端的双向认证和加密传输(已完成)

### Day 2(8月11日，周六)
1. 安装docker，在docker中部署Redis服务依赖，编写`Dockerfile`一键部署Redis服务
2. 编译安装hiredis(Redis C++客户端库)、libcrypt(Bcrypt加密算法C++库)
3. bazel构建项目(暂未完成，报错头文件未找到，#include文件路径问题)
4. 编写client、server相关功能及交互代码(全部完成)

### Day 3(8月12日，周日)
1. 更新`Makefile`，编译通过
2. 调试功能代码，测试通过
3. 修改`BUILD`文件，bazel编译通过
