#include "SendPower.h"
#include "particle_2d\particle_2d.h"


SendPower::SendPower(char* filename, Vector3 startPos, Vector3 centerPos,
	Vector3 center2Pos, Vector3 endPos, int endFlame, int power, int bBokusou)
{
	// いらす地
	m_pic = new tdn2DObj(filename);

	/****************************/
	//	初期化
	/****************************/
	m_pos = VECTOR_ZERO;
	for (int i = 0; i < (int)MOVE_POS::MAX; i++)
	{
		m_movePos[i] = VECTOR_ZERO;
	}

	// 設定
	m_movePos[(int)MOVE_POS::START] = startPos;
	m_movePos[(int)MOVE_POS::CENTER] = centerPos;
	m_movePos[(int)MOVE_POS::CENTER2] = center2Pos;
	m_movePos[(int)MOVE_POS::END] = endPos;

	m_flame = 0;
	m_percent = 0.0f;

	m_endFlame = endFlame;

	// レート
	m_percentRate = 1.0f / (float)m_endFlame;

	/*****************/
	m_bAction = false;
	m_bEnd = false;

	/************/
	// 送る値
	m_power = power;

	m_bBokusou = (bBokusou) ? true : false;	// わーにんぐ防止
}

SendPower::~SendPower()
{
	SAFE_DELETE(m_pic);
}

void SendPower::Update()
{
	if (m_bAction == false)return;// でていけ

	//if (KeyBoard(KB_J))
	//{
	//	m_percent -= 0.01f;
	//}
	//if (KeyBoard(KB_K))
	//{
	//	m_percent += 0.01f;
	//}

	// ジャッジですの
	EndJuge();

	// 動きの処理
	Move();

	// ベジェ移動
	Bezier();

}

void SendPower::Bezier()
{

	Math::Bezier(&m_pos, m_movePos, (int)MOVE_POS::MAX, m_percent);
}

void SendPower::Move()
{
	// レートを足していく
	m_percent += m_percentRate;
	// クランプ
	m_percent = Math::Clamp(m_percent, 0.0f, 1.0f);

	
	Particle2dManager::Effect_KiraKira(Vector2(m_pos.x, m_pos.y), Vector2(16, 16), 20, 5, 1, 80);// ぱーてぃくる

}

void SendPower::EndJuge()
{
	// 1.0fつまり最後まで終わったら
	if (m_percent >= 1.0f)
	{
		m_bAction = false;
		m_bEnd = true;
	}

}

void SendPower::Render()
{
	if (m_bAction == false)return;// でていけ

								  // ■描画
	m_pic->Render((int)m_pos.x - 64, (int)m_pos.y - 64);
	
	//
	//tdnPolygon::Rect(m_movePos[(int)MOVE_POS::CENTER].x - 64 , m_movePos[(int)MOVE_POS::CENTER].y - 64, 128, 128, RS::COPY, 0xff00ffff);
	//tdnPolygon::Rect(m_movePos[(int)MOVE_POS::CENTER2].x - 64, m_movePos[(int)MOVE_POS::CENTER2].y - 64, 128, 128, RS::COPY, 0xff00ff00);
	//tdnPolygon::Rect(m_movePos[(int)MOVE_POS::END].x - 64, m_movePos[(int)MOVE_POS::END].y - 64, 128, 128, RS::COPY, 0xff0000ff);

}

void SendPower::Action()
{
	m_bAction = true;
	m_percent = 0.0f;
}




