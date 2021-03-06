#include "TDNLIB.h"
#include "CharaBase.h"
#include "../Sound/SoundManager.h"

//**************************************************
//	牧草食って太った羊
//**************************************************
DebuBase::DebuBase(tdn2DObj *image, const Vector2 &pos, float radius) :m_radius(radius), m_image(image), m_pos(pos), m_angle(0), m_bErase(false), m_moveX(0), m_ReceiveSE(TDNSOUND_PLAY_NONE), m_AnimFrame(0), m_AnimPanel(0), m_accel(0)
{

}

DebuBase::~DebuBase()
{
	// ループしてるSEを止める
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("太った羊押す", m_ReceiveSE);
}

void DebuBase::Update()
{
	// アニメ
	if (++m_AnimFrame > 7)
	{
		m_AnimFrame = 0;
		if (++m_AnimPanel > 3)m_AnimPanel = 0;
	}

	// 動いてたら
	if (m_moveX != 0)
	{
		// SE鳴らしてないときの状態
		if (m_ReceiveSE == TDNSOUND_PLAY_NONE)m_ReceiveSE = se->Play("太った羊押す", m_pos, Vector2(0, 0), true);	// SE再生(デストラクタで止める)
		// SEの座標を設定
		se->SetPos("太った羊押す", m_ReceiveSE, m_pos);
	}
	// 動いてなかったら
	else
	{
		// SE流してたら
		if (m_ReceiveSE != TDNSOUND_PLAY_NONE)
		{
			// SE止める
			se->Stop("太った羊押す", m_ReceiveSE);
			m_ReceiveSE = TDNSOUND_PLAY_NONE;
		}
	}
	// 加速度を足していく
	m_moveX += m_accel;
	// 移動値を足していく
	m_pos.x += m_moveX;
	// 移動値に応じて羊を回転させる
	m_angle -= m_moveX * 0.01f;

	// 空気抵抗
	m_accel = max(m_accel - 0.01f, 0);
	m_moveX = max(m_moveX - 0.01f, 0);
}

void DebuBase::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, m_AnimPanel * 240, 0, 240, 240);
}
