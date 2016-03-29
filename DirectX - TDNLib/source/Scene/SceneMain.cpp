#include	"TDNLIB.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "../Part/Part.h"
#include "../IconButton/IconButton.h"
#include "Animation\AnimationUV.h"
#include "SceneSelect.h"

//GossipRipple* rip;

iexMesh* school;


//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneMain::Initialize()
{
	tdnView::Init();
	tdnMouse::Initialize(TRUE);
	tdnView::Set(Vector3(.0f,75.0f,-60.0f), Vector3(.0f, .0f, .0f));

	//rip = new GossipRipple(RIPPLE_TYPE::RED, Vector3(-20, 0, 20));

	GossipRippleMgr;

	school = new iexMesh("Data/Stage/school.imo");
	school->SetScale(0.8f);
	school->SetAngle(PI);
	school->Update();

	m_pStateMachine = new StateMachine<sceneMain>(this);
	m_pStateMachine->SetGlobalState(sceneMainGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(sceneMainGame::GetInstance());

	m_pButtonMgr = new IconButtonManager;
	m_pButtonMgr->TextLoad("DATA/Text/IconButton/main.txt");
	m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::YES, EN_DIS_TYPE::DISABLE_VANISH);
	m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::NO, EN_DIS_TYPE::DISABLE_VANISH);
	return true;
}

sceneMain::~sceneMain()
{
	//delete rip;
	GossipRippleMgr.Release();

	PersonMgr.Reset();
	PersonMgr.Release();
	delete m_pButtonMgr;
	delete school;
	delete m_pStateMachine;
}


//******************************************************************
//		更新・描画
//******************************************************************
bool sceneMain::Update()
{
	tdnMouse::Update();

	// ステートマシン更新
	m_pStateMachine->Update();

	// ボタン更新
	m_pButtonMgr->Update(tdnMouse::GetPos());

	if (tdnMouse::GetLeft() == 3)
	{
		if (m_mode == MODE::GAMECLEAR)
		{
			switch ((BUTTON_ID)m_pButtonMgr->GetInButtonNo())
			{
			case BUTTON_ID::YES:
			{
								   extern Framework *MainFrame;
								   MainFrame->ChangeScene(new sceneMain);
								   return true;
			}
				break;

			case BUTTON_ID::NO:
			{
								  extern Framework *MainFrame;
								  MainFrame->ChangeScene(new sceneSelect);
								  return true;
			}
				break;
			}
		}
		else if (m_mode == MODE::GAMEOVER)
		{
			switch ((BUTTON_ID)m_pButtonMgr->GetInButtonNo())
			{
			case BUTTON_ID::YES:
			{
								   extern Framework *MainFrame;
								   MainFrame->ChangeScene(new sceneMain);
								   return true;
			}
				break;

			case BUTTON_ID::NO:
			{
								  extern Framework *MainFrame;
								  MainFrame->ChangeScene(new sceneSelect);
								  return true;
			}
				break;
			}
		}
	}

	return true;	
}

void sceneMain::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);
	
	school->Render();

	//rip->Render();
	GossipRippleMgr.Render();

	PersonMgr.Render();

	if (m_mode == MODE::GAMECLEAR)
	{
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
		tdnText::Draw(320, 320, 0xffffffff, "ゲームクリア");
	}
	else if (m_mode == MODE::GAMEOVER)
	{
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
		tdnText::Draw(320, 320, 0xffffffff, "ゲームオーバー");
	}
	tdnText::Draw(150, 0, 0xffffffff, "シーンメイン");

	m_pButtonMgr->Render();

	//tdnText::Draw(320, 30, 0xffffffff, "残り回数 : %d", RippleCount);
}
