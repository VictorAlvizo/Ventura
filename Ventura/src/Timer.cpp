#include "Timer.h"

Timer::Timer()
	:m_WaitPeriod(1000)
{
	m_TimerThread = nullptr;
	m_ContinueThread = false;
}

Timer::Timer(int waitPeriod) 
	:m_WaitPeriod(waitPeriod)
{
	m_TimerThread = nullptr;
	m_ContinueThread = false;
}

Timer::~Timer() {
	if (m_ContinueThread) {
		m_ContinueThread = false;
		m_TimerThread->join();
	}

	std::lock_guard<std::mutex> lock(m_TimerMutex); //Just to ensure the thread is not deleted while mutex is busy
	delete m_TimerThread;
	m_TimerThread = nullptr;
}

void Timer::StartTimer(const std::function<void()>& func) {
	//Current running thread must be stopped before another one can go
	if (!m_ContinueThread) {
		m_ContinueThread = true;
		m_TimerThread = new std::thread(&Timer::TimerThread, this, func);
	}
}

void Timer::StopTimer() {
	if (m_ContinueThread) {
		m_ContinueThread = false;
		m_TimerThread->detach();
		delete m_TimerThread;
		m_TimerThread = nullptr;
	}
}

void Timer::ChangeWaitPeriod(int newPeriod) {
	std::lock_guard<std::mutex> lock(m_TimerMutex);
	if (newPeriod >= 0) {
		m_WaitPeriod = newPeriod;
	}
	else {
		std::cout << "Error: The wait period cannot be a negative number" << std::endl;
	}
}

void Timer::TimerThread(const std::function<void()>& func) {
	while (m_ContinueThread) {
		{ //Don't want to lock the thread out, encapsulate it
			std::lock_guard<std::mutex> lock(m_TimerMutex);
			std::this_thread::sleep_for(std::chrono::milliseconds(m_WaitPeriod));
		}

		if (!m_ContinueThread) {
			break;
		}

		func();
	}
}