// 本文件用于测试 locker.h 头文件

#include <iostream>
#include "locker.h"

int main(){
	
	sem sem1(3);
	sem sem2;
	sem2.wait();
	sem2.post();

	locker lo1;
	lo1.lock();
	lo1.unlock();

	cond con1;
	con1.signal();
	std::cout << "stop" << std::endl;
	return 0;
}
