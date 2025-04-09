#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <vector>
#include "wcgeThreadSafeRingBuffer.hpp"

namespace wcge {
	namespace mt {
		class ThreadPool {
		public:
			ThreadPool();
			ThreadPool(const ThreadPool&) = delete;
			ThreadPool& operator=(const ThreadPool&) = delete;
			ThreadPool(ThreadPool&&) = delete;
			~ThreadPool();


			void Execute(const std::function<void()>& job);
			void Dispatch(const size_t jobCount, const std::function<void(size_t)>& job);
			void Wait();
			bool IsBusy();

		private:
			void WorkerThread();
			void Poll();

		private:
			containers::ThreadSafeRingBuffer<std::function<void()>, 256> m_Jobs;
			size_t m_nNumThreads;
			std::vector<std::thread> m_Threads;

			std::condition_variable m_WakeCondition;
			std::mutex m_WakeMutex;

			std::atomic<bool> m_bIsRunning;
			std::atomic<size_t> m_nFinishedLabel;
			size_t m_nCurrentLabel;
		};
	}
}