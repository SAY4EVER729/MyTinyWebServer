## 同步、异步日志系统

日志系统大致可以分成两部分，其一是单例模式与阻塞队列的定义，其二是日志类的定义与使用。

* 阻塞队列（循环队列加入多线程多个控制流控制）
* 单例模式创建日志系统（单例模式、由循环队列构成，配合生产者消费者模式）
* 同步日志
* 异步日志
* 实现按填、超行分类

>整体描述：
1. 默认同步写数据
2. 同步write函数中,异步标志位为true且阻塞队列未满，生产者写数据至阻塞队列；否则，消费者将数据写入日志。
3. Log的init函数中，判断标志位为异步，则执行pthread_create多线程执行回调函数异步写数据至日志。

### 阻塞队列
* 循环数组构成
* 生产者消费者模型

  循环数组：```T *m_array``` 采用了模板泛型编程

生产者消费者模型：

  push 相当于生产者，用于给队列添加 T，也就是 实例化之后的string，
  当满产品时，  相当于队列写满 ```size >= max_size```，那么生产者会广播broadcast ，唤醒阻塞在条件变量上的消费者

  pop 相当于消费者，用于拿走队列中的 T(string), 当没有产品时，  相当于队列 ```size <= 0```，那么消费者需要阻塞在条件变量上，也就是 ```cond.timewait()```

注意：条件变量不是锁，锁和条件变量配合使用。
### 日志系统

#### 单例模式：
保证一个类仅有一个实例，并提供一个访问它的全局访问点，该实例被所有程序模块共享。
局部静态变量之线程安全的懒汉模式，使用函数内的局部静态对象，这种方法不用加锁和解锁操作。
```C++
class single{
private:
    single(){}
   ~single(){}
public:
    static single* getinstance();
};

single* single::getinstance(){
    static single obj;
    return &obj;
}
```


### pthread_create函数
创建一个新线程。	其作用，对应进程中fork() 函数。
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
	返回值：成功：0；	失败：错误号	-----Linux环境下，所有线程特点，失败均直接返回错误号。
参数：	
	pthread_t：当前Linux中可理解为：typedef  unsigned long int  pthread_t;
* 参数1：传出参数，保存系统为我们分配好的线程ID
* 参数2：通常传NULL，表示使用线程默认属性。若想使用具体属性也可以修改该参数。
* 参数3：函回调函数，该函数运行结束，则线程结束。
* 参数4：线程主函数执行期间所使用的参数，一般为NULL。

### 其他函数

```fputs()```

写文件

```fopen()```

打开文件

```snprintf()```

用于格式化输出字符串，并将结果写入到指定的缓冲区
