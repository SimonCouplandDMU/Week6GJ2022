/* \file timer.cpp */
#include "timer.h"

Timer::Timer()
{
	m_start = std::chrono::high_resolution_clock::now();
}

float Timer::reset()
{
	m_end = std::chrono::high_resolution_clock::now();
	m_durationSeconds = m_end - m_start;
	m_start = std::chrono::high_resolution_clock::now();
	return m_durationSeconds.count();
}

uint32_t Timer::resetMs()
{
	m_end = std::chrono::high_resolution_clock::now();
	m_durationSeconds = m_end - m_start;
	m_durationMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(m_durationSeconds);
	m_start = std::chrono::high_resolution_clock::now();
	return m_durationMilliseconds.count();;
}