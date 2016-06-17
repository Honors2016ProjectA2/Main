#include "Tips.h"
#include	"../system/FadeCtrl.h"
#include  "UI\UIManager.h"

/********************/
//	�q���g�V�[��
/********************/
Tips::Tips()
{
	m_state = STATE::START;

	// �^�C�v�X
	m_typs[TIPS_TYPE::MEAT ] = new tdn2DAnim("DATA/tips/MEAT.png");
	m_typs[TIPS_TYPE::MEAT_TYPE ] = new tdn2DAnim("DATA/tips/MEAT_TYPE.png");
	m_typs[TIPS_TYPE::HOUSE ] = new tdn2DAnim("DATA/tips/HOUSE.png");
	m_typs[TIPS_TYPE::KUSA_GAGE ] = new tdn2DAnim("DATA/tips/KUSA_GAGE.png");


	for (int i = 0; i < TIPS_TYPE::END ; i++)
	{
		//m_typs[i] = new tdn2DAnim("DATA/tips/tips.png");
		m_typs[i]->OrderGrow(16, 0.5f, 0.5f / 16.0f);
	}
	
	//m_typs->OrderMoveAppeared(18, 1280, 0);
	m_selectTips = TIPS_TYPE::MEAT;


}

Tips::~Tips()
{
	for (int i = 0; i < TIPS_TYPE::END; i++)
	{
		SAFE_DELETE(m_typs[i]);
	}

}

void Tips::Init()
{
	m_state = STATE::START;
	m_selectTips = TIPS_TYPE::MEAT;
}

bool Tips::Update()
{
	// �X�V
	m_typs[m_selectTips]->Update();
	
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
		m_typs[m_selectTips]->Action();

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
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xff000000);// ��

	// �^�C�v�X
	m_typs[m_selectTips]->Render(0, 0);
}


// �󋵂ɍ��킹�ĊG��ς���
void Tips::TipsSelect()
{
	// �Ă��Ă��Ȃ�������
	if (UIMNG.GetWorfHappyCount() == 0)
	{
		m_selectTips = TIPS_TYPE::MEAT;
	}
	else if (UIMNG.GetWorfHappyCount() < 5)
	{
		m_selectTips = TIPS_TYPE::MEAT_TYPE;
	}
	else
	{
		int ram = rand() % 2;
		if (ram==0)
		{
			m_selectTips = TIPS_TYPE::HOUSE;
		}
		else if (ram == 1)
		{
			m_selectTips = TIPS_TYPE::KUSA_GAGE;
		}

	}
	
}
