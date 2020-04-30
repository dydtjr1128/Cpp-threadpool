#include "ThreadPool.h"

namespace thread::threadpool {
	ThreadPool::ThreadPool(size_t threadCount)
		: threadCount_(threadCount),
		stop_all(false) {
		worker_threads_.reserve(threadCount);
		for (auto i = 0; i < threadCount_; i++) {
			worker_threads_.emplace_back([this]() {this->WorkerThread(); });
		}
	}

	ThreadPool::~ThreadPool() {
		for (auto& thread : worker_threads_) {
			thread.join();
		}
	}

	void ThreadPool::Job() {

	}

	void ThreadPool::WorkerThread() {
		while (true) {
			std::unique_lock lock(mutex_);
			this->cv.wait(lock, [this]() {return !this->jobs.empty() || this->stop_all; });

			auto job = jobs.front();
			jobs.pop(


		}

	}
}

