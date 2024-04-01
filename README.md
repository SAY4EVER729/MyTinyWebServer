# 基于Linux编程的高性能Web服务器

## 简介

本项目参考[qingguoyi项目](https://github.com/qinguoyi/TinyWebServer)；游双《Linux高性能服务器编程》

用于学习 Linux C++ 轻量级 Web 服务器编程

1、通过数据库连接池，实现客户端用户登录、注册功能，可以请求服务器图片和视频文件；
2、设计同步日志系统，以单例模式创建日志，基于阻塞队列实现异步日志记录，增强服务器并发能力；
3、设计使用线程池，减少动态增删线程的开销，增加服务其资源利用率；
4、针对非活跃连接，添加定时器，以双向链表方式保存计时器，并通过管道通信通知主线程。

## 运行

* 服务器环境
  腾讯云 Ubuntu 22.04<br>
  MySQL 8.0.36

* 浏览器测试 ：

  Safari<br>
  Chrome<br>
  其他暂无测试

### 1. 环境

保证拥有 MySQL 最新环境

确保拥有 mysql.h 头文件<br>
```bash
sudo apt-get install libmysqlclient-dev
```

### 2. 克隆仓库

```git@github.com:SAY4EVER729/MyTinyWebServer.git```  或 通过 HTTPS 链接下载```https://github.com/SAY4EVER729/MyTinyWebServer.git```


### 3. 创建 MySQL 数据库并绑定数据库账号密码

 创建 MySQL 数据库<br>
```MySQL
// 输入用户密码进入mysql
mysql -u root -p
// 创建数据库 yourdb
create database yourdb;
// 创建 user 表
USE yourdb;
CREATE TABLE user(
    username char(50) NULL,
    passwd char(50) NULL
)ENGINE=InnoDB;
// 添加数据
INSERT INTO user(username, passwd) VALUES('name', 'passwd');
```
绑定数据库账号密码

```bash
cd main.cpp

//数据库登录名,密码,库名
string user = "root";
string passwd = "root";
string databasename = "yourdb";

```

### 4. 编译并运行

内置 shell 自动编译

```bash
sh ./build.sh
# 启动
./server
```

### 5. ip 端口

端口默认9006，访问需要 hppt://ipv4地址 + 端口号，例如：

可以直接访问本项目 ```http://120.53.228.144:9006```

## 测试

webbench 压测：

腾讯云 2 G 2 核下：

1000 并发：

![ec6234a84562f4284beff56a1d7ce03](https://github.com/SAY4EVER729/MyTinyWebServer/assets/101921097/7cec5ac2-f9a8-40fa-851a-c91df0143a8a)


3000 并发：

![138384de0b6a2c097a4e67992732489](https://github.com/SAY4EVER729/MyTinyWebServer/assets/101921097/2ebdef56-84ce-463b-ad94-832d28c7cb25)



本项目编写的顺序（每个目录中都有相应的学习记录Readme）：

1. 首先编写的是用于控制多线程并发的锁类对象，易于代码复用，具体参见[locker_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/lock)

2. 编写服务器的灵魂（个人认为）--> 日志系统，具体参见[log_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/log)

3. 编写用于改善服务器性能的机制，该机制由升序链表构成，用于处理非活跃连接，具体参见[lst_timer_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/timer)

4. http类，涉及 有限状态机、 I/O 复用 、 HTTP 相关知识，见[http_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/http)

5. 连接池的实现，连接池的作用、逻辑、用到的函数见[CGImysql_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/GCImysql#readme)

6. 学习锁、信号、信号量前一定要掌握多进程多线程，但是需要先实现锁类编写，以下是线程池实现，具体参见[threadpool_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/threadpool#readme)

7. other : html 目录文件参见[root_readme](https://github.com/SAY4EVER729/MyTinyWebServer/blob/master/root/README.md)

持续更新中。。。
