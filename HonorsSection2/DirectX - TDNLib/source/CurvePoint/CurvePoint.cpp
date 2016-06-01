#include "TDNLIB.h"
#include "CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../Effect/EffectManager.h"

//**************************************************
//    CurvePoint class
//**************************************************

//------- constructor,destructor ---------

CurvePoint::Base::Base(StageManager *pMgr, tdn2DObj* image, Vector2 pos, DIR dir, int ID, int floor) :m_ID(ID), m_floor(floor), m_pMgr(pMgr),
W(120), H(120), SW(120), SH(120),
LOCK_TIME(600),
m_pImage(image), m_pos(pos), m_dir(dir), m_bCursorIn(false), m_AnimFrame(0), m_AnimPanel(0)
{
	state = CurvePointState::CLOSE;
}

CurvePoint::Base::~Base()
{

}

//----------- public method ------------

void CurvePoint::Base::Update()
{
	if (IsOpening())
	{
		if (++m_AnimFrame > 30)
		{
			m_AnimFrame = 0;
			if (++m_AnimPanel > 1) m_AnimPanel = 0;
		}
	}
}

void CurvePoint::Base::Render()
{
	if (IsOpening())
	{
		m_pImage->SetARGB((BYTE)255, (BYTE)255, (BYTE)255, (BYTE)255);
		m_pImage->Render((int)m_pos.x, (int)m_pos.y, W, H, m_AnimPanel*SW, 0, SW, SH);
	}
	else
	{
		m_pImage->SetARGB((BYTE)96, (BYTE)255, (BYTE)255, (BYTE)255);
		m_pImage->Render((int)m_pos.x, (int)m_pos.y, W, H, m_AnimPanel*SW, 0, SW, SH);
	}

}

void CurvePoint::Base::SetPos(int y)
{
	m_pos.y = (float)y;
}

bool CurvePoint::Base::IsOpening()
{
	return (state == CurvePointState::OPEN);
}

int CurvePoint::Base::GetWidth()
{
	return W;
}












////**************************************************
////    �q���Ϗ��N���X
////**************************************************
//void CurvePoint::Fire::Mode::Base::Update()
//{
//	// �ݒ莞�ԉ߂�����
//	if (++m_timer > m_limit)
//	{
//		NextMode();	// ���̃��[�h��
//	}
//}
//
//void CurvePoint::Fire::Mode::Base::Render()
//{
//	m_pImage->SetARGB((BYTE)255, (BYTE)255, (BYTE)255, (BYTE)255);
//	m_pImage->Render(pFire->GetPosX(), pFire->GetPosY(), 128, 128, m_srcX, 0, 128, 128);
//}
//
//void CurvePoint::Fire::Mode::Base::NextMode()
//{
//	// ���̃��[�h�ɕύX
//	pFire->ChangeMode(m_NextMode);
//}
//
////===========================================================
////		�܂��̏��
////===========================================================
//CurvePoint::Fire::Mode::Maki::Maki(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// �R���n�߂ɂ���
//	m_NextMode = FIRE_MODE::MOEHAJIME;
//
//	// �摜���W
//	m_srcX = 0;
//}
////void CurvePoint::Fire::Mode::Hutaba::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
////===========================================================
////		�R���n�ߏ��
////===========================================================
//CurvePoint::Fire::Mode::Moehajime::Moehajime(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// �R���Ă����ɂ���
//	m_NextMode = FIRE_MODE::MOETEKITA;
//
//	// �摜���W
//	m_srcX = 128;
//
//	// SE�̍Đ�
//	se->Play("�_��", me->GetPos());
//}
////void CurvePoint::Fire::Mode::Honba::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
////===========================================================
////		�R���Ă������
////===========================================================
//CurvePoint::Fire::Mode::Moetekita::Moetekita(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// �R�����ɍs��
//	m_NextMode = FIRE_MODE::MOETA;
//
//	// �摜���W
//	m_srcX = 256;
//
//	// SE�̍Đ�
//	se->Play("�R���Ă���", me->GetPos());
//}
////void CurvePoint::Fire::Mode::Tsubomi::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
//
////===========================================================
////		�������\
////===========================================================
//CurvePoint::Fire::Mode::Moeta::Moeta(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// �����ĔR��������
//	m_NextMode = FIRE_MODE::ENABLE;
//
//	// �摜���W
//	m_srcX = 128 * 3;
//
//	// SE�̍Đ�
//	se->Play("�R���Ă���", me->GetPos());
//
//}
////void CurvePoint::Fire::Mode::Saita::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
////===========================================================
////		�ő�Η�
////===========================================================
//CurvePoint::Fire::Mode::Enable::Enable(Fire *me, tdn2DObj *image) :Base(me, image), m_KiraKiraCoolTime(0)
//{
//	// �摜���W
//	m_srcX = 0;
//
//	// SE�̍Đ�
//	se->Play("�R���ŏI", me->GetPos());
//
//	// �G�t�F�N�g����
//	EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PLUS);
//}
//void CurvePoint::Fire::Mode::Enable::Update()
//{
//	// ���炫��p�[�e�B�N��
//	//if (++m_KiraKiraCoolTime > 4)
//	//{
//	//	m_KiraKiraCoolTime = 0;
//	//	Particle2dManager::Effect_KiraKira(pFire->GetCenterPos(), Vector2(56, 56), 15.0f, 10.0f);
//	//}
//}
//
////===========================================================
////		�q�b�g��
////===========================================================
//CurvePoint::Fire::Mode::Hited::Hited(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// �摜���W
//	m_srcX = 0;
//}
//void CurvePoint::Fire::Mode::Hited::Update()
//{
//	// �ݒ莞�ԉ߂�����
//	if (++m_timer > m_limit)
//	{
//		pFire->Change();	// �߂�
//	}
//}
//
////**************************************************
////    �q���N���X
////**************************************************
//
////===========================================================
////		�������E���
////===========================================================
//CurvePoint::Fire::Fire(StageManager *pMgr, tdn2DObj *image, Vector2 pos, DIR dir, int ID, int floor) :Base(pMgr, image, pos, dir, ID, floor), m_pMode(nullptr), m_pFireImage(new tdn2DObj("DATA/�q��/��.png"))
//{
//	// ���[�h������(�d����J�n)
//	this->ChangeMode(FIRE_MODE::MAKI);
//}
//
//CurvePoint::Fire::~Fire()
//{
//	delete m_pMode;
//	delete m_pFireImage;
//}
//
//
////===========================================================
////		�X�V
////===========================================================
//void CurvePoint::Fire::Update()
//{
//	if (IsOpening())
//	{
//		m_pMode->Update();
//	}
//}
//
//
//
////===========================================================
////		�`��
////===========================================================
//void CurvePoint::Fire::Render()
//{
//	if (IsOpening())m_pMode->Render();
//	else if (m_pMgr->m_FireSelect && !m_pMgr->GetDog(m_floor, m_ID)->IsOpening())
//	{
//		// �������`��
//		m_pImage->SetARGB((BYTE)96, (BYTE)255, (BYTE)255, (BYTE)255);
//		m_pImage->Render((int)m_pos.x, (int)m_pos.y, 120, 120, 0, 0, 120, 120);
//	}
//}
//
//void CurvePoint::Fire::Change()
//{
//	if (IsOpening())
//	{
//		state = CurvePointState::CLOSE;
//		m_pMgr->m_FireStock++;	// �X�g�b�N��
//	}
//	else
//	{
//		state = CurvePointState::OPEN;
//		ChangeMode(FIRE_MODE::MAKI);
//	}
//}
//void CurvePoint::Fire::ChangeMode(FIRE_MODE m)
//{
//	if (m_pMode)delete m_pMode;
//
//	switch (m)
//	{
//	case FIRE_MODE::MAKI:
//		m_pMode = new Mode::Maki(this, m_pImage);
//		break;
//
//	case FIRE_MODE::MOEHAJIME:
//		m_pMode = new Mode::Moehajime(this, m_pImage);
//		break;
//
//	case FIRE_MODE::MOETEKITA:
//		m_pMode = new Mode::Moetekita(this, m_pImage);
//		break;
//
//	case FIRE_MODE::MOETA:
//		m_pMode = new Mode::Moeta(this, m_pImage);
//		break;
//
//	case FIRE_MODE::ENABLE:
//		m_pMode = new Mode::Enable(this, m_pFireImage);
//		break;
//
//	case FIRE_MODE::HITED:
//		m_pMode = new Mode::Hited(this, m_pImage);
//		break;
//	}
//
//	// ���[�h���ς�鎞�Ԑݒ�
//	m_pMode->SetChangeModeTime(g_FireModeChangeTime[(int)m]);			// ���[�h���ς���Ă�������);
//}