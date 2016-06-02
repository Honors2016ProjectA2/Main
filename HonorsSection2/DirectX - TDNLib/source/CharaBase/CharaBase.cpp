#include "TDNLIB.h"
#include "CharaBase.h"
#include "../Sound/SoundManager.h"

//**************************************************
//	�q���H���đ������r
//**************************************************
DebuBase::DebuBase(tdn2DObj *image, const Vector2 &pos) :m_image(image), m_pos(pos), m_angle(0), m_bErase(false), m_moveX(0), m_ReceiveSE(TDNSOUND_PLAY_NONE), m_AnimFrame(0), m_AnimPanel(0), m_accel(0)
{

}

DebuBase::~DebuBase()
{
	// ���[�v���Ă�SE���~�߂�
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("�������r����", m_ReceiveSE);
}

void DebuBase::Update()
{
	// �A�j��
	if (++m_AnimFrame > 4)
	{
		m_AnimFrame = 0;
		if (++m_AnimPanel > 3)m_AnimPanel = 0;
	}

	// SE�炵�ĂȂ��Ƃ��̏��
	if (m_ReceiveSE == TDNSOUND_PLAY_NONE)
	{
		// �����Ă���
		if (m_moveX != 0) m_ReceiveSE = se->Play("�������r����", m_pos, Vector2(0, 0), true);	// SE�Đ�(�f�X�g���N�^�Ŏ~�߂�)
	}
	else
	{
		// SE�̍��W��ݒ�
		se->SetPos("�������r����", m_ReceiveSE, m_pos);
	}
	// �����x�𑫂��Ă���
	m_moveX += m_accel;
	// �ړ��l�𑫂��Ă���
	m_pos.x += m_moveX;
	// �ړ��l�ɉ����ėr����]������
	m_angle -= m_moveX * 0.01f;

	// ��C��R
	m_accel = max(m_accel - 0.01f, 0);
	m_moveX = max(m_moveX - 0.01f, 0);
}

void DebuBase::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, m_AnimPanel * 240, 0, 240, 240);
}
