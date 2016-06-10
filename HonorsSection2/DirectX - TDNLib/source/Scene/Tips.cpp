#include "Tips.h"
#include	"../system/FadeCtrl.h"

/********************/
//	ヒントシーン
/********************/
Tips::Tips()
{
	m_state = STATE::START;
	m_typs = new tdn2DObj("DATA/tips/tips.png");
}

Tips::~Tips()
{
	SAFE_DELETE(m_typs);
}

void Tips::Init()
{
	m_state = STATE::START;
}

bool Tips::Update()
{
	// 
	if (KeyBoard(KB_ENTER) == 3)
	{
		return true;
	}
	if (tdnMouse::GetLeft() == 3)
	{
		return true;
	}

	switch (m_state)
	{
	case Tips::STATE::START:
		FadeControl::Setting(FadeControl::MODE::FADE_IN, 30.0f);
		
		m_state = STATE::EXECUTE;

		break;
	case Tips::STATE::EXECUTE:
		
		break;
	case Tips::STATE::END:
		
		break;
	default:
		break;
	}

	return false;
}

void Tips::Render()
{
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xff000000);// 黒

	// タイプス
	m_typs->Render(0, 0);
}
