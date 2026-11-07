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

constexpr int BURGER_SIZE_X = 24;
constexpr int BURGER_SIZE_Y = 20;
constexpr int BURGER_SIZE_Z = 24;

class Burger :public BaseObject
{
private:
	int	 m_modelHandle = -1;
	bool m_isBitten	   = false; //1度でも噛まれたかどうか
	//3Dグリッド配列
	VoxelType m_grid[BURGER_SIZE_X][BURGER_SIZE_Y][BURGER_SIZE_Z]{};
	//1ボクセルのサイズ
	float m_voxelSize = 0.25f;

	//マウス回転用の変数
	VECTOR m_rotation = VGet(0.0f, 0.0f, 0.0f);
	int m_prevMouseX = 0;	//前フレームのマウスX座標
	int m_prevMouseY = 0;	//前フレームのマウスY座標

	//各ボクセルの種類に応じた描画色を取得するヘルパー関数
	unsigned int GetVoxelColor(VoxelType type)const;
public:
	Burger();
	~Burger() override;

	void Initialize() override;
	void Update()	  override;
	void Draw()		  override;

	//噛みちぎり処理
	bool Bite(int targetX, int targetY, int targetZ, int radius);

	//レイ(光線)を飛ばしてヒットした場所を噛む
	int BiteByRay(VECTOR rayOrigin, VECTOR rayDir, int radius);

	void SetRotation(VECTOR rot) { m_rotation = rot; }
};