#include"TitleScene.h"
#include"GameScene.h"
#include"SceneManager.h"

void TitleScene::Initialize()
{

}

void TitleScene::Finalize()
{

}
void TitleScene::Update()
{
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		m_sceneManager.ChangeScene<GameScene>();
	}
}

void TitleScene::Draw()
{
	DrawString(260, 180, "===THE EATING KING", COLOR_YELLOW);
	DrawString(250, 240, "PRESS SPACE TO START", COLOR_WHITE);

}