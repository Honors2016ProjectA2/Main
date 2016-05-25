#include "TDNLIB.h"
#include "CurvePoint.h"
#include "../Sound/SoundManager.h"

//**************************************************
//    CurvePoint class
//**************************************************

//------- constructor,destructor ---------

CurvePoint::CurvePoint(tdn2DObj* image, Vector2 pos, DIR dir) :
W(120), H(120), SW(120), SH(120),
LOCK_TIME(600),
image(image), pos(pos), m_dir(dir), m_bCursorIn(false), m_AnimFrame(0), m_AnimPanel(0)
{
	state = CurvePointState::CLOSE;
}

CurvePoint::~CurvePoint()
{

}

//----------- public method ------------

void CurvePoint::Update()
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

void CurvePoint::Render()
{
	image->SetARGB((BYTE)this->IsOpening() ? 255 : 96, 255, 255, 255);
	image->Render((int)pos.x, (int)pos.y, W, H, m_AnimPanel*SW, 0, SW, SH);
}

void CurvePoint::SetPos(int y)
{
	pos.y = (float)y;
}

bool CurvePoint::IsOpening()
{
	return (state == CurvePointState::OPEN);
}

int CurvePoint::GetWidth()
{
	return W;
}