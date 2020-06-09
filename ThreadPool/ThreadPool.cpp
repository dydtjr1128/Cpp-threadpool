#include "ThreadPool.h"

#include <iostream>

namespace thread::threadpool { // Overlapping namespace in C++17
	ThreadPool::ThreadPool(size_t threadCount)
		: threadCount_(threadCount),
		stop_all_(false),
		jobCount_(0) {
		worker_threads_.reserve(threadCount);
		for (auto i = 0; i < threadCount_; ++i) {
			worker_threads_.emplace_back([this]() {this->WorkerThread(); });
		}
	}

	ThreadPool::~ThreadPool() {
		stop_all_ = true;
		cv_.notify_all();
		for (auto& thread : worker_threads_) {
			thread.join();
		}
	}

	void ThreadPool::EncuqueJob(std::function<void()> func) {
		{
			std::lock_guard<std::mutex> lock_guard(mutex_);
			jobs_.push(std::move(func));
		}
		cv_.notify_one();
	}

	void ThreadPool::shutDown() {
		stop_all_ = true;
		cv_.notify_all();
	}

	int ThreadPool::getAllThreadCount() {
		return worker_threads_.size();
	}

	int ThreadPool::getRunningJobCount() {
		return jobCount_.load();
	}

	void ThreadPool::WorkerThread() {
		while (true) {
			std::unique_lock<std::mutex> lock(mutex_); // while문의 thread-safe를 보장하는 mutex 락
			std::cout << "대기 시작" << std::endl;
			this->cv_.wait(lock, [this]() {return !this->jobs_.empty() || this->stop_all_; });
			std::cout << "대기 종료" << std::endl;

			if (this->stop_all_ && this->jobs_.empty()) {
				--jobCount_;
				break;
			}

			auto job = std::move(jobs_.front());
			jobs_.pop();
			lock.unlock();

			++jobCount_;

			job();
		}
	}
}

