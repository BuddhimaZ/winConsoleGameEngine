#pragma once

#include <mutex>

namespace wcge {
	namespace containers {
		template <typename T, size_t capacity>
		class ThreadSafeRingBuffer {
		public:
			ThreadSafeRingBuffer() : m_nHead(0), m_nTail(0), m_nCount(0) {
				m_pData = new T[capacity];
			}

			ThreadSafeRingBuffer(const ThreadSafeRingBuffer&) = delete;
			ThreadSafeRingBuffer& operator=(const ThreadSafeRingBuffer&) = delete;
			ThreadSafeRingBuffer(ThreadSafeRingBuffer&&) = delete;

			~ThreadSafeRingBuffer() {
				delete[] m_pData;
			}

			bool PushBack(const T& value) {

				bool result = false;

				m_mutex.lock();

				if (m_nCount < capacity) {
					m_pData[m_nTail] = value;
					m_nTail = (m_nTail + 1) % capacity;
					++m_nCount;
					result = true;
				}

				m_mutex.unlock();

				return result;
			}

			bool PopFront(T& value) {

				bool result = false;

				m_mutex.lock();

				if (m_nCount > 0) {
					value = m_pData[m_nHead];
					m_nHead = (m_nHead + 1) % capacity;
					--m_nCount;
					result = true;
				}

				m_mutex.unlock();

				return result;
			}

			bool IsEmpty() {
				bool result = false;

				m_mutex.lock();

				result = m_nCount == 0;

				m_mutex.unlock();

				return result;
			}

			bool IsFull() {
				bool result = false;

				m_mutex.lock();

				result = m_nCount == capacity;

				m_mutex.unlock();

				return result;
			}
		
		private:
			T* m_pData;
			size_t m_nHead;
			size_t m_nTail;
			size_t m_nCount;
			std::mutex m_mutex;
		};
	}
}