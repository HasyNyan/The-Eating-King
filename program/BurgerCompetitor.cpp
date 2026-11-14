#include "BurgerCompetitor.h"

BurgerCompetitor::~BurgerCompetitor()
{
	if (m_modelHandle != -1)
	{
		MV1DeleteModel(m_modelHandle);
		m_modelHandle = -1;
	}
}

void BurgerCompetitor::Initialize()
{
	m_progress = 0.0f;
	m_score = 0;
	m_eatenCount = 0;
	m_biteTimer = 0.0f;
	m_position = VGet(0.0f, 0.0f, 0.0f);
}

void BurgerCompetitor::Setup(const std::string& name, VECTOR position, float biteInterval)
{
	m_name = name;
	m_position = position;
	m_biteInterval = biteInterval;

	if (m_modelHandle == -1)
	{
		// 3Dモデル読み込み
		m_modelHandle = MV1LoadModel("Assets/Models/hamburger/hambuger.mv1");
	}
	if (m_modelHandle != -1)
	{
		MV1SetPosition(m_modelHandle, m_position);
		MV1SetScale(m_modelHandle, VGet(0.1f, 0.1f, 0.1f));
	}
}

void BurgerCompetitor::Update()
{
	if (!m_isActive)return;

	//食事進行の更新
	UpdateEating();
	//Y軸回転させる
	m_rotation.y += 1.0f * m_deltaTime;
}

void BurgerCompetitor::UpdateEating()
{
	//タイマー更新
	m_biteTimer += m_deltaTime;

	//定期的に食べる
	if (m_biteTimer >= m_biteInterval)
	{
		m_biteTimer = 0.0f;
		m_progress += 0.05f;//一口で5%進む
		m_score += 100;
		//1個完食したらカウントを増やして次のバーガーを用意
		if (m_progress >= 1.0f)
		{
			m_eatenCount++;
			m_progress = 0.0f;
			m_score += 500;
		}

		//食べた進行度に応じてモデルのスケールを小さくしていく演出
		if (m_modelHandle != -1)
		{
			float currentScale = 0.1f * (1.0f - m_progress);
			MV1SetScale(m_modelHandle, VGet(currentScale, currentScale, currentScale));
		}
	}
}

void BurgerCompetitor::Draw()
{
	if (!m_isActive)return;
	if (m_modelHandle != -1)
	{
		MV1SetPosition(m_modelHandle, m_position);
		MV1SetRotationXYZ(m_modelHandle, m_rotation);
		MV1DrawModel(m_modelHandle);
	}
}

