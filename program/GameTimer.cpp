#include "GameTimer.h"

void GameTimer::Start(float limitTimeSeconds)
{
	m_limitTime = limitTimeSeconds;
	m_timeCount = limitTimeSeconds;
	m_isTimeUp  = false;
	m_isRunning = true;
}

void GameTimer::Update(float deltaTime)
{
	if (!m_isRunning || m_isTimeUp) return;

	m_timeCount -= deltaTime;

	if (m_timeCount <= 0.0f)
	{
		m_timeCount = 0.0f;
		m_isTimeUp = true;
		m_isRunning = false;
	}
}