#pragma once

//前方宣言
class BaceScene;

class SceneManager
{
private:
	std::unique_ptr<BaceScene>m_currentScene;//現在実行中のシーン
	std::unique_ptr<BaceScene>m_nextScene;	 //次のフレームで切り替えるシーン
public:
	SceneManager();
	~SceneManager();

	void Update();
	void Draw();

	template<typename T,typename...Args>
	void ChangeScene(Args&&... args)
	{
		m_nextScene = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}
};

