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
客户端通过**UUID**唯一标识，并用本地标志位`isLogin`保持登录状态，远程调用服务端方法`Register`、`Login`、`Logout`与`Interact`分别执行注册、登录、登出及交互操作。为实现单终端登录功能，客户端成功登录后将创建一个单独线程，在其中调用服务端`KeepAlive`(保活)服务，该服务阻塞，直至出现下述两种场景时方才返回：</br>
① 该用户在别处登录，此时服务端将回写一个强制下线消息(**USER_KICK_OFF**，状态码含义见后文) </br>
② 该用户执行正常下线操作(登出或退出)</br>
`KeepAlive`调用返回后，客户端解析回包类型(状态码)，输出提示语，并还原本地登录态
#### 注册
客户端发送注册请求无需提前登录，在请求中设置用户名与密码，根据返回状态码确定注册结果。
#### 登录
客户端发送登录请求前需先判断当前是否已登录，若已登录(`isLogin`为`true`)则不发送请求，未登录时才发送请求，并在请求中设置用户名、密码与设备ID(**UUID**)，根据返回状态码确定登录结果。
#### 登出
客户端发送登出请求前需先判断当前是否已登录，若未登录(`isLogin`为`false`)则不发送请求，已登录时才发送请求，并在请求中设置用户名与设备ID(**UUID**)，根据返回状态码确定登录结果。
#### 交互
客户端发送交互请求需先判断当前是否已登录，若未登录(`isLogin`为`false`)则不发送请求，已登录时才发送请求，并在请求中设置用户名、设备ID(**UUID**)与回显信息，根据返回状态码确定登录结果。
#### 保活
客户端登录成功后即创建监听线程，在该线程中发送请求，并在请求中设置用户名，调用返回时即还原本地登录态。

### 服务端
服务端采用**Redis**存储用户数据(用户名与密码hash值)，使用**Bcrypt**算法对密码加密后存储，定义并实现了注册、登录、登出以及交互四种服务供客户端调用。</br>
服务端在本地利用一个`map<string, string>`类型变量`active_users`存储在线用户列表(用户名到设备ID的映射)，用户在新终端登录时，服务端更新在线用户列表，通过观察者(条件变量)通知`KeepAlive`服务返回，并回写一个强制下线消息(状态码**USER_KICK_OFF**)

#### 注册
服务端接收到注册请求后，提取出用户名与密码，以用户名为**Key**查询Redis，若Redis连接失败或用户名已存在则注册失败，并通过状态码加以区分，否则将用户名及密码hash值存入Redis，返回注册成功。

#### 登录
服务端接收到登录请求后，提取出用户名、密码与设备ID，首先以用户名为**Key**查询Redis，若用户名存在则验证密码正确性。密码正确则再查询在线用户列表，若之前已登录则通知保活服务返回，并回写一个强制下线消息(状态码**USER_KICK_OFF**)，客户端接收后即还原登录态，从而实现将旧终端踢出。未登录则正常登录，并修改在线用户列表。Redis连接失败、密码错误或用户名不存在则登录失败，并通过状态码加以区分。

#### 登出
服务端接收到登出请求后，提取出用户名与设备ID，查询在线用户列表，若用户已在线并且设备ID与请求终端的设备ID相同，则正常下线，删除在线用户列表中对应项。若设备ID与请求终端的设备ID不同，则用户已在别处登录，并通过状态码告知请求终端。通知保活服务返回，不回写消息表明是正常下线逻辑，客户端调用返回后即还原登录态。

#### 交互
服务端接收到交互请求后，提取出用户名、设备ID与回显内容，查询在线用户列表，若用户已在线并且设备ID与请求终端的设备ID相同，则正常回显。若设备ID与请求终端的设备ID不同，则用户已在别处登录，并通过状态码告知请求终端。

#### 保活
服务端提供此服务给客户端的监听线程调用，将为该在线用户注册一个新观察者，用户在该终端登录期间服务一直阻塞不返回，阻塞采用条件变量(观察者)调用`wait`方法实现，每个在线用户都对应一个观察者条件变量，存储在一个`map<string, condition_variable *>`类型变量`observers`中。当前已在线用户在新终端调用`Login`服务将调用`notify_all`方法唤醒观察者条件变量，然后注销观察者，使得保活服务返回，并回写一个强制下线消息，客户端接收到此消息后将提示用户已被踢出，并还原登录态。此外，用户在执行登出或退出操作时均会发起`Logout`服务调用，故在`Logout`服务中也要唤醒观察者，然后注销观察者，但是并不回写任何类型的消息，以与强制下线情况区分，客户端监听线程`KeepAlive`调用返回后仅需还原其本地登录态即可。

### 时序图
本项目前后端交互时序图如下：
![正常交互逻辑](https://github.com/sundongxu/wechatpay-test/raw/master/img/sequence/normal.png)

正常交互逻辑和下线消息监听逻辑属于不同线程，不太方便在同一时序图中给出，单独给出监听调用时序图如下：
![监听交互逻辑](https://github.com/sundongxu/wechatpay-test/raw/master/img/sequence/kickoff.png)

### 踢下线功能
先后实现了`Pull`和`Push`两种方式，共三个版本，当前最新版本采用`Push`方式，利用`GRPC`提供的`Streaming RPC`能力实现。各版本说明如下：

#### Pull方式
服务端在检测到一个已在线用户在新终端发起登录请求后，更新在线用户列表，为新终端正常提供服务。
而对于旧终端，则在其下次请求服务时，通过返回状态码告知其用户已在别处登录，即其已被挤下线，需要访问服务则需先登录，客户端接收到此回复后还原其本地登录态。

#### Push方式(反向RPC)
客户端登录成功后，将新建一个单独的线程，通过`GRPC`创建一个监听服务器实例，监听来自服务端的下线请求。
服务端在检测到一个已在线用户在新终端发起登录请求后，先通过`GRPC`主动调用客户端提供的强制下线服务接口，随后更新在线用户列表。
客户端则在接收到一个来自服务端的强制下线请求后，还原本地登录态，取消监听线程并回收其占用资源。

#### Push方式(Streaming RPC))
服务端增加一个`KeepAlive`服务，客户端登录成功后新建监听线程并调用此服务。`KeepAlive`服务阻塞直至 
> (1) 当前已在线用户在新设备上登录，需将旧设备踢下线，服务端发送一个强制下线消息(状态码**USER_KICK_OFF**)</br>
> (2) 当前已在线用户发起`Logout`调用请求(登出或退出)，即正常下线，此时服务端不回写任何类型的消息。</br>

此服务调用返回后，客户端根据接收到的消息类型输出用户提示，并还原本地登录态。

### 协议
本项目采用`ProtoBuf`作为前后端的通信协议，基于`Proto3`语法在`demo.proto`协议文件中定义`GRPC`消息结构与服务接口。

#### 消息结构
客户端请求消息包含4个属性：用户名、(SSL/TLS加密传输的)密码、设备ID(UUID)、回显内容(仅用于`Interact`服务)。
服务端回复消息包含2个属性：状态码(区分不同请求处理结果及其对应原因)、回显内容(仅用于`Interact`服务)。
在`demo.proto`中定义消息结构如下：
```demo.proto
// 请求参数结构
message Request {
    string username = 1;                            // 用户名
    string password = 2;                            // 密码
    string device_id = 3;                           // 设备唯一标识uuid
    string echo_content = 4;                        // 请求回显内容
}

// 响应参数结构
message Response {
    int32 status_code = 1;                          // 状态码
    string echo_content = 2;                        // 回复回显内容
}
```
#### 服务接口
服务端提供4个服务接口，定义如下：
```demo.proto
service ServerService {
    rpc Register (Request) returns (Response) {}           // 注册服务
    rpc Login (Request) returns (Response) {}              // 登录服务
    rpc Logout (Request) returns (Response) {}             // 登出服务
    rpc Interact (Request) returns (Response) {}           // 交互服务(回显echo)
    rpc KeepAlive (Request) returns (stream Response) {}   // 保活服务(终端登录后发起，服务端阻塞直至新终端登录，将旧终端踢出
    // KeepAlive调用返回则客户端应该下线，包括强制下线(当前已在线用户在别处登录)和正常下线
}
```

##### 状态码
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
|100|USER_KICK_OFF|特殊类型，强制下线消息|
|200|USER_REQUEST_SUCCESS|用户请求处理成功|
|201|USER_KICK_OTHERS_OFF_SUCCESS|用户请求处理成功，并将旧登录设备踢出，用于登录操作|
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

以上依赖均通过bazel管理，一键编译安装：
```shell
bazel build //src:all
```

### Redis部署
采用**Docker**部署**Redis**服务，安装**Docker**后执行如下命令：
```shell
docker pull redis
docker run -d -p 6379:6379 redis 
```
在Docker中以守护进程模式启动Redis，并映射到外部端口6379

### 证书密钥
利用`GRPC`提供的**SSL/TLS**加密传输能力，已提供测试所用证书及密钥文件，也可通过执行以下命令以生成测试证书与密钥文件：
```shell
cd crt
./gen-crt.sh
```
ps.
> 特别注意，本项目用到的测试证书及密钥文件路径在`src/include/comm_def.h`头文件中指定，此处暂时采用绝对路径，需根据个人实际运行环境修改

### 编译
支持**bazel**与**make**两种构建方式，编译生成两个可执行文件：`server`和`client`
#### bazel
```shell
bazel build //src:all
```
上述命令配置安装依赖同时编译工程，输出的可执行文件位于路径`bazel-bin/src`下
#### make
本项目默认支持**bazel**编译，如要采用**make**编译，需修改部分文件的头文件**#include**路径
执行如下命令编译：
```shell
cd src
make -j
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
客户端各项功能测试：
![客户端运行功能测试](https://github.com/sundongxu/wechatpay-test/raw/master/img/running/version_3_push_streaming_rpc/normal_test_client.png)

对应后端服务调用情况：
![服务端运行功能测试](https://github.com/sundongxu/wechatpay-test/raw/master/img/running/version_3_push_streaming_rpc/normal_test_server.png)

#### 踢下线功能测试
第一个登录的客户端：
![第一个登陆的客户端](https://github.com/sundongxu/wechatpay-test/raw/master/img/running/version_3_push_streaming_rpc/kickoff_test_terminal_old.png)

另一个登录相同用户的客户端：
![另一个登录的客户端](https://github.com/sundongxu/wechatpay-test/raw/master/img/running/version_3_push_streaming_rpc/kickoff_test_terminal_new.png)

对应后端服务调用情况：
![服务端服务调用](https://github.com/sundongxu/wechatpay-test/raw/master/img/running/version_3_push_streaming_rpc/kickoff_test_server.png)
