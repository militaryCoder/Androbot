#include "Clock.hpp"

void Clock::start()
{
	m_startTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void Clock::stop()
{
	m_stopTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

std::time_t Clock::getElapsedTime() const
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - m_startTime;
}

std::time_t Clock::getCurrentTime() const
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
