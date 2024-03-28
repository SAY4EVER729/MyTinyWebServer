/* 使用循环数组来实现阻塞队列
 * 为了线程安全，每个操作前都需要加互斥锁，然后再解锁 */
#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "../lock/locker.h"

using namespace std;

template <class T>
class block_queue {
public:
	/* 循环队列构造函数，初始化参数 */
	block_queue(int max_size = 1000) {
		if(max_size <= 0)
			exit(-1);

		m_max_size = max_size;
		m_array = new T[max_size];
		m_size = 0;
		m_front = -1;
		m_back = -1;
	}

	/* 析构函数 */
	~block_queue(){
		m_mutex.lock();
		if (m_array != NULL)
				delete [] m_array;
		m_mutex.unlock();
	}

	/* 清空队列 */
	void clear() {
		m_mutex.lock();
		m_size = 0;
		m_back = -1;
		m_front = -1;
		m_mutex.unlock();
	}

	/* 判断队列是否满了 */
	bool full() {
        m_mutex.lock();
        if (m_size >= m_max_size) {
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }

	/* 判断队列是否为空 */
	bool empty() {
        m_mutex.lock();
        if (0 == m_size) {
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }

	/* 返回队首元素 */
/*	
	bool front(T &value) {
		m_mutex.lock();
		if (m_size == 0) {
			m_mutex.unlock();
			return false;
		}
		value = m_array[m_front];
		m_mutex.unlock();
		return true;
	}	
*/
	/* 返回队尾元素 */
	
	/* 返回数组大小 */
	int size(){
		int tmp = 0;

		m_mutex.lock();
		tmp = m_size;
		m_mutex.unlock();

		return tmp;
	}

	/* 返回最大容量 */
	int max_size() {
		int tmp = 0;

		m_mutex.lock();
		tmp = m_max_size;
		m_mutex.unlock();

		return tmp;
	}

	/* 队列添加元素  需注意：当前必须要有线程等待在条件变量上，否则内部的唤醒无意义*/
	bool push(const T &item) {
		m_mutex.lock();
		if (m_size >= m_max_size) {
			/* 生产者消费者模式，在 push 前，生产者生产产品，添加至队列中 */
			m_cond.broadcast(); // 循环队列已满，需要 唤醒所有阻塞在条件变量上的消费者（线程）
			m_mutex.unlock();
			return false;
		}

		m_back = (m_back + 1) % m_max_size;
		m_array[m_back] = item;

		m_size++;

		m_cond.broadcast();
		m_mutex.unlock();
		return true;
	}

	/* pop 如果队列没有元素，则消费者将会阻塞在条件变量上 */
	bool pop(T &item) {
		m_mutex.lock();
		while (m_size <= 0) {
			if (!m_cond.wait(m_mutex.get())) {
				m_mutex.unlock();
				return false;
			}
		}

		m_front = (m_front + 1) % m_max_size;
		item = m_array[m_front];
		m_size--;
		m_mutex.unlock();
		return true;
	}

	/* 超时处理的 pop */
	bool pop(T &item, int ms_timeout) {
		struct timespec t = {0, 0};
		struct timeval now = {0, 0};
		gettimeofday(&now, NULL);
		m_mutex.lock();
		if (m_size <= 0) {
			t.tv_sec = now.tv_sec + ms_timeout / 1000;
           	t.tv_nsec = (ms_timeout % 1000) * 1000;
			if (!m_cond.timewait(m_mutex.get(), t)) {
                m_mutex.unlock();
                return false;
            }
		}
		if (m_size <= 0){
            m_mutex.unlock();
            return false;
        }

		m_front = (m_front + 1) % m_max_size;
		item = m_array[m_front];
		m_size--;
		m_mutex.unlock();
		return true;
	}

private:
	locker m_mutex;
	cond m_cond;

	T *m_array;
	int m_size;
	int m_max_size;
	int m_front;
	int m_back;	
};

#endif
