#include "TDNLIB.h"
#include "CurvePoint.h"
#include "../Sound/SoundManager.h"

//**************************************************
//    CurvePoint class
//**************************************************

//------- constructor,destructor ---------

CurvePoint::CurvePoint(tdn2DObj* image, Vector2 pos, DIR dir) :
W(199), H(199), SX(0), SY(0), SW(256), SH(256),
SPEED_Y(16), LOCK_TIME(600), WAIT_TIME(30),
image(image), pos(pos), m_dir(dir), m_bCursorIn(false)
{
	image = nullptr;
	pos = Vector2(0, 0);
#ifdef _SPIC2015_SHUTTER_TYPE_ANIMATION_
	frame = 0;
#else
	height = H;
#endif
	state = CurvePointState::CLOSE;
	waitTimer = 0;
	timer = 0;
	speed = 0;
}

CurvePoint::~CurvePoint()
{

}

//----------- public method ------------

void CurvePoint::Update()
{
	switch (state)
	{
	case CurvePointState::OPEN:

		break;
	case CurvePointState::CLOSE:

		break;
	}

}

void CurvePoint::Render()
{
	//if (image == nullptr)return;

#ifdef _SPIC2015_SHUTTER_TYPE_ANIMATION_
	image->Render(param.dpos.x, param.dpos.y, param.dw, param.dh,
		param.spos.x, param.spos.y, param.sw, param.sh);
#else
	image->SetARGB((BYTE)this->IsOpening() ? 255 : 96, 255, 255, 255);
	image->Render((int)pos.x, (int)pos.y);
#endif
}

void CurvePoint::SetPos(int y)
{
	pos.y = (float)y;
}

void CurvePoint::Open()
{
	if (height <= SH - SH * 9 / 10)return;

	state = CurvePointState::OPEN;
	speed = SPEED_Y;
	timer = 0;
	waitTimer = 0;
}

void CurvePoint::Close()
{
	if (height >= SH)return;

	if (state != CurvePointState::CLOSE)
		speed = 0;

	state = CurvePointState::CLOSE;
	timer = 0;
	waitTimer = 0;
}

void CurvePoint::Stop()
{
	if (timer < 20)
	{
		timer++;
		return;
	}
	if (height >= SH - SH * 8 / 10)
	{
		if (height >= SH)return;
		state = CurvePointState::CLOSE;
		waitTimer = 0;
	}
	else
	{
		if (height <= SH - SH * 9 / 10)return;
		speed = SPEED_Y;
		state = CurvePointState::OPEN;
	}
}

bool CurvePoint::IsOpening()
{
	return (state == CurvePointState::OPEN);
}

int CurvePoint::GetWidth()
{
	return W;
}