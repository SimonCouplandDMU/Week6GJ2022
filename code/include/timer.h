/* \file timer.h */
#pragma once
#include <chrono>

class Timer
{
public:
	Timer(); //!< Constructor
	float reset(); //!< Resets timer and gives back time elapsed in seconds
	uint32_t resetMs(); // Resets the time and gives back time elapsed in milliseconds
private:
	std::chrono::high_resolution_clock::time_point m_start; //!< Start time point
	std::chrono::high_resolution_clock::time_point m_end; //!< End time point
	std::chrono::duration<float> m_durationSeconds; //!< Float duration
	std::chrono::milliseconds m_durationMilliseconds; //!< Uint32_t duration
};
