# 基于Linux编程的高性能Web服务器

本项目参考[qingguoyi项目](https://github.com/qinguoyi/TinyWebServer)；游双《Linux高性能服务器编程》

本项目编写的顺序：

1. 首先编写的是用于控制多线程并发的锁类对象，易于代码复用，具体参见[locker_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/lock)

2. 编写服务器的灵魂（个人认为）--> 日志系统，具体参见[log_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/log)

3. 编写用于改善服务器性能的机制，该机制由升序链表构成，用于处理非活跃连接，具体参见[lst_timer_readme](https://github.com/SAY4EVER729/MyTinyWebServer/tree/master/timer)

仍在更新中。。。
