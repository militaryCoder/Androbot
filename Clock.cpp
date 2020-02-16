#include "Clock.hpp"

void Clock::start()
{
	m_startTime = time(NULL);
}

void Clock::stop()
{
	m_stopTime = time(NULL);
}

std::time_t Clock::getElapsedTime() const
{
	return time(NULL) - m_startTime;
}

std::time_t Clock::getCurrentTime() const
{
	return time(NULL);
}
