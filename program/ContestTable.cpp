#include"ContestTable.h"

ContestTable::~ContestTable()
{
	if (m_tableModelHandle != -1)
	{
		MV1DeleteModel(m_tableModelHandle);
		m_tableModelHandle = -1;
	}
	if (m_burgerModelHandle != -1)
	{
		MV1DeleteModel(m_burgerModelHandle);
		m_burgerModelHandle = -1;
	}
}

void ContestTable::Initialize()
{	
	//テーブルモデルの読み込み
	m_tableModelHandle = MV1LoadModel("Assets/Models/Table/table.mv1");

	// ストック描画用のバーガーモデル
	if (m_burgerModelHandle == -1)
	{
		m_burgerModelHandle = MV1LoadModel("Assets/Models/hamburger/hambuger.mv1");
	}
	if (m_burgerModelHandle != -1)
	{
		MV1SetScale(m_burgerModelHandle, VGet(BURGER_SCALE, BURGER_SCALE, BURGER_SCALE));
	}
	for (int i = 0; i < 3; ++i)
	{
		m_stockCounts[i] = INITIAL_STOCK_COUNT;
	}
}

void ContestTable::ConsumeStock(int laneIndex)
{
	if (laneIndex < 0 || laneIndex >= 3)return;

	if (m_stockCounts[laneIndex] > 0)
	{
		m_stockCounts[laneIndex]--;
	}
}

void ContestTable::Draw()
{
	if (!m_isActive)return;

	//テーブルの描画
	if (m_tableModelHandle != -1)
	{
		MV1DrawModel(m_tableModelHandle);
	}
	//各レーンの山積みバーガーストックを描画
	for (int lane = 0; lane < 3; ++lane)
	{
		DrawBurgerStack(lane);
	}
}

void ContestTable::DrawBurgerStack(int laneIndex)const
{
	if (m_burgerModelHandle == -1)return;
	int count = m_stockCounts[laneIndex];
	VECTOR basepos = m_laneBasePos[laneIndex];
}
