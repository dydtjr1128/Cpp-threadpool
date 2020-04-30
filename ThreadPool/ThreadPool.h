#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <mutex>
#include <functional>
#include <thread>
#include <queue>
#include <vector>

namespace thread::threadpool {
	class ThreadPool {
	public:
		explicit ThreadPool(size_t threadCount);
		~ThreadPool();

	private:
		void WorkerThread();
		void Job();

		std::queue<std::function<void()>> jobs_;
		std::vector<std::thread> worker_threads_;
		std::mutex mutex_;
		std::condition_variable cv_;

		size_t threadCount_;
		bool stop_all;
	};
}

#endif // THREADPOOL_H