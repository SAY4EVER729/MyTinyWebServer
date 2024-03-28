/* 日志系统的类
 * 单例模式，懒汉模式 
 * 私有化构造函数，以防止外界创建单例类的对象；
 * 使用类的私有静态指针变量指向类的唯一实例，并用一个公有的静态方法获取该实例
 * 实例的初始化放在getinstance函数内部
 * 经典的线程安全懒汉模式，使用双检测锁模式（p == NULL检测了两次）
 * 利用局部静态变量实现线程安全懒汉模式*/

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <pthread.h>
#include "block_queue.h"

using namespace std;

class Log {
public:
	/* 对外提供一个公有的静态方法，获取指向单例的指针 */
	static Log *get_instance(){
		static Log instance;
		return &instance;
	}
	
	/* 异步写 */
	static void *flush_log_thread(void *args){
			Log:: get_instance()->async_write_log();
	}

	//可选择的参数有日志文件、日志缓冲区大小、最大行数以及最长日志条队列
    bool init(const char *file_name, int close_log, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);

    void write_log(int level, const char *format, ...);

    void flush(void);
private:
	Log();			// 私有化构造函数

	virtual ~Log(); // 私有化析构函数
					
	/* 用于异步写操作 */
	void *async_write_log() {
		string single_log;
		// 从阻塞队列中取出日志string，用fputs写入文件
		while (m_log_queue->pop(single_log)) {
			m_mutex.lock();
			fputs(single_log.c_str(), m_fp);
			m_mutex.unlock();
		}
	}

private:
	char dir_name[128]; 	// 路径名
	char log_name[128];	 	// log 文件名
	int  m_split_lines;		// 日志的最大行数
	int  m_log_buf_size;	// 缓冲区大小
	long long m_count;		// 日志行数记录
  	int  m_today;			// 按天分类，用于记录当前时间处于哪一天
	FILE *m_fp;				// 用于打开 log 文件的指针
	char *m_buf;		
	block_queue<string> *m_log_queue;	//阻塞队列
	bool m_is_async;		// 是否同步标志位
	locker m_mutex;
	int m_close_log;		// 关闭日志	
};

#define LOG_DEBUG(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(0, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(1, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(2, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(3, format, ##__VA_ARGS__); Log::get_instance()->flush();}

#endif
