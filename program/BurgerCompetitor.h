#pragma once
#include "Burger.h"

class BurgerCompetitor:public BaseObject
{
private:
	std::string m_name;				//ライバルの名前
	int m_modelHandle = -1;			//バーガーモデル
	int m_score = 0;				//スコア
	int m_eatenCount = 0;			//完食したハンバーガーの個数
	float m_progress = 0.0f;		//完食率
	float m_biteInterval = 0.5f;	//一口の間隔
	float m_biteTimer = 0.0f;		//タイマー
	float m_deltaTime = 0.0f;		//経過時間

	VECTOR m_rotation = VGet(0.0f, 0.0f, 0.0f);//モデルの回転角

	//食べる進行とモデル縮小の更新
	void UpdateEating();

public:
	BurgerCompetitor() = default;
	~BurgerCompetitor() override;

	void Initialize() override;
	void Update()override;
	void Draw()override;

	//ライバルAI固有の設定を行う関数
	void Setup(const std::string& name, VECTOR position, float biteInterval);
	void SetDeltaTime(float dt) { m_deltaTime = dt;}

	int GetEatenCount() const { return m_eatenCount; }
	float GetProgress() const { return m_progress; }
	int GetScore() const { return m_score;}
	const std::string& GetName() const { return m_name; }
};