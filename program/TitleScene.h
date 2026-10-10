#pragma once
#include "BaseScene.h"

class TitleScene :public BaseScene
{
public:
	//親クラス(BaseScene)のコンストラクタを継承
	using BaseScene::BaseScene;

	~TitleScene() override = default;

	void Initialize() override;
	void Finalize()	  override;
	void Update()     override;
	void Draw()       override;
};