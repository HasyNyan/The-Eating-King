#include "GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

void GameScene::Initialize()
{
	m_score = 0;

	//カメラの生成と初期化
	m_camera = std::make_unique<Camera>();
	m_camera->Initialize(VGet(0.0f, 15.0f, -25.0f), VGet(0.0f, 0.0f, 0.0f));
}

void GameScene::Finalize()
{

}

void GameScene::Update()
{
	if (m_camera)
	{
		m_camera->Update();
	}
	//Zキーを押したら食べたことにしてスコア加算
	if (PushHitKey(KEY_INPUT_Z))
	{
		m_score += 100;
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

	//テスト用のボックス
	DrawCube3D(VGet(-2.0f, -2.0f, -2.0f), VGet(2.0f, 2.0f, 2.0f),GetColor(255,100,100),GetColor(200,50,50),TRUE);


	DrawString(200, 100, "----- GAME SCENE -----", COLOR_AQUA);
	DrawFormatString(200, 160, COLOR_WHITE, "Burger Score: %d", m_score);

	DrawString(200, 240, "[Z] Key : Eat Burger (+100)", COLOR_WHITE);
	DrawString(200, 280, "[ENTER] Key : Back to Title", COLOR_WHITE);
}