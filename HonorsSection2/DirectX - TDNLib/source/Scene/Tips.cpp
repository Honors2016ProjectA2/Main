#include "Tips.h"
#include	"../system/FadeCtrl.h"
#include  "UI\UIManager.h"

/********************/
//	ヒントシーン
/********************/
Tips::Tips()
{
	m_state = STATE::START;

	// タイプス
	m_typs[TIPS_TYPE::MEAT ] = new tdn2DAnim("DATA/tips/MEAT.png");
	m_typs[TIPS_TYPE::MEAT_TYPE ] = new tdn2DAnim("DATA/tips/MEAT_TYPE.png");
	m_typs[TIPS_TYPE::HOUSE ] = new tdn2DAnim("DATA/tips/HOUSE.png");
	m_typs[TIPS_TYPE::KUSA_GAGE ] = new tdn2DAnim("DATA/tips/KUSA_GAGE.png");


	for (int i = 0; i < TIPS_TYPE::END ; i++)
	{
		//m_typs[i] = new tdn2DAnim("DATA/tips/tips.png");
		m_typs[i]->OrderGrow(24, 0.5f, 0.5f / 24.0f);
	}
	
	//m_typs->OrderMoveAppeared(18, 1280, 0);
	m_selectTips = TIPS_TYPE::MEAT;

	//m_MovieList.push_back(new MovieInfo("DATA/tips/title.wmv", Vector2(0, 0)));
}

Tips::~Tips()
{
	for (int i = 0; i < TIPS_TYPE::END; i++)
	{
		SAFE_DELETE(m_typs[i]);
	}

	// 動画開放
	for (auto it : m_MovieList)delete it;
}

void Tips::Init()
{
	m_state = STATE::START;
	m_selectTips = TIPS_TYPE::MEAT;
}

bool Tips::Update()
{
	// 更新
	m_typs[m_selectTips]->Update();

	// 動画たち更新
	for (auto &it : m_MovieList)it->Update();
	
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
		FadeControl::Setting(FadeControl::MODE::FADE_IN, 30.0f);
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
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xff000000);// 黒

	// タイプス
	m_typs[m_selectTips]->Render(0, 0);

	// 動画たち描画
	for (auto &it : m_MovieList) it->Render();
}


// 状況に合わせて絵を変える
void Tips::TipsSelect()
{
	// 焼いていなかったら
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
