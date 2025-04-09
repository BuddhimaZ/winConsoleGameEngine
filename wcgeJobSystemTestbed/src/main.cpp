#include <wcgeJobSystem.hpp>
#include <wcgeThreadPool.hpp>
#include <iostream>
#include <vector>
#include <chrono>

void ExpensiveFunction(std::shared_ptr<uint64_t> out) {
	uint64_t result = 0;
	for (uint64_t i = 0; i < 100000000; ++i) {
		result += i;
	}
	*out = result;
}

constexpr int numJobs = 7;

float TestMultithreading() {
	wcge::js::Init();

	std::vector<std::shared_ptr<uint64_t>> results;

	auto start = std::chrono::high_resolution_clock::now();

	// Create and submit jobs
	for (int i = 0; i < numJobs; ++i) {
		auto result = std::make_shared<uint64_t>(0);
		results.push_back(result);

		// Submit job
		wcge::js::Execute([result]() {
			ExpensiveFunction(result);
			});
	}

	// Wait for all jobs to finish
	wcge::js::Wait();

	auto end = std::chrono::high_resolution_clock::now();

	// Print the results
	for (int i = 0; i < numJobs; ++i) {
		std::cout << "Result of job " << i << ": " << *results[i] << std::endl;
	}

	wcge::js::DeInit();

	std::chrono::duration<float> duration = end - start;
	return duration.count();
}

float TestSinglethreading() {
	std::vector<std::shared_ptr<uint64_t>> results;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numJobs; ++i) {
		auto result = std::make_shared<uint64_t>(0);
		results.push_back(result);

		ExpensiveFunction(result);
	}

	auto end = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numJobs; ++i) {
		std::cout << "Result of job " << i << ": " << *results[i] << std::endl;
	}

	std::chrono::duration<float> duration = end - start;
	return duration.count();
}


int* pA = nullptr;
int* pB = nullptr;
int* pC = nullptr;

constexpr size_t numElements = 256000000;
constexpr size_t jobCount = 256;

float TestMultithreading2() {
	// Initialize the pA, pB, pC pointers
	pA = new int[numElements];
	pB = new int[numElements];
	pC = new int[numElements];

	for (size_t i = 0; i < numElements; i++) {
		pA[i] = i;
		pB[i] = i;
		pC[i] = 0;
	}

	wcge::mt::ThreadPool* pThreadPool = new wcge::mt::ThreadPool();

	//auto job = [](size_t groupNo, size_t jobNo) {
	//	size_t i = groupNo * groupSize + jobNo;
	//	pC[i] = pA[i] + pB[i];
	//};

	auto job = [](size_t jobNo) {
		const size_t start = jobNo * (numElements / jobCount);
		const size_t end = start + (numElements / jobCount);

		for (size_t i = start; i < end; i++)
			pC[i] = pA[i] + pB[i];
	};

	auto start = std::chrono::high_resolution_clock::now();

	pThreadPool->Dispatch(jobCount, job);
	pThreadPool->Wait();

	auto end = std::chrono::high_resolution_clock::now();

	delete pThreadPool;

	// Check the results
	for (size_t i = 0; i < numElements; i++) {
		if (pC[i] != (pA[i] + pB[i])) {
			std::cout << "Error: pC[" << i << "] = " << pC[i] << " != " << pA[i] << " + " << pB[i] << std::endl;
		}
	}

	delete[] pA;
	delete[] pB;
	delete[] pC;

	std::chrono::duration<float> duration = end - start;
	return duration.count();
}

float TestSinglethreading2() {
	// Initialize the pA, pB, pC pointers
	pA = new int[numElements];
	pB = new int[numElements];
	pC = new int[numElements];

	for (size_t i = 0; i < numElements; i++) {
		pA[i] = i;
		pB[i] = i;
		pC[i] = 0;
	}

	auto job = [](size_t jobNo) {
		const size_t start = jobNo * (numElements / jobCount);
		const size_t end = start + (numElements / jobCount);

		for (size_t i = start; i < end; i++)
			pC[i] = pA[i] + pB[i];
		};

	auto start = std::chrono::high_resolution_clock::now();


	for (size_t j = 0; j < jobCount; j++) {
		job(j);
	}


	auto end = std::chrono::high_resolution_clock::now();

	// Check the results
	for (size_t i = 0; i < numElements; i++) {
		if (pC[i] != (pA[i] + pB[i])) {
			std::cout << "Error: pC[" << i << "] = " << pC[i] << " != " << pA[i] << " + " << pB[i] << std::endl;
		}
	}

	delete[] pA;
	delete[] pB;
	delete[] pC;

	std::chrono::duration<float> duration = end - start;
	return duration.count();
}

int main() {
	float mtTime = TestMultithreading2();
	std::cout << "Multithreading time: " << mtTime << std::endl << std::endl;
	
	float stTime = TestSinglethreading2();
	std::cout << "Singlethreading time: " << stTime << std::endl;

	return 0;

}