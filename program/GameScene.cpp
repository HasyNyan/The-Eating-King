#include "GameScene.h"
#include"ResultScene.h"
#include "SceneManager.h"

void GameScene::Initialize()
{
	m_score = 0;
}

void GameScene::Finalize()
{

}

void GameScene::Update()
{
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
	DrawString(200, 100, "----- GAME SCENE -----", COLOR_AQUA);
	DrawFormatString(200, 160, COLOR_WHITE, "Burger Score: %d", m_score);

	DrawString(200, 240, "[Z] Key : Eat Burger (+100)", COLOR_WHITE);
	DrawString(200, 280, "[ENTER] Key : Back to Title", COLOR_WHITE);
}