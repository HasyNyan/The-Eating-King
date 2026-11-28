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
	//3Dモデルファイルの読み込み
	m_modelHandle = MV1LoadModel("Assets/Models/hamburger/hambuger.mv1");
	//モデルのサイズを調整
	if (m_modelHandle != -1)
	{
		MV1SetScale(m_modelHandle, VGet(0.1f, 0.1f, 0.1f));
	}
	//ボクセルと状態の初期化
	Reset();
}

void Burger::Reset()
{
	m_isBitten = false;
	m_rotation = VGet(0.0f, 0.0f, 0.0f);//角度も正面に戻す
	m_initialVoxelCount = 0;//カウントリセット
	//配膳演出からスタート
	m_state = BurgerState::Spawning;
	m_stateTimer = 0.0f;
	m_currentScale =  0.0f;
	m_position = m_tablePos;

	if (m_modelHandle != -1)
	{
		MV1SetPosition(m_modelHandle, m_position);
		MV1SetScale(m_modelHandle, VGet(0.0f, 0.0f, 0.0f));
	}

	// 配列のゼロクリア
	for (int x = 0; x < BURGER_SIZE_X; ++x) {
		for (int y = 0; y < BURGER_SIZE_Y; ++y) {
			for (int z = 0; z < BURGER_SIZE_Z; ++z) {
				m_grid[x][y][z] = VoxelType::None;
			}
		}
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
				if (y <= 3)			m_grid[x][y][z] = VoxelType::Bun;		//下バンズ
				else if (y <= 8)	m_grid[x][y][z] = VoxelType::Patty;		//パティ
				else if (y == 10)	m_grid[x][y][z] = VoxelType::Cheese;	//チーズ
				else if (y == 12)	m_grid[x][y][z] = VoxelType::Lettuce;	//レタス
				else 				m_grid[x][y][z] = VoxelType::Bun;		//上バンズ

				//生成したボクセルの総数を記録
				m_initialVoxelCount++;

			}
		}
	}
	//残りボクセル数を初期値と同じに設定
	m_remainingVoxelCount = m_initialVoxelCount;
}

void Burger::Grab()
{
	//テーブルに置かれている時だけ掴める
	if (m_state == BurgerState::OnTable)
	{
		m_state = BurgerState::MovingToMouth;
		m_stateTimer = 0.0f;
	}
}

void Burger::Update()
{
	switch (m_state)
	{
	case BurgerState::Spawning:
		UpdateSpawning();
		break;
	case BurgerState::OnTable:
		//テーブル上で待機中(掴まれるのを待つ)
		break;
	case BurgerState::MovingToMouth:
		UpdateMovingToMouth();
		break;
	case BurgerState::Eating:
		//食事中のみマウスドラッグ回転を受け付ける
		UpdateRotationInput();
		break;
	}
}

void Burger::UpdateSpawning()
{
	m_stateTimer += m_deltaTime;
	float t = m_stateTimer / SPAWN_DURATION;

	if (t >= 1.0f)
	{
		t = 1.0f;
		m_state = BurgerState::OnTable;//配膳完了 -> テーブル待機状態へ
	}
	//スケールを0.0からBASE_MODEL_SCALEへ拡大
	m_currentScale = BASE_MODEL_SCALE * t;
	if (m_modelHandle != -1)
	{
		MV1SetScale(m_modelHandle, VGet(m_currentScale, m_currentScale, m_currentScale));
	}
}

void Burger::UpdateMovingToMouth()
{
	m_stateTimer += m_deltaTime;
	float t = m_stateTimer / MOVE_DURATION;

	if (t >= 1.0f)
	{
		t = 1.0f;
		m_state = BurgerState::Eating;//到着->食事状態へ移行
	}

	//イージング
	float easeT = 1.0f - std::pow(1.0f - t, 3.0f);

	//テーブル座標からカメラ前座標へ線形補間
	m_position.x = m_tablePos.x + (m_mouthPos.x - m_tablePos.x) * easeT;
	m_position.y = m_tablePos.y + (m_mouthPos.y - m_tablePos.y) * easeT;
	m_position.z = m_tablePos.z + (m_mouthPos.z - m_tablePos.z) * easeT;

}

void Burger::UpdateRotationInput()
{
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

		MV1SetScale(m_modelHandle, VGet(m_currentScale, m_currentScale, m_currentScale));

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

	//ボクセル描画用の設定
	SetUseBackCulling(FALSE);     // 両面描画にして裏返ったポリゴンの消滅を防ぐ

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
	SetUseBackCulling(TRUE);
}


bool Burger::IsEmpty()const
{
	return m_remainingVoxelCount <= 0;
}


int Burger::Bite(int targetX, int targetY, int targetZ, int radius)
{
	m_isBitten = true;//噛まれたのでボクセル表示モードへ切り替え
	int erasedCount = 0;

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
						erasedCount++;
					}
				}

			}
		}
	}
	//残りボクセル数を減算
	m_remainingVoxelCount -= erasedCount;
	if (m_remainingVoxelCount < 0)m_remainingVoxelCount = 0;

	return erasedCount;
}

int Burger::BiteByRay(VECTOR rayOrigin, VECTOR rayDir, int radius)
{
	//食事状態以外なら抜け出す
	if (m_state != BurgerState::Eating)return 0;

	//残量が最低値以下なら、残りすべてを一気に完食
	if (m_initialVoxelCount > 0)
	{
		float remainingRatio = static_cast<float>(m_remainingVoxelCount) / static_cast<float>(m_initialVoxelCount);
		if (remainingRatio <= LAST_BITE_THRESHOLD)
		{
			return ConsumeAllRemaining();
		}
	}

	//レイの方向ベクトルを正規化
	VECTOR normDir = VNorm(rayDir);

	//ワールド空間からハンバーガーのローカル空間への逆変換行列を作成
	MATRIX invRotMat = MGetRotY(-m_rotation.y);
	invRotMat = MMult(invRotMat, MGetRotX(-m_rotation.x));

	//レイの始点と方向をバーガーのローカル回転座標系へ変換
	VECTOR localRayOrigin = VSub(rayOrigin, m_position);
	localRayOrigin	= VTransform(localRayOrigin, invRotMat);
	VECTOR localRayDir = VTransform(normDir, invRotMat);

	float offsetX = (BURGER_SIZE_X * m_voxelSize) / 2.0f;
	float offsetY = (BURGER_SIZE_Y * m_voxelSize) / 2.0f;
	float offsetZ = (BURGER_SIZE_Z * m_voxelSize) / 2.0f;

	//レイをステップ進行させてヒット判定
	float stepSize = 0.2f;
	for (float step = 0.0f; step < 50.0f; step += stepSize)
	{
		VECTOR currentPos = VAdd(localRayOrigin, VScale(localRayDir, step));

		//ローカル空間の座標から配列インデックスに変換
		int gridX = static_cast<int>(std::floor((currentPos.x + offsetX) / m_voxelSize));
		int gridY = static_cast<int>(std::floor((currentPos.y + offsetY) / m_voxelSize));
		int gridZ = static_cast<int>(std::floor((currentPos.z + offsetZ) / m_voxelSize));

		//範囲内かつボクセルが存在すればヒット
		int hitX = 0, hitY = 0, hitZ = 0;
		if (FindNearestVoxel(gridX,gridY,gridZ,RAY_SEARCH_RADIUS,hitX,hitY,hitZ))
		{
			//ヒットした位置を中心に噛みちぎる
			return Bite(hitX, hitY, hitZ, radius);
		}
	}
	return 0;//何もヒットしなかった
}

bool Burger::FindNearestVoxel(int centerX, int centerY, int centerZ, int searchRadius, int& outX, int& outY, int& outZ)const
{
	int minDistSq = searchRadius * searchRadius + 1;
	bool found = false;

	for (int x = centerX - searchRadius; x <= centerX + searchRadius; ++x)
	{
		for (int y = centerY - searchRadius; y <= centerY + searchRadius; ++y)
		{
			for (int z = centerZ - searchRadius; z <= centerZ + searchRadius; ++z)
			{
				if (x < 0 || x >= BURGER_SIZE_X ||
					y < 0 || y >= BURGER_SIZE_Y ||
					z < 0 || z >= BURGER_SIZE_Z)continue;

				if (m_grid[x][y][z] != VoxelType::None)
				{
					int dx = x - centerX;
					int dy = y - centerY;
					int dz = z - centerZ;
					int distSq = dx * dx + dy * dy + dz * dz;

					if (distSq < minDistSq)
					{
						minDistSq = distSq;
						outX = x;
						outY = y;
						outZ = z;
						found = true;
					}
				}

			}
		}
	}
	return found;
}

int Burger::ConsumeAllRemaining()
{
	m_isBitten = true;
	int erasedCount = m_remainingVoxelCount;

	for (int x = 0; x < BURGER_SIZE_X; ++x)
	{
		for (int y = 0; y < BURGER_SIZE_Y; ++y)
		{
			for (int z = 0; z < BURGER_SIZE_Z; ++z)
			{
				m_grid[x][y][z] = VoxelType::None;
			}
		}
	}

	m_remainingVoxelCount = 0;
	return erasedCount;
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