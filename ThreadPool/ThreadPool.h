#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
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

		void EncuqueJob(std::function<void()> func);

		void shutDown();
		int getAllThreadCount();
		int getRunningJobCount();
	private:
		void WorkerThread();

		std::queue<std::function<void()>> jobs_; // job array
		std::vector<std::thread> worker_threads_; // thread array
		std::mutex mutex_;
		std::condition_variable cv_;

		size_t threadCount_;
		bool stop_all_;
		std::atomic<int> jobCount_;
	};
}

#endif // THREADPOOL_H