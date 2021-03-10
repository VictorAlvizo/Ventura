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

	void StartTimer(const std::function<T()>& func) {
		if (!m_ContinueThread) {
			m_ContinueThread = true;
			m_TimerThread = new std::thread(&TimerSpecialized::TimerThread, this, func);
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