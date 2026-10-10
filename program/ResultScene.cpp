#include"ResultScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
ResultScene::ResultScene(SceneManager& manager, int score)
	:BaseScene(manager),m_finalScore(score)
{

}

void ResultScene::Initialize()
{

}


void ResultScene::Finalize()
{

}

void ResultScene::Update()
{
	//スペースキーまたはENTERキーでタイトル画面へ遷移
	if (PushHitKey(KEY_INPUT_SPACE) || PushHitKey(KEY_INPUT_RETURN))
	{
		m_sceneManager.ChangeScene<TitleScene>();
	}
}

void ResultScene::Draw()
{
	DrawString(260, 120, "===RESULT===", COLOR_YELLOW);
	//GameSceneから受け取った最終スコアの表示
	DrawFormatString(240, 200, COLOR_WHITE, "Final Score : %d pts", m_finalScore);

	DrawString(200, 300, "PRESS SPACE / ENTER TO TITLE", COLOR_AQUA);
}