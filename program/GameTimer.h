#pragma once
class GameTimer
{
private:
	float m_limitTime = 0.0f;
	float m_timeCount = 0.0f;
	bool  m_isTimeUp  = false;
	bool  m_isRunning = false;
public:
	GameTimer()  = default;
	~GameTimer() = default;

	//タイマーの開始
	void Start(float limitTimeSeconds);
	void Update(float deltaTime);
	//停止・再開制御
	void Stop()   { m_isRunning = false; }
	void Resume() { m_isRunning = true; }

	//状態・数値の取得
	bool	IsTimeUp()				const { return m_isTimeUp; }
	float	GetRemainingTime()		const { return m_timeCount; }
	int		GetRemainingSeconds()	const { return static_cast<int>(m_timeCount);}
};