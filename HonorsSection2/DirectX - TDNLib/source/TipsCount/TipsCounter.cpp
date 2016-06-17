#include "TDNLIB.h"
#include "TipsCounter.h"
#include "../UI/UIManager.h"
#include "../Sheep/Sheep.h"
#include "../Niku/Niku.h"

TipsCounter::TipsCounter()
{
	// �z��̊m��(�Ȃ�.h�Ŕz��錾���Ȃ����Ƃ����ƁA.h�ɃC���N���[�h�������Ȃ���������)
	m_NikuSheepTypes = new int[(int)SHEEP_TYPE::MAX];
	m_YakinikuSteps = new int[(int)YAKINIKU_MODE::MAX];

	// �I�[��0
	Reset();
}

TipsCounter::~TipsCounter()
{
	// �ϔz��|�C���^�̊J��
	delete[] m_NikuSheepTypes;
	delete[] m_YakinikuSteps;
}

void TipsCounter::Reset()
{
	// UI�}�l�[�W���[�̐��l�̃A�h���X���Q�Ƃ���B
	m_ReferenceUI.pScore = UIMNG.GetScoreAddress();
	m_ReferenceUI.pWolfKill = UIMNG.GetWolfHappyCountAddress();
	m_ReferenceUI.pSheepKill = UIMNG.GetSheepKillCountAddress();

	m_DogUse = m_LaneChange = 0;
	memset(m_NikuSheepTypes, 0, sizeof(int) * (int)SHEEP_TYPE::MAX);
	memset(m_YakinikuSteps, 0, sizeof(int) * (int)YAKINIKU_MODE::MAX);
}