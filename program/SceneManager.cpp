#include "SceneManager.h"
#include"BaceScene.h"

SceneManager::SceneManager()
	:m_currentScene(nullptr),m_nextScene(nullptr){}

SceneManager::~SceneManager()
{
	///終了時に現在のシーンを破棄
	if (m_currentScene)
	{
		m_currentScene->Finalize();
	}
}

void SceneManager::Update()
{
	//次のシーンの値が入っていれば切り替える
	if (m_nextScene)
	{
		if (m_currentScene)
		{
			m_currentScene->Finalize();//既存のシーンの終了処理
		}

		m_currentScene = std::move(m_nextScene);//所有権を移動
		m_currentScene->Initialize();			//新しいシーンの初期化処理
	}

	//現在のシーンの更新処理を実行
	if (m_currentScene)
	{
		m_currentScene->Update();
	}
}

void SceneManager::Draw()
{
	//現在のシーンの描画処理を実行
	if (m_currentScene)
	{
		m_currentScene->Draw();
	}
}