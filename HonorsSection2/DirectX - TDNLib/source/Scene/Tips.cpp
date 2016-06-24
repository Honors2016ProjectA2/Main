#include "Tips.h"
#include	"../system/FadeCtrl.h"
#include  "UI\UIManager.h"
#include "../UI/ResultUIManager.h"
#include "../TipsCount/TipsCounter.h"

/********************/
//	ヒントシーン
/********************/
Tips::Tips() : m_typs(nullptr), m_pTipsMovie(nullptr)
{
	m_state = STATE::START;

	// タイプス
	//m_typs[TIPS_TYPE::MEAT ] = new tdn2DAnim("DATA/tips/MEAT.png");
	//m_typs[TIPS_TYPE::MEAT_TYPE ] = new tdn2DAnim("DATA/tips/MEAT_TYPE.png");
	//m_typs[TIPS_TYPE::HOUSE ] = new tdn2DAnim("DATA/tips/HOUSE.png");
	//m_typs[TIPS_TYPE::KUSA_GAGE ] = new tdn2DAnim("DATA/tips/KUSA_GAGE.png");
	m_paths[TIPS_TYPE::GISEINAKUSHITE] = "DATA/tips/gisei.wmv";
	m_paths[TIPS_TYPE::MINNATIGATTE] = "DATA/tips/minnna.wmv";
	m_paths[TIPS_TYPE::TO_S_ROAD] = "DATA/tips/to_s.wmv";
	m_paths[TIPS_TYPE::TO_SS_ROAD] = "DATA/tips/to_ss.wmv";
	m_paths[TIPS_TYPE::NANIMOIUMAI] = "DATA/tips/iumai.png";
	m_paths[TIPS_TYPE::OK_BOKUSOU] = "DATA/tips/ok.wmv";

	for (int i = 0; i < TIPS_TYPE::END ; i++)
	{
		//m_typs[i] = new tdn2DAnim("DATA/tips/tips.png");
		//m_typs[i]->OrderGrow(24, 0.5f, 0.5f / 24.0f);
	}
	
	//m_typs->OrderMoveAppeared(18, 1280, 0);
	m_selectTips = TIPS_TYPE::GISEINAKUSHITE;

	//m_MovieList.push_back(new MovieInfo("DATA/tips/title.wmv", Vector2(0, 0)));
}

Tips::~Tips()
{
	SAFE_DELETE(m_typs);
	SAFE_DELETE(m_pTipsMovie);

	// 動画開放
	//for (auto it : m_MovieList)delete it;
}

void Tips::Init()
{
	m_state = STATE::START;
	m_selectTips = TIPS_TYPE::GISEINAKUSHITE;
}

bool Tips::Update()
{
	// 更新
	if(m_typs)m_typs->Update();

	// 動画更新
	if(m_pTipsMovie)m_pTipsMovie->LoopUpdate();
	
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
		assert(m_typs);
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
	if(m_typs)m_typs->Render(0, 0);

	// 動画たち描画
	//for (auto &it : m_MovieList) it->Render();
}


// 状況に合わせて絵を変える
void Tips::TipsSelect()
{
	// もう何も言うまい
	if (RESULT_UIMNG.m_rankType == ResultUIManager::SS)
	{
		m_selectTips = TIPS_TYPE::NANIMOIUMAI;
	}

	// SSへの道
	else if (RESULT_UIMNG.m_rankType == ResultUIManager::S)
	{
		m_selectTips = TIPS_TYPE::TO_SS_ROAD;
	}

	// 犠牲なくして
	else if (UIMNG.GetWorfHappyCount() == 0)
	{
		m_selectTips = TIPS_TYPE::GISEINAKUSHITE;
	}

	// みんなちがってみんないい
	else if (TipsCountMgr->m_NikuSheepTypes[(int)SHEEP_TYPE::GOLD] <= 1 && TipsCountMgr->m_NikuSheepTypes[(int)SHEEP_TYPE::REAL] <= 1)
	{
		m_selectTips = TIPS_TYPE::MINNATIGATTE;
	}

	// Sランクへの道
	else if (RESULT_UIMNG.m_rankType == ResultUIManager::A)
	{
		m_selectTips = TIPS_TYPE::TO_S_ROAD;
	}
	else m_selectTips = TIPS_TYPE::OK_BOKUSOU;

	/* ここでtypsアニメの初期化を行う */

	// 何も言うまいじゃなかったら
	if (m_selectTips != TIPS_TYPE::NANIMOIUMAI)
	{
		// 動画
		m_pTipsMovie = new tdnMovie((char*)m_paths[m_selectTips].c_str());
		m_pTipsMovie->Play();

		// 動画からテクスチャ取得して2DAnim初期化
		m_typs = new tdn2DAnim(m_pTipsMovie->GetTexture());
	}
	else
	{
		// 1枚絵
		m_typs = new tdn2DAnim((char*)m_paths[TIPS_TYPE::NANIMOIUMAI].c_str());
	}

	// アニメ設定
	m_typs->OrderGrow(24, 0.5f, 0.5f / 24.0f);
}
