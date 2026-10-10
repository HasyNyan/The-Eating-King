#pragma once
#include "BaseScene.h"
#include "Camera.h"
class GameScene : public BaseScene {
private:
    int m_score = 0; // テスト用スコア変数
    std::unique_ptr<Camera>m_camera;//カメラオブジェクトの保持

public:
    using BaseScene::BaseScene;

    ~GameScene() override = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
};