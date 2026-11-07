#pragma once
#include "Burger.h"

class BurgerCompetitor:public BaseObject
{
private:
	std::string m_name;				//ライバルの名前
	int m_modelHandle = -1;			//バーガーモデル
	int m_score = 0;				//スコア
	float m_progress = 0.0f;		//完食率
	float m_biteInterval = 0.5f;	//一口の間隔
	float m_biteTimer = 0.0f;			//タイマー

	VECTOR m_rotation = VGet(0.0f, 0.0f, 0.0f);//モデルの回転角

	Burger m_burger;		//ライバル専用のバーガーオブジェクト
	VECTOR m_position;		//ライバルの配置位置

public:
	BurgerCompetitor() = default;
	~BurgerCompetitor() = default;

	void Initialize(const std::string& name, VECTOR position, float biteSpeed);
	void Update()override;
	void Draw()override;

	bool IsFinished() const { return m_progress >= 1.0f;}
	float GetProgress() const { return m_progress; }
	int GetScore() const { return m_score;}
	const std::string& GetName() const { return m_name; }
};