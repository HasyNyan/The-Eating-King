#pragma once
#include "BaseScene.h"

class GameScene : public BaseScene {
private:
    int m_score = 0; // テスト用スコア変数

public:
    using BaseScene::BaseScene;

    ~GameScene() override = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
};