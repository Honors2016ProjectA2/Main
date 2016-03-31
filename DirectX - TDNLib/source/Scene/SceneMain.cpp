#include	"TDNLIB.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "sceneMainState.h"
#include "../IconButton/IconButton.h"
#include "Animation\AnimationUV.h"
#include "SceneSelect.h"
#include "../UI/UI.h"
#include "../Fade/Fade.h"

//******************************************************************
//		�O���[�o���ϐ�
//******************************************************************
//GossipRipple* rip;
iexMesh* school;
tdn2DObj *lpGameOver, *lpGameClear;

// ����A�N�Ɏ�������ׂ���������Ȃ��̂ł����ɂ����Ƃ�
int RippleCount(0);


//******************************************************************
//		�������E���
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
	m_pStateMachine->SetGlobalState(sceneMainGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(sceneMainGame::GetInstance());

	m_pButtonMgr = new IconButtonManager;
	m_pButtonMgr->TextLoad("DATA/Text/IconButton/main.txt");
	m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::YES, EN_DIS_TYPE::DISABLE_VANISH);
	m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::NO, EN_DIS_TYPE::DISABLE_VANISH);

	lpGameClear = new tdn2DObj("DATA/UI/game_clear.png");
	lpGameOver = new tdn2DObj("DATA/UI/game_over.png");

	UIMgr.Initialize();

	Fade::Set(Fade::FLAG::FADE_IN, 6, 0x00000000, 255, 0);
	m_mode = MODE::MAIN;

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

	delete lpGameClear;
	delete lpGameOver;

	UIMgr.Release();
}


//******************************************************************
//		�X�V�E�`��
//******************************************************************
bool sceneMain::Update()
{
	tdnMouse::Update();

	// �X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �{�^���X�V
	m_pButtonMgr->Update(tdnMouse::GetPos());

	// UI�}�l�[�W���[�X�V
	UIMgr.Update();

	Fade::Update();

	if (m_mode != MODE::MAIN)
	{
		if (Fade::alpha == 0)
		{
			Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		}
	}

	if (tdnMouse::GetLeft() == 3)
	{
		if (m_mode == MODE::GAMECLEAR)
		{
			MainFrame->ChangeScene(new sceneSelect);
			//switch ((BUTTON_ID)m_pButtonMgr->GetInButtonNo())
			//{
			//case BUTTON_ID::YES:
			//{
			//					   extern Framework *MainFrame;
			//					   MainFrame->ChangeScene(new sceneMain);
			//					   return true;
			//}
			//	break;
			//
			//case BUTTON_ID::NO:
			//{
			//					  extern Framework *MainFrame;
			//					  MainFrame->ChangeScene(new sceneSelect);
			//					  return true;
			//}
			//	break;
			//}
		}
		else if (m_mode == MODE::GAMEOVER)
		{
			MainFrame->ChangeScene(new sceneMain);
			//switch ((BUTTON_ID)m_pButtonMgr->GetInButtonNo())
			//{
			//case BUTTON_ID::YES:
			//{
			//					   extern Framework *MainFrame;
			//					   MainFrame->ChangeScene(new sceneMain);
			//					   return true;
			//}
			//	break;
			//
			//case BUTTON_ID::NO:
			//{
			//					  extern Framework *MainFrame;
			//					  MainFrame->ChangeScene(new sceneSelect);
			//					  return true;
			//}
			//	break;
			//}
		}
		else if (m_pButtonMgr->GetInButtonNo() == 2)
		{
			MainFrame->ChangeScene(new sceneMain);
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

	UIMgr.Render();

	Fade::Render();

	if (m_mode == MODE::GAMECLEAR)
	{
		//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);

		if (Fade::alpha == 128)
		{
			tdnText::Draw(580, 380, 0xffffffff, "�N���b�N�Ői��");
			lpGameClear->Render(160, 160);
		}
	}
	else if (m_mode == MODE::GAMEOVER)
	{
		//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);

		if (Fade::alpha == 128)
		{
			tdnText::Draw(580, 380, 0xffffffff, "�N���b�N�ł�����x�v���C");
			lpGameOver->Render(160, 160);
		}
	}

	m_pButtonMgr->Render();
}
