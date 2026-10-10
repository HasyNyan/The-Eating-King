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

constexpr int BURGER_SIZE_X = 12;
constexpr int BURGER_SIZE_Y = 10;
constexpr int BURGER_SIZE_Z = 12;

class Burger :public BaseObject
{
private:
	int	 m_modelHandle = -1;
	bool m_isBitten	   = false; //1度でも噛まれたかどうか
	//3Dグリッド配列
	VoxelType m_grid [BURGER_SIZE_X] [BURGER_SIZE_Y] [BURGER_SIZE_Z];
	//1ボクセルのサイズ
	float m_voxelSize = 0.5f;
	//各ボクセルの種類に応じた描画色を取得するヘルパー関数
	unsigned int GetVoxelColor(VoxelType type)const;
public:
	Burger() = default;
	~Burger() override = default;

	void Initialize() override;
	void Update()	  override;
	void Draw()		  override;

	//特定の座標のボクセルを消す(かみちぎり用関数)
	bool RemoveVoxel(int x, int y, int z);

	//ボクセルデータが存在するか確認
	bool IsVoxelActive(int x, int y, int z)const;
};