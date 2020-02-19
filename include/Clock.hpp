#pragma once

#include <ctime>

class Clock
{
public:
	Clock() = default;

	void start();
	void stop();
	std::time_t getElapsedTime() const;
	std::time_t getCurrentTime() const;

private:
	std::time_t m_startTime;
	std::time_t m_stopTime;
};