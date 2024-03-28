/* 参考游双《Linux高性能服务器编程》p260
 * 线程同步机制封装3个类：锁mutex 、 信号量 sem 、条件变量 cond
 * 用于多线程复用，函数被多线程同时调用不发生竟态条件
 */
#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

// 封装信号量 semaphare 的类
class sem {
public:
	/* 创建并初始化信号量 */
	sem() {
		if (sem_init(&m_sem, 0, 0) != 0) {
			// 构造函数没有返回值，抛出异常
			throw std:: exception();
		}
	}

	/* 创建 num 信号量 */
	sem(int num) {
		if (sem_init(&m_sem, 0, num) != 0) {
				throw std:: exception();
		}
	}

	/* 销毁信号量 */
	~sem() {
		sem_destroy(&m_sem);
	}

	/* 等待信号量 */
	inline bool wait() {
		return sem_wait(&m_sem) == 0;
	}

	/* 增加信号量 */
	inline bool post(){
		return sem_post(&m_sem) == 0;
	}
private:
	sem_t m_sem;
};

/* 封装互斥锁的类 */
class locker{
public:
	/* 创建并初始化互斥锁*/
	locker(){
		if (pthread_mutex_init(&m_mutex, NULL) != 0) {
			throw std::exception();
		}
	}

	/* 销毁互斥锁 */
	~locker(){
		pthread_mutex_destroy(&m_mutex);
	}

	/* 获取互斥锁 */
	bool lock(){
		return pthread_mutex_lock(&m_mutex) == 0;
	}

	/* 释放互斥锁 */
	bool unlock() {
		return pthread_mutex_unlock(&m_mutex) == 0;
	}
	pthread_mutex_t *get() {
		return &m_mutex;
	}

private:
	pthread_mutex_t m_mutex;
};

/* 封装条件变量的类 */
class cond {
public:
	/* 创建并初始化条件变量 
	 * 这里条件变量的类不再添加锁类，实现流程中单独进行锁的加锁、释放锁操作*/
	cond(){
		if (pthread_cond_init(&m_cond, NULL) != 0)
			throw std::exception();
	}

	/* 销毁条件变量 */
	~cond(){
		pthread_cond_destroy(&m_cond);
	}

	/* 等待条件变量 */
	bool wait(pthread_mutex_t *m_mutex) {
        	int ret = 0;
        	//pthread_mutex_lock(&m_mutex);
        	ret = pthread_cond_wait(&m_cond, m_mutex);
        	//pthread_mutex_unlock(&m_mutex);
        	return ret == 0;
    	}
	
	/* 限时等待条件变量 */
    	bool timewait(pthread_mutex_t *m_mutex, struct timespec t){
       		int ret = 0;
        	//pthread_mutex_lock(&m_mutex);
        	ret = pthread_cond_timedwait(&m_cond, m_mutex, &t);
        	//pthread_mutex_unlock(&m_mutex);
        	return ret == 0;
    	}

	/* 唤醒等待条件变量的线程 */
	bool signal(){
		return pthread_cond_signal(&m_cond) == 0;
	}
	bool broadcast(){
		return pthread_cond_broadcast(&m_cond) == 0;
	}

private:
	pthread_cond_t m_cond;
};

#endif
