#include <wcgeJobSystem.hpp>
#include <wcgeThreadSafeRingBuffer.hpp>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>

namespace wcge {
	namespace js {
		uint32_t numThreads = 0;    // number of worker threads, it will be initialized in the Initialize() function
		containers::ThreadSafeRingBuffer<std::function<void()>, 256> jobPool;    // a thread safe queue to put pending jobs onto the end (with a capacity of 256 jobs). A worker thread can grab a job from the beginning
		std::condition_variable wakeCondition;    // used in conjunction with the wakeMutex below. Worker threads just sleep when there is no job, and the main thread can wake them up
		std::mutex wakeMutex;    // used in conjunction with the wakeCondition above
		uint64_t currentLabel = 0;    // tracks the state of execution of the main thread
		std::atomic<uint64_t> finishedLabel;    // track the state of execution across background worker threads
		std::atomic<bool> g_bIsRunning;    // a flag to indicate if the system is running or not
		std::vector<std::thread> g_threads;    // a vector to hold the worker threads
		
		void Worker();
		void Poll();
		bool IsBusy();
	}
}

void wcge::js::Worker() {
	std::function<void()> job;

	while (g_bIsRunning)
	{
		if (jobPool.PopFront(job)) // try to grab a job from the jobPool queue
		{
			// It found a job, execute it:
			job(); // execute job
			finishedLabel.fetch_add(1); // update worker label state
		}
		else
		{
			// no job, put thread to sleep
			std::unique_lock<std::mutex> lock(wakeMutex);
			wakeCondition.wait(lock, []() {return !g_bIsRunning || !jobPool.IsEmpty(); });
		}
	}
}

void wcge::js::Poll() {
	wakeCondition.notify_one(); // wake one worker thread
	std::this_thread::yield(); // allow this thread to be rescheduled
}

bool wcge::js::IsBusy()
{
	// Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
	return finishedLabel.load() < currentLabel;
}

void wcge::js::Init() {

	g_bIsRunning.store(true);

	// Initialize the worker execution state to 0:
	finishedLabel.store(0);

	// Retrieve the number of hardware threads in this system:
	auto numCores = std::thread::hardware_concurrency();

	// Calculate the actual number of worker threads we want:
	numThreads = std::max(1u, numCores);

	// Create the worker threads:
	for (uint32_t i = 0; i < numThreads; ++i)
	{
		g_threads.push_back(std::thread(Worker));
	}

}

void wcge::js::DeInit() {
	g_bIsRunning.store(false);

	wakeCondition.notify_all();

	for (std::thread& thread : g_threads) {
		if ( thread.joinable() ) thread.join();
	}
}

void wcge::js::Execute(const std::function<void()>& job) {

	// The main thread label state is updated:
	currentLabel += 1;

	// Try to push a new job until it is pushed successfully:
	while (!jobPool.PushBack(job)) { Poll(); }

	wakeCondition.notify_one(); // wake one thread
}

void wcge::js::Dispatch(const size_t numGroups, const size_t groupSize, const std::function<void(size_t, size_t)>& job) {
	for (size_t i = 0; i < numGroups; ++i) {
		Execute([i, groupSize, job]() {
				for (size_t j = 0; j < groupSize; ++j) {
					job(i, j);
				}
			});
	}
}

void wcge::js::Wait() {
	while (IsBusy()) { Poll(); }
}
