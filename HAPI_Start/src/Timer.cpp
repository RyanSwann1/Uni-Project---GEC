#include "Timer.h"

Timer::Timer(float expirationTime, bool active)
	: m_expirationTime(expirationTime),
	m_elaspedTime(0.0f),
	m_active(active)
{}

bool Timer::isExpired() const
{
	return m_elaspedTime >= m_expirationTime;
}

void Timer::deactive()
{
	m_active = false;
}

void Timer::activate()
{
	m_active = true;
}

void Timer::update(float deltaTime)
{
	if (m_active)
	{
		m_elaspedTime += deltaTime;
	}
}

void Timer::reset()
{
	m_elaspedTime = 0;
}