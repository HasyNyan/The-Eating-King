#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include"Burger.h"
#include"BurgerCompetitor.h"
#include"GameTimer.h"
class GameScene : public BaseScene {
private:
    int m_score = 0; // テスト用スコア変数
    LONGLONG m_prevTime = 0; //前フレームのシステム時間

    std::unique_ptr<Camera>m_camera;//カメラオブジェクトの保持
    std::unique_ptr<Burger>m_burger;//バーガーオブジェクトの保持
    //ライバルAIの配列
    std::vector<std::unique_ptr<BurgerCompetitor>> m_competitors;
    std::unique_ptr<GameTimer>m_gameTimer;//タイマーオブジェクトの保持

    bool UpdateTimer(float deltaTime);//タイムアップしてシーン遷移したらtrueを返す関数
    void DrawTimerUI()const;          //タイマーのUI描画
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