## 定时器处理非活动连接

由于非活跃连接会占用连接资源，影响服务器性能，所以需要定时断开非活跃的连接用户，释放资源

* 统一事件源
* 定时器由升序链表的类构成
* 处理非活动连接

## lst_timer

升序链表的定时器，包含两个成员：一个超时时间，一个任务回调函数

每个定时器包含下一个定时器的指针。

```tick()```相当于心搏函数，每隔一段时间执行一次，用来检测并处理到期的任务

```adjust_timer()```相当于调整函数，某个定时任务发生变化时，需要调整到对应的位置，只考虑被调整的定时器时间延长的情况，所以该定时器往往要到链表的尾部方向移动（升序）

```add_timer()```和```del_timer()``` 添加/删除指定定时器。

## Utils

处理非活动连接的**实际应用**

由alarm 和 setitimer 函数设置的实时闹钟一旦超时，将触发SIGALARM信号，利用该信号的信号处理函数可以处理定时任务。

定时处理任务，实际就是调用 ```tick()```

回调函数```cb_func()```作用：通过epoll_ctl删除socket上的非活动连接，并关闭。
