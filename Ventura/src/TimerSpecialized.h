#pragma once
#include "Timer.h"

template <typename T, typename N>
class TimerSpecialized : public Timer {
public:
	//Default constructer with a wait time of 1 second
	TimerSpecialized()
		:Timer()
	{
		//holder
	}

	//Give a wait period in miliseconds (1000 miliseconds = 1 second)
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

		if (m_TimerThread != nullptr) {
			std::lock_guard<std::mutex> lock(m_TimerMutex); //Just to ensure the thread is not deleted while mutex is busy
			delete m_TimerThread;
			m_TimerThread = nullptr;
		}
	}

	//Start the timer with a method wanted
	void StartTimer(const std::function<T(N)>& func, N value) {
		if (!m_ContinueThread) {
			m_PassVal = value;
			m_ContinueThread = true;
			m_TimerThread = new std::thread(&TimerSpecialized::TimerThread, this, func);
		}
	}

	//Stop the timer
	void StopTimer() {
		if (m_ContinueThread) {
			m_ContinueThread = false;
			m_TimerThread->detach();
			delete m_TimerThread;
			m_TimerThread = nullptr;
		}
	}

	//Change the value that's being passed into the function
	void ChangeValue(N newValue) {
		std::lock_guard<std::mutex> lock(m_ChangeValMutex);
		m_PassVal = newValue;
	}

	//Retrives the data of the method called
	inline T getData() const { return m_ReturnData; }
	//Retrives the value currenty being passed into the function
	inline N getValue() const { return m_PassVal; }
	//Returns true if the timer is running
	inline bool isTimerRunning() const { return m_ContinueThread; }

private:
	void TimerThread(const std::function<T(N)>& func) {
		while (m_ContinueThread) {
			{
				std::lock_guard<std::mutex> lock(m_TimerMutex);
				std::this_thread::sleep_for(std::chrono::milliseconds(m_WaitPeriod));
			}

			if (!m_ContinueThread) {
				break;
			}

			{
				std::lock_guard<std::mutex> lock(m_ChangeValMutex);
				m_ReturnData = func(m_PassVal);
			}
		}
	}

	T m_ReturnData{};
	N m_PassVal{};

	std::mutex m_ChangeValMutex; //Mutex for when changing the value being passed
};