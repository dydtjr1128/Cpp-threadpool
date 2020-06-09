#include <iostream>
#include <sstream>
#include "ThreadPool.h"

namespace {
	namespace tp = thread::threadpool;
}

int main() {

	tp::ThreadPool threadPool(5);

	for (int i = 0; i < 10; i++) {
		threadPool.EncuqueJob([]() {
			for (int i = 0; i < 10; i++) {
				std::ostringstream os;
				os << "thread" << i << " id : " << std::this_thread::get_id() << " " << i << std::endl;
				std::cout << os.str();
				std::this_thread::sleep_for(std::chrono::duration<double>(2));
			}
			}
		);
	}

	std::ostringstream os;
	while (true) {
		os << "main thread id : " << std::this_thread::get_id() << " ThreadPool::getAllThreadCount() : " << threadPool.getAllThreadCount() << " ThreadPool::getRunningJobCount() : " << threadPool.getRunningJobCount() << std::endl;
		std::cout << os.str();
		os.clear();
		std::this_thread::sleep_for(std::chrono::duration<double>(2));
	}

}