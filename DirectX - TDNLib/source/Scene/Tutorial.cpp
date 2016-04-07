#include "TDNLIB.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "../AI/Message/Message.h"
#include "../Stage/Stage.h"
#include "sceneMainState.h"
#include "../IconButton/IconButton.h"
#include "../UI/UI.h"
#include "../Sound/SoundManager.h"
#include "../Fade/Fade.h"
#include "../JudgeManager/JudgeManager.h"
#include "Tutorial.h"

// sceneMainのグローバル変数から
extern int RippleCount;


//*****************************************************************************************************************************
//			チュートリアル
//*****************************************************************************************************************************
//------------------------------------------------------
//	第1
//------------------------------------------------------
void TutorialContents::One::Initialize(sceneMain *pMain)
{
	// ステージの番号に応じて人間を読み込む
	//RippleCount = StageMgr.LoadPerson();

	// ボタンの有効・無効化
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_BLACK);
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::RED, EN_DIS_TYPE::DISABLE_BLACK);
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::GREEN, EN_DIS_TYPE::DISABLE_BLACK);
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::START, EN_DIS_TYPE::DISABLE_BLACK);
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::RETRY, EN_DIS_TYPE::DISABLE_BLACK);
}

TutorialContents::One::~One()
{

}

bool TutorialContents::One::Update(sceneMain *pMain)
{
	switch (step)
	{
	case 0:
		if (tdnMouse::GetLeft() == 3)
		{
			step++;
		}
		break;

	case 1:
		if (tdnMouse::GetLeft() == 3)
		{
			step++;

			// 青い設置ボタンを有効に
			pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
		}
		break;

	case 2:
		if (tdnMouse::GetLeft() == 3)
		{
			if (pMain->GetButtonMgr()->GetInButtonNo() == BUTTON_ID::BLUE)
			{
				pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_WHITE);
				m_HoldMesh = PersonMgr.GetPerson(0)->GetObj();
				step++;
			}
		}
		break;

	case 3:
		if (tdnMouse::GetLeft() == 3)
		{
			step++;
		}
		break;
	case 4:
		m_HoldMeshPos = Math::ScreenToWorldPlate(tdnMouse::GetPos());
		if (tdnMouse::GetLeft() == 3)
		{
			if (m_HoldMeshPos.LengthSq() < 5 * 5)
			{
				PersonMgr.AddPerson(PERSON_TYPE::WAIT, m_HoldMeshPos);
				RippleCount--;
				step++;
			}
		}
		break;

	case 5:
		if (tdnMouse::GetLeft() == 3)
		{
			step++;

			// スタートボタンを有効に
			pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::START, EN_DIS_TYPE::ENABLE);

			// 終了
			//return false;
		}
		break;

	case 6:
		if (tdnMouse::GetLeft() == 3)
		{
			if (pMain->GetButtonMgr()->GetInButtonNo() == BUTTON_ID::START)
			{
				// ★波紋流しスタート！
				PersonMgr.StartGossip();

				// ボタン無効にする！
				pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::START, EN_DIS_TYPE::DISABLE_BLACK);
				pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_BLACK);

				// ステートを波紋流れてるのを見るだけのステートにする
				pMain->GetFSM()->ChangeState(sceneMainGossip::GetInstance());
			}
		}
		break;
	}
	return true;
}

void TutorialContents::One::Render(sceneMain *pMain)
{
	if (m_HoldMesh)
	{
		m_HoldMesh->SetPos(m_HoldMeshPos);
		m_HoldMesh->Update();
		m_HoldMesh->Render(RS::COPY, .5f);
	}
}

void TutorialContents::One::Render2D(sceneMain *pMain)
{
	switch (step)
	{
	case 0:
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
		tdnText::Draw(640, 320, 0xffffffff, "テキスト1再生");
		break;

	case 1:
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
		tdnText::Draw(640, 320, 0xffffffff, "テキスト2再生");
		break;

	case 2:

		break;

	case 3:
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
		tdnText::Draw(640, 320, 0xffffffff, "テキスト3再生");
		break;

	case 4:
		if (m_HoldMeshPos.LengthSq() < 5 * 5)
		{
			tdnText::Draw(600, 380, 0xff00ffff, "設置OK！");
		}
		break;

	case 5:
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
		tdnText::Draw(640, 320, 0xffffffff, "テキスト5再生");
		break;
		break;
	}
}

//------------------------------------------------------
//	第2
//------------------------------------------------------
void TutorialContents::Two::Initialize(sceneMain *pMain)
{

}

TutorialContents::Two::~Two()
{

}

bool TutorialContents::Two::Update(sceneMain *pMain)
{
	return true;
}

void TutorialContents::Two::Render(sceneMain *pMain)
{

}

void TutorialContents::Two::Render2D(sceneMain *pMain)
{

}

//------------------------------------------------------
//	第3
//------------------------------------------------------
void TutorialContents::Three::Initialize(sceneMain *pMain)
{

}

TutorialContents::Three::~Three()
{

}

bool TutorialContents::Three::Update(sceneMain *pMain)
{
	return true;
}

void TutorialContents::Three::Render(sceneMain *pMain)
{

}

void TutorialContents::Three::Render2D(sceneMain *pMain)
{

}

//*****************************************************************************************************************************
//			チュートリアル管理
//*****************************************************************************************************************************

TutorialManager::~TutorialManager()
{
	if (m_pTutorial)
	{
		delete m_pTutorial, m_pTutorial = nullptr;
	}
}

// 入り口
void TutorialManager::Initialize(sceneMain *pMain, int StageNo)
{
	if (m_pTutorial)delete m_pTutorial;

	// ステージの番号に応じてそれぞれのチュートリアルを持たせる
	switch (StageNo)
	{
	case 0:
		m_pTutorial = new TutorialContents::One;
		break;
	case 1:
		m_pTutorial = new TutorialContents::Two;
		break;
	case 2:
		m_pTutorial = new TutorialContents::Three;
		break;
	default:
		MyAssert(0, "例外: チュートリアルのステージ番号");
		break;
	}
	m_pTutorial->Initialize(pMain);
}

// 実行中
bool TutorialManager::Update(sceneMain *pMain)
{
	GossipRippleMgr.Update();

	PersonMgr.Update();

	return m_pTutorial->Update(pMain);
}

void TutorialManager::Render(sceneMain *pMain)
{
	m_pTutorial->Render(pMain);
}

void TutorialManager::Render2D(sceneMain *pMain)
{
	m_pTutorial->Render2D(pMain);
}