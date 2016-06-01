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
////    牧草委譲クラス
////**************************************************
//void CurvePoint::Fire::Mode::Base::Update()
//{
//	// 設定時間過ぎたら
//	if (++m_timer > m_limit)
//	{
//		NextMode();	// 次のモードへ
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
//	// 次のモードに変更
//	pFire->ChangeMode(m_NextMode);
//}
//
////===========================================================
////		まきの状態
////===========================================================
//CurvePoint::Fire::Mode::Maki::Maki(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// 燃え始めにいく
//	m_NextMode = FIRE_MODE::MOEHAJIME;
//
//	// 画像座標
//	m_srcX = 0;
//}
////void CurvePoint::Fire::Mode::Hutaba::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
////===========================================================
////		燃え始め状態
////===========================================================
//CurvePoint::Fire::Mode::Moehajime::Moehajime(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// 燃えてきたにいく
//	m_NextMode = FIRE_MODE::MOETEKITA;
//
//	// 画像座標
//	m_srcX = 128;
//
//	// SEの再生
//	se->Play("点火", me->GetPos());
//}
////void CurvePoint::Fire::Mode::Honba::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
////===========================================================
////		燃えてきた状態
////===========================================================
//CurvePoint::Fire::Mode::Moetekita::Moetekita(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// 燃えたに行く
//	m_NextMode = FIRE_MODE::MOETA;
//
//	// 画像座標
//	m_srcX = 256;
//
//	// SEの再生
//	se->Play("燃えてきた", me->GetPos());
//}
////void CurvePoint::Fire::Mode::Tsubomi::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
//
////===========================================================
////		もえた―
////===========================================================
//CurvePoint::Fire::Mode::Moeta::Moeta(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// そして燃え盛った
//	m_NextMode = FIRE_MODE::ENABLE;
//
//	// 画像座標
//	m_srcX = 128 * 3;
//
//	// SEの再生
//	se->Play("燃えてきた", me->GetPos());
//
//}
////void CurvePoint::Fire::Mode::Saita::Update(Fire *pFire)
////{
////	NextMode(pFire);
////}
//
////===========================================================
////		最大火力
////===========================================================
//CurvePoint::Fire::Mode::Enable::Enable(Fire *me, tdn2DObj *image) :Base(me, image), m_KiraKiraCoolTime(0)
//{
//	// 画像座標
//	m_srcX = 0;
//
//	// SEの再生
//	se->Play("燃え最終", me->GetPos());
//
//	// エフェクト発動
//	EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PLUS);
//}
//void CurvePoint::Fire::Mode::Enable::Update()
//{
//	// きらきらパーティクル
//	//if (++m_KiraKiraCoolTime > 4)
//	//{
//	//	m_KiraKiraCoolTime = 0;
//	//	Particle2dManager::Effect_KiraKira(pFire->GetCenterPos(), Vector2(56, 56), 15.0f, 10.0f);
//	//}
//}
//
////===========================================================
////		ヒット後
////===========================================================
//CurvePoint::Fire::Mode::Hited::Hited(Fire *me, tdn2DObj *image) :Base(me, image)
//{
//	// 画像座標
//	m_srcX = 0;
//}
//void CurvePoint::Fire::Mode::Hited::Update()
//{
//	// 設定時間過ぎたら
//	if (++m_timer > m_limit)
//	{
//		pFire->Change();	// 閉める
//	}
//}
//
////**************************************************
////    牧草クラス
////**************************************************
//
////===========================================================
////		初期化・解放
////===========================================================
//CurvePoint::Fire::Fire(StageManager *pMgr, tdn2DObj *image, Vector2 pos, DIR dir, int ID, int floor) :Base(pMgr, image, pos, dir, ID, floor), m_pMode(nullptr), m_pFireImage(new tdn2DObj("DATA/牧草/炎.png"))
//{
//	// モード初期化(薪から開始)
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
////		更新
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
////		描画
////===========================================================
//void CurvePoint::Fire::Render()
//{
//	if (IsOpening())m_pMode->Render();
//	else if (m_pMgr->m_FireSelect && !m_pMgr->GetDog(m_floor, m_ID)->IsOpening())
//	{
//		// 半透明描画
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
//		m_pMgr->m_FireStock++;	// ストック回復
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
//	// モードが変わる時間設定
//	m_pMode->SetChangeModeTime(g_FireModeChangeTime[(int)m]);			// モードが変わっていく時間);
//}