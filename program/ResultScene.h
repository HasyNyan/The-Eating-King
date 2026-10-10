#pragma once
#include"BaseScene.h"

class ResultScene : public BaseScene
{
private:
	int m_finalScore = 0;
public:
	//コンストラクタでSceneManagerの参照とスコアを受け取る
	ResultScene(SceneManager& manager, int score);

	~ResultScene() override = default;

	void Initialize() override;
	void Finalize()   override;
	void Update()     override;
	void Draw()       override;
};