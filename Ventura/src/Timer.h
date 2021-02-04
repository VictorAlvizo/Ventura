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

	void StartTimer(const std::function<void(int)>& func); //TODO: Template this?
	void StopTimer();
	void ChangeWaitPeriod(int newPeriod);

private:
	//TODO: Don't allow copying?
	void TimerThread(const std::function<void(int)>& func);

	std::thread * m_TimerThread;
	std::mutex m_TimerMutex;

	int m_WaitPeriod;
	bool m_ContinueThread;
};