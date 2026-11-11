#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include"Burger.h"
#include"BurgerCompetitor.h"
class GameScene : public BaseScene {
private:
    int m_score = 0; // テスト用スコア変数
    std::unique_ptr<Camera>m_camera;//カメラオブジェクトの保持
    std::unique_ptr<Burger>m_burger;//バーガーオブジェクトの保持
    //ライバルAIの配列
    std::vector<std::unique_ptr<BurgerCompetitor>> m_competitors;
    //Zキー押下時のレイキャスト噛み処理
    void TryBiteBurgerByRay();

public:
    using BaseScene::BaseScene;

    ~GameScene() override = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
};