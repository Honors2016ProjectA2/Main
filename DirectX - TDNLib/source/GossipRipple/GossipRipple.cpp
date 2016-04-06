#include "GossipRipple.h"


GossipRipple::GossipRipple(RIPPLE_TYPE type, Vector3 pos)
{
	m_Type = type;	// 波紋の色のタイプ

	m_uvObj = NULL;
	m_Pos = pos;
	m_Size = 0.1f;

	switch (m_Type)// タイプによってUVエフェクトを変える
	{
	case RIPPLE_TYPE::WHITE:
		m_uvObj = new AnimationUV("DATA/UV/wave.imo", 0.00f, 0.01f, 60, false, 1, 45);
		break;
	case RIPPLE_TYPE::RED:
		m_uvObj = new AnimationUV("DATA/UV/Red/wave.imo", 0.00f, 0.01f, 60, false, 1, 45);
		break;
	case RIPPLE_TYPE::BLUE:
		m_uvObj = new AnimationUV("DATA/UV/Blue/wave.imo", 0.00f, 0.01f, 60, false, 1, 45);
		
		m_Size = 0.25f;// (仮)サイズでかく
		break;
	default:
		MyAssert(0, "ねぇよ");
		break;
	}


	//m_Flame = 0;

	//isAction = false;
	//isEnd = false;

}

GossipRipple::~GossipRipple()
{
	SAFE_DELETE(m_uvObj);
}

// 実行
void GossipRipple::Action()
{
	// 新しく
	m_uvObj->Action();

	//isAction = true;
	// 初期化
	//isEnd = false;
	//m_Flame = 0;
	//m_Size = 1.0f;
}

// 更新
void GossipRipple::Update()
{
	//if (isAction == false)return;// 起動するまで更新しない。 

	//m_Flame++;// カウント開始
	//if (m_Flame >= 40)
	//{
	//	m_Flame = 0;
	//	isEnd = true;
	//	isAction = false;	// 実行停止
	//}

	//m_Size += 0.01f;
	//m_uvObj->GetObj()->SetScale(m_Size);
	//obj->SetPos(m_Pos);
	//obj->Update();
	m_uvObj->Update(m_Pos, VECTOR_ZERO, m_Size);
}

// 描画
void GossipRipple::Render()
{
	//if (isAction == false)return;// 起動するまで更新しない。

	m_uvObj->Render();
}
