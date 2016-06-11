#include "Tips.h"
#include	"../system/FadeCtrl.h"

/********************/
//	ヒントシーン
/********************/
Tips::Tips()
{
	m_state = STATE::START;
	m_typs = new tdn2DAnim("DATA/tips/tips.png");
	//m_typs->OrderMoveAppeared(18, 1280, 0);
	m_typs->OrderGrow(16, 0.5f, 0.5f / 16.0f);
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
	// 更新
	m_typs->Update();
	
	// 
	if (KeyBoard(KB_ENTER) == 3)
	{
		return true;
	}
	//if (tdnMouse::GetLeft() == 3)
	//{
	//	return true;
	//}

	switch (m_state)
	{
	case Tips::STATE::START:
		FadeControl::Setting(FadeControl::MODE::FADE_IN, 5.0f);
		m_state = STATE::EXECUTE;
		m_typs->Action();

		break;
	case Tips::STATE::EXECUTE:
		if (FadeControl::IsFade()) return false;

		if (tdnMouse::GetLeft() == 3)
		{
			m_state = STATE::END;
			FadeControl::Setting(FadeControl::MODE::FADE_OUT, 25.0f);
		}

		break;
	case Tips::STATE::END:
		if (FadeControl::IsFade()) return false;
		if (FadeControl::IsEndFade())
		{
			return true;
		}

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
