#include "Burger.h"

Burger::Burger()
	:m_modelHandle(-1)
	, m_isBitten(false)
	, m_voxelSize(0.5f)
{
	// 配列のゼロクリア
	for (int x = 0; x < BURGER_SIZE_X; ++x) {
		for (int y = 0; y < BURGER_SIZE_Y; ++y) {
			for (int z = 0; z < BURGER_SIZE_Z; ++z) {
				m_grid[x][y][z] = VoxelType::None;
			}
		}
	}
}


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
	//モデルのサイズを調整
	if (m_modelHandle != -1)
	{
		MV1SetScale(m_modelHandle, VGet(0.1f, 0.1f, 0.1f));
	}

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

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	//マウス左クリックをされている間にドラッグで回転
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		int diffX = mouseX - m_prevMouseX;
		int diffY = mouseY - m_prevMouseY;

		//移動量に応じて回転角を加算
		m_rotation.y += diffX * 0.005f;//左右ドラッグでY軸回転
		m_rotation.x += diffY * 0.005f;//上下ドラッグでX軸回転
	}
	//現在のマウス位置を保存
	m_prevMouseX = mouseX;
	m_prevMouseY = mouseY;

}

void Burger::Draw()
{
	if (!m_isActive)return;
	//噛まれる前:きれいな3Dモデルを描画
	if (!m_isBitten && m_modelHandle != -1)
	{
		// 座標設定
		MV1SetPosition(m_modelHandle, m_position);
		//モデルの回転を設定
		MV1SetRotationXYZ(m_modelHandle, m_rotation);

		MV1DrawModel(m_modelHandle);
		return;
	}

	//噛まれた後、敷き詰められた内包ボクセル群を描画
	float offsetX = (BURGER_SIZE_X * m_voxelSize) / 2.0f;
	float offsetY = (BURGER_SIZE_Y * m_voxelSize) / 2.0f;
	float offsetZ = (BURGER_SIZE_Z * m_voxelSize) / 2.0f;

	//回転行列を作成
	MATRIX rotMat = MGetRotX(m_rotation.x);
	rotMat = MMult(rotMat, MGetRotY(m_rotation.y));


	for (int x = 0; x < BURGER_SIZE_X; ++x)
	{
		for (int y = 0; y < BURGER_SIZE_Y; ++y)
		{
			for (int z = 0; z < BURGER_SIZE_Z; ++z)
			{
				VoxelType type = m_grid[x][y][z];
				if(type == VoxelType::None) continue;

				//ローカル空間でのボクセル中心座標
				VECTOR localPos = VGet(
					(x * m_voxelSize) - offsetX,
					(y * m_voxelSize) - offsetY,
					(z * m_voxelSize) - offsetZ
				);
				//回転行列を適用してワールド座標へ移動
				VECTOR rotatedPos = VTransform(localPos, rotMat);
				VECTOR finalPos = VAdd(m_position, rotatedPos);

				//ボクセルの対角最小・最大点
				float halfSize = m_voxelSize / 2.0f;
				VECTOR minPos = VGet(finalPos.x - halfSize, finalPos.y - halfSize, finalPos.z - halfSize);
				VECTOR maxPos = VGet(finalPos.x + halfSize, finalPos.y + halfSize, finalPos.z + halfSize);
		
				unsigned int color = GetVoxelColor(type);
				DrawCube3D(minPos, maxPos, color, GetColor(40, 40, 40), TRUE);
			}
		}
	}
}

bool Burger::Bite(int targetX, int targetY, int targetZ, int radius)
{
	m_isBitten = true;//噛まれたのでボクセル表示モードへ切り替え
	bool ateSomething = false;

	//噛んだ中心点から球範囲のボクセルを空にする
	for (int x = targetX - radius; x <= targetX + radius; ++x)
	{
		for (int y = targetY - radius; y <= targetY + radius; ++y)
		{
			for (int z = targetZ - radius; z <= targetZ + radius; ++z)
			{
				if (x < 0 || x >= BURGER_SIZE_X ||
					y < 0 || y >= BURGER_SIZE_Y ||
					z < 0 || z >= BURGER_SIZE_Z)continue;

				int dx = x - targetX;
				int dy = y - targetY;
				int dz = z - targetZ;
				if ((dx * dx + dy * dy + dz * dz) <= (radius * radius))
				{
					if (m_grid[x][y][z] != VoxelType::None)
					{
						m_grid[x][y][z] = VoxelType::None;
						ateSomething = true;
					}
				}

			}
		}
	}
	return ateSomething;
}

unsigned int  Burger::GetVoxelColor(VoxelType type)const
{
	switch(type)
	{
	case VoxelType::Bun:	  return GetColor(210, 140,  70);
	case VoxelType::Patty:	  return GetColor( 90,  45,  20);
	case VoxelType::Cheese:	  return GetColor(255, 210,   0);
	case VoxelType::Lettuce:  return GetColor( 40, 180,  40);
	default:				  return GetColor(255, 255, 255);
	}
}