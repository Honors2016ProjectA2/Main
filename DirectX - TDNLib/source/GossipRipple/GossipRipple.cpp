#include "GossipRipple.h"


GossipRipple::GossipRipple(RIPPLE_TYPE type, Vector3 pos)
{
	m_Type = type;	// 波紋の色のタイプ

	obj = NULL;
	switch (m_Type)
	{
	case RIPPLE_TYPE::WHITE:
		obj = new iexMesh("Data/hasubo.IMO");
		break;
	case RIPPLE_TYPE::RED:
		obj = new iexMesh("Data/hasubo.IMO");
		break;
	case RIPPLE_TYPE::BLUE:
		obj = new iexMesh("Data/hasubo.IMO");
		break;
	default:
		MyAssert(0, "ねぇよ");
		break;
	}

	m_Pos = pos;
	m_Size = 1.0f;
	m_Flame = 0;

	isAction = false;
	isEnd = false;

}

GossipRipple::~GossipRipple()
{
	SAFE_DELETE(obj);
}

// 実行
void GossipRipple::Action()
{
	isAction = true;
	// 初期化
	isEnd = false;
	m_Flame = 0;
	m_Size = 1.0f;
}

// 更新
void GossipRipple::Update()
{
	if (isAction == false)return;// 起動するまで更新しない。 

	m_Flame++;// カウント開始
	if (m_Flame >= MAX_FLAME)
	{
		m_Flame = 0;
		isEnd = true;
		isAction = false;	// 実行停止
	}

	//m_Size += 0.01f;
	obj->SetScale(m_Size);
	obj->SetPos(m_Pos);
	obj->Update();
}

// 描画
void GossipRipple::Render()
{
	if (isAction == false)return;// 起動するまで更新しない。

	obj->Render();
}
