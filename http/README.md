# http 连接处理

## 主要函数

```C++
#include <sys/epoll.h>
// 用于创建epoll内核事件表的文件描述符，底层实现为红黑树
int epoll_create(int size)

// 该函数用于操作内核事件表监控的文件描述符上的事件：注册、修改、删除
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)

// 用于等待所监控文件描述符上有事件的产生，返回就绪的文件描述符个数
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
```

### epoll 有两种模式：

select 、 poll 、 epoll 三种Linux下 实现 I/O 复用的系统调用

1. LT 模式(level Trigger) 默认工作模式，相当于一个效率较高的 poll

>  当```epoll_wait()```检测到有事件发生，可以不立即处理该事件，下一次调用 ```epoll_wait()```时，还会再次向应用程序通告此事件，直到该事件被处理。

2. ET 模式(edge trigger) 高效工作模式，但是只支持 非阻塞模式。 --- 忙轮询。
 
>  当```epoll_wait()```检测到有事件发生，通知应用程序，必须立刻中断其他，处理该事件，后续的```epoll_wait()```调用将不再向应用程序通知，ET模式减少了同一个epoll事件的触发次数，效率进行了提高。

3. EPOLLONESHOT

> 即使使用 ET 模式，一个 socket 上的某个事件还有可能被多次触发，因为：<br>
一个线程处理某个socket，处理过程中，又有新数据可读。这时，另一个线程被唤醒，两个线程操作同一个 socket
<br> 希望一个 socket 连接只有一个线程处理，在其处理期间，其他线程无法处理，
<br> 所以使用 ```epoll_ctl()```在文件描述符上注册EPOLLONESHOT事件。该线程处理完后，需要通过 ```epoll_ctl()```重置EPOLLONESHOT事件（只有这样，该线程操作完之后，其他线程才能继续操作）

## 有限状态机

已经通过 连接池README 中的概念知道了 I/O处理单元、请求队列和逻辑单元之间的协调完成各种任务后，逻辑单元本身，其内部，有一种高效编程方法：有限状态机(finite state machine)

带有状态转移的有限状态机：

```C++
STATE_MACHINE(){
    State cur_State = type_A;
    while(cur_State != type_C){
        Package _pack = getNewPackage();
        switch(){
            case type_A:
                process_pkg_state_A(_pack);
                cur_State = type_B;
                break;
            case type_B:
                process_pkg_state_B(_pack);
                cur_State = type_C;
                break;
        }
    }
}
```
type_A是初始状态，type_C是结束状态。

通过当前状态对数据进行处理，然后再通过cur_State完成状态转移。

服务器可以根据不同状态或消息类型进行相应的处理逻辑，使得程序清晰易懂。


### 有限状态机实用

```C++
	//报文的请求方法，本项目只用到GET和POST
    enum METHOD{GET=0,POST,HEAD,PUT,DELETE,TRACE,OPTIONS,CONNECT,PATH}; 
    //主状态机的状态 
    enum CHECK_STATE{CHECK_STATE_REQUESTLINE=0,CHECK_STATE_HEADER,CHECK_STATE_CONTENT};
    //报文解析的结果
    enum HTTP_CODE{NO_REQUEST,GET_REQUEST,BAD_REQUEST,NO_RESOURCE,FORBIDDEN_REQUEST,FILE_REQUEST,INTERNAL_ERROR,CLOSED_CONNECTION};
    //从状态机的状态
    enum LINE_STATUS{LINE_OK=0,LINE_BAD,LINE_OPEN};
```

http 类：

```C++
    //主状态机解析报文中的请求行数据
    HTTP_CODE parse_request_line(char *text);
    //主状态机解析报文中的请求头数据
    HTTP_CODE parse_headers(char *text);
    //主状态机解析报文中的请求内容
    HTTP_CODE parse_content(char *text);
    //生成响应报文
    HTTP_CODE do_request();

    //m_start_line是已经解析的字符
    //get_line用于将指针向后偏移，指向未处理的字符
    char* get_line(){return m_read_buf+m_start_line;};

    //从状态机读取一行，分析是请求报文的哪一部分
    LINE_STATUS parse_line();
```











