#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include"Burger.h"
#include"BurgerCompetitor.h"
#include"GameTimer.h"
class GameScene : public BaseScene {
private:
    //スコア配分定数
    static constexpr int SCORE_PER_BURGER   = 1000;
    static constexpr int COMPLETE_BONUS     = 200;
    static constexpr int PLAYER_BITE_RADIUS = 4;



    int m_score = 0; // テスト用スコア変数
    int m_playerEatenCount = 0;
    LONGLONG m_prevTime = 0; //前フレームのシステム時間

    std::unique_ptr<Camera>m_camera;//カメラオブジェクトの保持
    std::unique_ptr<Burger>m_burger;//バーガーオブジェクトの保持
    //ライバルAIの配列
    std::vector<std::unique_ptr<BurgerCompetitor>> m_competitors;
    std::unique_ptr<GameTimer>m_gameTimer;//タイマーオブジェクトの保持

    //Update用ヘルパー関数
    float CalculateDeltaTime();
    bool UpdateTimer(float deltaTime);//タイムアップしてシーン遷移したらtrueを返す関数
    void UpdateCompetitors(float deltaTime);
    void HandleInput();
    //Zキー押下時のレイキャスト噛み処理
    void TryBiteBurgerByRay();
    //Draw用ヘルパー関数
    void DrawTimerUI()const;          //タイマーのUI描画

public:
    using BaseScene::BaseScene;

    ~GameScene() override = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
};