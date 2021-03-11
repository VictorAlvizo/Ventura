#pragma once
#include "Timer.h"

template <typename T>
class TimerSpecialized : public Timer {
public:
	TimerSpecialized()
		:Timer()
	{
		//holder
	}

	TimerSpecialized(int waitPeriod) 
		:Timer(waitPeriod)
	{
		//holder
	}

	~TimerSpecialized() {
		if (m_ContinueThread) {
			m_ContinueThread = false;
			m_TimerThread->join();
		}

		std::lock_guard<std::mutex> lock(m_TimerMutex); //Just to ensure the thread is not deleted while mutex is busy
		delete m_TimerThread;
		m_TimerThread = nullptr;
	}

	void StartTimer(const std::function<T()>& func) {
		if (!m_ContinueThread) {
			m_ContinueThread = true;
			m_TimerThread = new std::thread(&TimerSpecialized::TimerThread, this, func);
		}
	}

	void StopTimer() {
		if (m_ContinueThread) {
			m_ContinueThread = false;
			m_TimerThread->detach();
			delete m_TimerThread;
			m_TimerThread = nullptr;
		}
	}

	inline T getData() const { return m_ReturnData; }
	inline bool isTimerRunning() const { return m_ContinueThread; }

private:

	void TimerThread(const std::function<T()>& func) {
		while (m_ContinueThread) {
			{
				std::lock_guard<std::mutex> lock(m_TimerMutex);
				std::this_thread::sleep_for(std::chrono::milliseconds(m_WaitPeriod));
			}

			if (!m_ContinueThread) {
				break;
			}

			m_ReturnData = func();
		}
	}

	T m_ReturnData{};
};