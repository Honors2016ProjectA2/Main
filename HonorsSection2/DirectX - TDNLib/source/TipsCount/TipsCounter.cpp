#include "TDNLIB.h"
#include "TipsCounter.h"
#include "../UI/UIManager.h"
#include "../Sheep/Sheep.h"
#include "../Niku/Niku.h"

TipsCounter::TipsCounter()
{
	// 配列の確保(なぜ.hで配列宣言しないかというと、.hにインクルードしたくなかったから)
	m_NikuSheepTypes = new int[(int)SHEEP_TYPE::MAX];
	m_YakinikuSteps = new int[(int)YAKINIKU_MODE::MAX];

	// オール0
	Reset();
}

TipsCounter::~TipsCounter()
{
	// 可変配列ポインタの開放
	delete[] m_NikuSheepTypes;
	delete[] m_YakinikuSteps;
}

void TipsCounter::Reset()
{
	// UIマネージャーの数値のアドレスを参照する。
	m_ReferenceUI.pScore = UIMNG.GetScoreAddress();
	m_ReferenceUI.pWolfKill = UIMNG.GetWolfHappyCountAddress();
	m_ReferenceUI.pSheepKill = UIMNG.GetSheepKillCountAddress();

	m_DogUse = m_LaneChange = 0;
	memset(m_NikuSheepTypes, 0, sizeof(int) * (int)SHEEP_TYPE::MAX);
	memset(m_YakinikuSteps, 0, sizeof(int) * (int)YAKINIKU_MODE::MAX);
}