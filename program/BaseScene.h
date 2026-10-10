#pragma once

//前方宣言(BaseScene内でSceneManagerをインクルードされるのを防ぐ)
class SceneManager;

class BaseScene
{
protected:
	SceneManager& m_sceneManager;//シーン遷移をするためのマネージャーへの参照
public:
	//コンストラクタ
	explicit BaseScene(SceneManager& manager);
		
	//デストラクタ
	virtual ~BaseScene() = default;

	virtual void Initialize()	= 0;
	virtual void Finalize()		= 0;
	virtual void Update()		= 0;
	virtual void Draw()			= 0;
};