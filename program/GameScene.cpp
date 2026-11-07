#include "GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

void GameScene::Initialize()
{
	m_score = 0;

	//カメラの生成と初期化
	m_camera = std::make_unique<Camera>();
	m_camera->Initialize(VGet(0.0f, 15.0f, -25.0f), VGet(0.0f, 0.0f, 0.0f));

	//ハンバーガー生成
	m_burger = std::make_unique<Burger>();
	m_burger -> Initialize();
}

void GameScene::Finalize()
{

}

void GameScene::Update()
{
	if (m_camera)m_camera->Update();
	if (m_burger)m_burger->Update();
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

	DrawString(200, 100, "----- GAME SCENE -----", COLOR_AQUA);
	DrawFormatString(200, 160, COLOR_WHITE, "Burger Score: %d", m_score);

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