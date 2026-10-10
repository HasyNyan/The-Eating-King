#include "Burger.h"

Burger::~Burger()
{
	//読み込んだ3Dモデルのメモリ解放
	if (m_modelHandle != -1)
	{
		MV1DeleteModel(m_modelHandle);
		m_modelHandle = -1;
	}
}

void Burger::Initialize()
{
	m_position = VGet(0.0f, 0.0f, 0.0f);//原点に配置
	m_isBitten = false;
	//3Dモデルファイルの読み込み
	m_modelHandle = MV1LoadModel("Assets/Models/hamburger/hambuger.mv1");

	//内部のボクセルデータ敷き詰め
	for (int x = 0; x < BURGER_SIZE_X; ++x)
	{
		for (int z = 0; z < BURGER_SIZE_Z; ++z)
		{
			float dx = x - (BURGER_SIZE_X - 1) / 2.0f;
			float dz = z - (BURGER_SIZE_Z - 1) / 2.0f;
			float dist = std::sqrt(dx * dx + dz * dz);
			if (dist > (BURGER_SIZE_X / 2.0f))continue;

			for (int y = 0; y < BURGER_SIZE_Y; ++y)
			{
				if (y <= 1)			m_grid[x][y][z] = VoxelType::Bun;		//下バンズ
				else if (y <= 3)	m_grid[x][y][z] = VoxelType::Patty;		//パティ
				else if (y == 4)	m_grid[x][y][z] = VoxelType::Cheese;	//チーズ
				else if (y == 5)	m_grid[x][y][z] = VoxelType::Lettuce;	//レタス
				else 				m_grid[x][y][z] = VoxelType::Bun;		//上バンズ
				
			}
		}
	}
}

void Burger::Update()
{
	//モデルの位置を設定
	if (m_modelHandle != -1)
	{
		MV1SetPosition(m_modelHandle, m_position);
	}
}

void Burger::Draw()
{
	if (!m_isActive)return;
	//噛まれる前:きれいな3Dモデルを描画
	if (!m_isBitten && m_modelHandle != -1)
	{
		MV1DrawModel(m_modelHandle);
		return;
	}
	//噛まれた後、敷き詰められた内包ボクセル群を描画
	float offsetX = (BURGER_SIZE_X)

}