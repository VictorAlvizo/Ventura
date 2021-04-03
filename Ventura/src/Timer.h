#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>

class Timer {
public:
	//Will have a default wait period of 1 second
	Timer();
	//Give a wait period in miliseconds (1000 miliseconds = 1 second)
	Timer(int waitPeriod);
	~Timer();

	//Start the timer with a method wanted (must be void with no parameters)
	virtual void StartTimer(const std::function<void()>& func);

	//Stop the timer
	void StopTimer();
	//Change the wait period of the timer, must be in miliseconds (1000 miliseconds = 1 second)
	void ChangeWaitPeriod(int newPeriod);

	//Returns true if the timer is currently running
	inline bool isTimerRunning() const { return m_ContinueThread; }

protected:
	virtual void TimerThread(const std::function<void()>& func);

	std::thread * m_TimerThread;
	std::mutex m_TimerMutex;

	int m_WaitPeriod;
	bool m_ContinueThread;
};