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


### 服务端
#### 注册

#### 登录

#### 登出


## 编译运行
---
### 编译
目标采用`bazel`，暂时采用`make`

### 运行


## 完成进度
---
### 8月7日(周二)
1. 编译安装GRPC及其全部依赖(已完成)
2. 编写`demo.proto`协议文件，及对应client、server相关功能及交互代码(部分完成)
3. 基于openssl编写证书生成脚本`gen-crts.sh`，利用证书和GPRC提供的TLS/SSL支持实现客户端与服务端的双向认证和加密传输(已完成)

### 8月日(周三)
1. 安装docker，在docker中部署Redis服务依赖，编写`Dockerfile`一键部署Redis服务
2. 编译安装hiredis(Redis C++客户端库)、libcrypt(Bcrypt加密算法C++库)
3. 

### 8月9日(周四)


### 8月10日(周五)