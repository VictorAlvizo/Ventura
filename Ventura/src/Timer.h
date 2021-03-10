#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>

class Timer {
public:
	Timer();
	Timer(int waitPeriod);
	~Timer();

	virtual void StartTimer(const std::function<void()>& func);

	void StopTimer();
	void ChangeWaitPeriod(int newPeriod);

	inline bool isTimerRunning() const { return m_ContinueThread; }

protected:
	virtual void TimerThread(const std::function<void()>& func);

	std::thread * m_TimerThread;
	std::mutex m_TimerMutex;

	int m_WaitPeriod;
	bool m_ContinueThread;
};