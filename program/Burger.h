#pragma once
//ボクセルの種類
enum class VoxelType
{
	None = 0,
	Bun,	  //パン
	Patty,	  //肉
	Lettuce,  //レタス
	Cheese,   //チーズ
};

//ハンバーガーライフサイクル状態
enum class BurgerState
{
	Spawning,		//テーブルに配膳されるアニメーション中
	OnTable,		//テーブルに置かれて待機中(掴み待ち)
	MovingToMouth,	//テーブルからカメラ前へ持ち上げ移動中
	Eating			//カメラ前で食事中(回転・噛みちぎり可能)
};
constexpr int BURGER_SIZE_X = 24;
constexpr int BURGER_SIZE_Y = 20;
constexpr int BURGER_SIZE_Z = 24;

class Burger :public BaseObject
{
private:
	//配置座標・アニメーション定数
	static constexpr float BASE_MODEL_SCALE = 0.1f;
	static constexpr float SPAWN_DURATION	= 0.25f;	//配膳演出の時間
	static constexpr float MOVE_DURATION	= 0.30f;	//持ち上げ移動の時間

	//噛み判定アシスト用定数
	static constexpr int RAY_SEARCH_RADIUS = 2;		//レイの太さ
	static constexpr float LAST_BITE_THRESHOLD = 0.08f;//残り8%以下でラスト一口アシスト発動


	int	 m_modelHandle = -1;
	bool m_isBitten	   = false; //1度でも噛まれたかどうか
	int m_initialVoxelCount = 0;//初期状態の総ボクセル数
	int m_remainingVoxelCount = 0;//現在の残りボクセル数

	//3Dグリッド配列
	VoxelType m_grid[BURGER_SIZE_X][BURGER_SIZE_Y][BURGER_SIZE_Z]{};
	//1ボクセルのサイズ
	float m_voxelSize = 0.25f;

	//マウス回転用の変数
	VECTOR m_rotation = VGet(0.0f, 0.0f, 0.0f);
	int m_prevMouseX = 0;	//前フレームのマウスX座標
	int m_prevMouseY = 0;	//前フレームのマウスY座標

	//状態管理と移動アニメーション用変数
	BurgerState m_state = BurgerState::Spawning;
	float m_stateTimer = 0.0f;
	float m_deltaTime = 0.0f;
	float m_currentScale = BASE_MODEL_SCALE;

	VECTOR m_tablePos = VGet(0.0f, -6.0f, -12.0f);//テーブル上の待機位置
	VECTOR m_mouthPos = VGet(0.0f,  12.0f,   -20.0f);//カメラ前の食事位置

	//各ボクセルの種類に応じた描画色を取得するヘルパー関数
	unsigned int GetVoxelColor(VoxelType type)const;

	//指定座標の周辺に有効なボクセルがあればその座標を返す
	bool FindNearestVoxel(int centerX, int centerY, int centerZ, int searchRadius, int& outX, int& outY, int& outZ)const;
	//残っているすべてのボクセルを一気に消去する
	int ConsumeAllRemaining();

	//状態ごとの更新ヘルパー
	void UpdateSpawning();
	void UpdateMovingToMouth();
	void UpdateRotationInput();

public:
	Burger() = default;
	~Burger() override;

	void Initialize() override;
	void Update()	  override;
	void Draw()		  override;

	void SetDeltaTime(float dt) { m_deltaTime = dt; }
	int GetInitialVoxelCount()const { return m_initialVoxelCount; }
	int GetRemainingVoxelCount()const { return m_remainingVoxelCount; }
	BurgerState GetState() const { return m_state; }

	//テーブル上のバーガーを掴んでカメラ前へ持ち上げる
	void Grab();

	//おかわり用:3Dモデルを再ロードせず、ボクセルと状態だけを初期化する
	void Reset();

	//すべてのボクセルが空になったか判定
	bool IsEmpty() const;

	//噛みちぎり処理
	int Bite(int targetX, int targetY, int targetZ, int radius);

	//レイ(光線)を飛ばしてヒットした場所を噛む
	int BiteByRay(VECTOR rayOrigin, VECTOR rayDir, int radius);
};