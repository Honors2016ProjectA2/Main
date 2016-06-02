#include "TDNLIB.h"
#include "CharaBase.h"
#include "../Sound/SoundManager.h"

//**************************************************
//	–q‘H‚Á‚Ä‘¾‚Á‚½—r
//**************************************************
DebuBase::DebuBase(tdn2DObj *image, const Vector2 &pos) :m_image(image), m_pos(pos), m_angle(0), m_bErase(false), m_moveX(0), m_ReceiveSE(TDNSOUND_PLAY_NONE), m_AnimFrame(0), m_AnimPanel(0), m_accel(0)
{

}

DebuBase::~DebuBase()
{
	// ƒ‹[ƒv‚µ‚Ä‚éSE‚ðŽ~‚ß‚é
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("‘¾‚Á‚½—r‰Ÿ‚·", m_ReceiveSE);
}

void DebuBase::Update()
{
	// ƒAƒjƒ
	if (++m_AnimFrame > 4)
	{
		m_AnimFrame = 0;
		if (++m_AnimPanel > 3)m_AnimPanel = 0;
	}

	// SE–Â‚ç‚µ‚Ä‚È‚¢‚Æ‚«‚Ìó‘Ô
	if (m_ReceiveSE == TDNSOUND_PLAY_NONE)
	{
		// “®‚¢‚Ä‚½‚ç
		if (m_moveX != 0) m_ReceiveSE = se->Play("‘¾‚Á‚½—r‰Ÿ‚·", m_pos, Vector2(0, 0), true);	// SEÄ¶(ƒfƒXƒgƒ‰ƒNƒ^‚ÅŽ~‚ß‚é)
	}
	else
	{
		// SE‚ÌÀ•W‚ðÝ’è
		se->SetPos("‘¾‚Á‚½—r‰Ÿ‚·", m_ReceiveSE, m_pos);
	}
	// ‰Á‘¬“x‚ð‘«‚µ‚Ä‚¢‚­
	m_moveX += m_accel;
	// ˆÚ“®’l‚ð‘«‚µ‚Ä‚¢‚­
	m_pos.x += m_moveX;
	// ˆÚ“®’l‚É‰ž‚¶‚Ä—r‚ð‰ñ“]‚³‚¹‚é
	m_angle -= m_moveX * 0.01f;

	// ‹ó‹C’ïR
	m_accel = max(m_accel - 0.01f, 0);
	m_moveX = max(m_moveX - 0.01f, 0);
}

void DebuBase::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, m_AnimPanel * 240, 0, 240, 240);
}
