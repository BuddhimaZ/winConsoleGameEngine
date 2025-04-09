#include <wcgeThreadPool.hpp>
#include <algorithm>

wcge::mt::ThreadPool::ThreadPool() {
	// Get the number of threads supported by the hardware
	m_nNumThreads = std::thread::hardware_concurrency();
	m_nNumThreads = std::max<size_t>(m_nNumThreads, 1);

	// Create the worker threads
	for (size_t i = 0; i < m_nNumThreads; ++i)
	{
		m_Threads.push_back(std::thread(&ThreadPool::WorkerThread, this));
	}

	// Set the running state to true
	m_bIsRunning.store(true);

	// Set the finished label to 0
	m_nFinishedLabel.store(0);

	// Set the current label to 0
	m_nCurrentLabel = 0;
}

wcge::mt::ThreadPool::~ThreadPool() {
	// Set the running state to false
	m_bIsRunning.store(false);

	// Notify all threads to wake up
	m_WakeCondition.notify_all();

	// Join all threads
	for (std::thread& thread : m_Threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

void wcge::mt::ThreadPool::Execute(const std::function<void()>& job) {
	// Update the current label state
	m_nCurrentLabel += 1;

	// Try to push a new job until it is pushed successfully
	while (!m_Jobs.PushBack(job))
	{
		Poll();
	}

	// Wake one thread
	m_WakeCondition.notify_one();
}

void wcge::mt::ThreadPool::Dispatch(const size_t jobCount, const std::function<void(size_t)>& job) {
	for (size_t i = 0; i < jobCount; ++i) {
		Execute([i, job]() {
			job(i);
			});
	}
}

void wcge::mt::ThreadPool::Wait() {
	while (IsBusy()) {
		Poll();
	}
}

bool wcge::mt::ThreadPool::IsBusy()
{
	// Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
	return m_nFinishedLabel.load() < m_nCurrentLabel;
}

void wcge::mt::ThreadPool::WorkerThread() {
	std::function<void()> job;

	while (m_bIsRunning.load())
	{
		if (m_Jobs.PopFront(job)) // try to grab a job from the jobPool queue
		{
			// It found a job, execute it:
			job(); // execute job
			m_nFinishedLabel.fetch_add(1); // update worker label state
		}
		else
		{
			// no job, put thread to sleep
			std::unique_lock<std::mutex> lock(m_WakeMutex);
			m_WakeCondition.wait(lock, [this]() {return !m_bIsRunning.load() || !m_Jobs.IsEmpty(); });
		}
	}
}

void wcge::mt::ThreadPool::Poll() {
	m_WakeCondition.notify_one(); // wake one worker thread
	std::this_thread::yield(); // allow this thread to be rescheduled
}
