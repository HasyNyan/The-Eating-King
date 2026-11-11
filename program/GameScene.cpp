#include "GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

void GameScene::Initialize()
{
	m_score = 0;

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

}

void GameScene::Finalize()
{

}

void GameScene::Update()
{
	if (m_camera)m_camera->Update();
	if (m_burger)m_burger->Update();
	//ライバルAIの更新
	for (auto& competitor : m_competitors)
	{
		if (competitor)competitor->Update();
	}
	//Zキーを押したら食べたことにしてスコア加算
	if (PushHitKey(KEY_INPUT_Z))
	{
		TryBiteBurgerByRay();
	}

	//ENTERキーを押したらタイトル画面に戻る
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		m_sceneManager.ChangeScene<ResultScene>(m_score);
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
	DrawString(200, 100, "----- GAME SCENE -----", COLOR_AQUA);
	DrawFormatString(200, 160, COLOR_WHITE, "Burger Score: %d", m_score);

	int yOffset = 80;
	for (const auto& competitor : m_competitors)
	{
		if (competitor)
		{
			DrawFormatString(20, yOffset, COLOR_YELLOW, "%s : %.0f%% (Score: %d)",
				competitor->GetName().c_str(),
				competitor->GetProgress() * 100.0f,
				competitor->GetScore()
			);
			yOffset += 25;
		}
	}

	DrawString(200, 240, "[Z] Key : Eat Burger (+100)", COLOR_WHITE);
	DrawString(200, 280, "[ENTER] Key : Back to Title", COLOR_WHITE);
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
	}
}