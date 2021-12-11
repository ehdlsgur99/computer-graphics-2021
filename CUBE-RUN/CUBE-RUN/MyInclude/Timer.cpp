#include "Timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency(&m_liCountTime);
	QueryPerformanceCounter(&m_liCurTime);
	QueryPerformanceCounter(&m_liPrevTime);

	updateDeltaTime();
}

void Timer::resetTimer()
{
	QueryPerformanceFrequency(&m_liCountTime);
	QueryPerformanceCounter(&m_liCurTime);
	QueryPerformanceCounter(&m_liPrevTime);

	updateDeltaTime();
}

void Timer::updateDeltaTime()
{
	QueryPerformanceCounter(&m_liCurTime);

	m_fDeltaTime = (m_liCurTime.QuadPart - m_liPrevTime.QuadPart) / float(m_liCountTime.QuadPart);

	m_liPrevTime = m_liCurTime;
}

float Timer::getDeltaTime()
{
	return m_fDeltaTime;
}