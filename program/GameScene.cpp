#include "GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

void GameScene::Initialize()
{
	m_score = 0;
	//現在のシステム時間を取得
	m_prevTime = GetNowHiPerformanceCount();

	//カメラの生成と初期化
	m_camera = std::make_unique<Camera>();
	m_camera->Initialize(VGet(0.0f, 18.0f, -30.0f), VGet(0.0f, 0.0f, 0.0f));

	//ハンバーガー生成
	m_burger = std::make_unique<Burger>();
	m_burger -> Initialize();

	//ライバルAIを2体生成
	m_competitors.clear();

	//CPU1
	auto cpu1 = std::make_unique<BurgerCompetitor>();
	cpu1->Initialize();//基底クラスの初期化
	cpu1->Setup("CPU-1(John)", VGet(-12.0f, 0.0f, 0.0f), 0.35f);
	m_competitors.push_back(std::move(cpu1));

	//CPU2
	auto cpu2 = std::make_unique<BurgerCompetitor>();
	cpu2->Initialize();//基底クラスの初期化
	cpu2->Setup("CPU-2(Mike)", VGet(12.0f, 0.0f, 0.0f), 0.50f);
	m_competitors.push_back(std::move(cpu2));


	//60秒タイマーを開始
	m_gameTimer = std::make_unique<GameTimer>();
	m_gameTimer->Start(60.0f);

}

void GameScene::Finalize()
{

}

void GameScene::Update()
{
	//実経過時間の算出
	float deltaTime = CalculateDeltaTime();
	//タイマー更新
	if (UpdateTimer(deltaTime))return;

	if (m_camera)m_camera->Update();
	if (m_burger)m_burger->Update();
	UpdateCompetitors(deltaTime);

	//プレイヤーの入出力
	HandleInput();
}

float GameScene::CalculateDeltaTime()
{
	//現在のシステム時間を取得し、前フレームとの差分を秒単位に変換
	LONGLONG currentTime = GetNowHiPerformanceCount();
	float deltaTime = static_cast<float>(currentTime - m_prevTime) / 1000000.0f;
	m_prevTime = currentTime;
	//処理落ち・フリーズ対策
	if (deltaTime > 0.1f)deltaTime = 0.1f;
	return deltaTime;
}

bool GameScene::UpdateTimer(float deltaTime)
{
	if (!m_gameTimer)return false;
	m_gameTimer->Update(deltaTime);
	if (m_gameTimer->IsTimeUp())
	{
		m_sceneManager.ChangeScene<ResultScene>(m_score);
		return true;
	}
	return false;
}

void GameScene::UpdateCompetitors(float deltaTime)
{
	for (auto& competitor : m_competitors)
	{
		if (competitor)
		{
			competitor->SetDeltaTime(deltaTime);
			competitor->Update();
		}
	}
}

void GameScene::HandleInput()
{
	if (PushHitKey(KEY_INPUT_Z))
	{
		TryBiteBurgerByRay();
	}
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		m_sceneManager.ChangeScene<ResultScene>(m_score);
	}
}

void GameScene::TryBiteBurgerByRay()
{
	if (!m_camera || !m_burger)return;

	//カメラ位置から注視点へのレイ(方向)を算出
	VECTOR camPos = m_camera->GetPosition();
	VECTOR camTarget = m_camera->GetTarget();
	VECTOR rayDir = VSub(camTarget, camPos);//カメラから注視点への方向

	//半径2の範囲でレイ噛みを実施
	int erasedCount = m_burger->BiteByRay(camPos, rayDir, 4);
	if (erasedCount > 0)
	{
		m_score += erasedCount * 10;//削れたボクセル数に応じたスコア加算
		if (m_burger->IsEmpty())
		{
			m_playerEatenCount++;
			m_score += 500;
			m_burger->Reset();
		}
	}
}
void GameScene::Draw()
{
	//カメラの設定を適用
	if (m_camera)
	{
		m_camera->SetToDxLib();
	}
	//バーガーモデルの描画
	if (m_burger)m_burger->Draw();
	//ライバルAIの更新
	for(auto& competitor : m_competitors)
	{
		if (competitor)competitor->Draw();
	}

	//UI表示
	DrawString(20, 20, "----- EATING CONTEST -----", COLOR_AQUA);
	DrawTimerUI();
	DrawFormatString(20, 80, COLOR_WHITE, "Player : %d Burgers (Score: %d)", m_playerEatenCount,m_score);

	int yOffset = 110;
	for (const auto& competitor : m_competitors)
	{
		if (competitor)
		{
			DrawFormatString(20, yOffset, COLOR_YELLOW, "%s : %.0f%% (Score: %d)",
				competitor->GetName().c_str(),
				competitor->GetEatenCount(),
				competitor->GetProgress() * 100.0f,
				competitor->GetScore()
			);
			yOffset += 25;
		}
	}

	DrawString(20, yOffset + 20, "[Z] Key : Bite Burger", COLOR_WHITE);
	DrawString(20, yOffset + 40, "Left Drag : Rotate Burger", COLOR_WHITE);
	DrawString(20, yOffset + 60, "[ENTER] Key : Finish", COLOR_YELLOW);
}

void GameScene::DrawTimerUI() const
{
	if (!m_gameTimer)return;
	DrawFormatString(20, 50, COLOR_YELLOW, "TIME LEFT : %2d sec", m_gameTimer->GetRemainingSeconds());
}
