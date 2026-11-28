#pragma once

class ContestTable :public BaseObject
{
private:
	//テーブルとストックのレイアウト定数
	static constexpr int INITIAL_STOCK_COUNT = 12;	//1レーンあたりの初期バーガー山積み数
	static constexpr float BURGER_SCALE = 0.1f;

	int m_tableModelHandle = -1;	//テーブル用モデルハンドル
	int m_burgerModelHandle = -1;	//山積みストック描画用の共用バーガーモデル

	//各レーンの残りストック数
	int m_stockCounts[3] = { INITIAL_STOCK_COUNT,INITIAL_STOCK_COUNT,INITIAL_STOCK_COUNT };

	//各レーンの山積み基準座標
	VECTOR m_laneBasePos[3] =
	{
		VGet(  0.0f,-6.0f,-12.0f),	//プレイヤー手前
		VGet(-12.0f,-6.0f, -4.0f),	//CPU1
		VGet( 12.0f,-6.0f, -4.0f)	//CPU2
	};

	//指定レーンの山積みバーガーを描画する関数
	void DrawBurgerStack(int laneIndex) const;

public:
	ContestTable() = default;
	~ContestTable() override{}
	
	void Initialize() override;
	void Update() override{}
	void Draw() override;

	//レーン番号
	void ConsumeStock(int laneIndex);

	//プレイヤーが次に掴むバーガーのテーブル上出現座標を取得
	VECTOR GetPlayerGrabPos() const { return m_laneBasePos[0]; }

};