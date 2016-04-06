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
#include "../JudgeManager/JudgeManager.h"

//******************************************************************
//		�O���[�o���ϐ�
//******************************************************************
//GossipRipple* rip;
iexMesh* school;

// ����A�N�Ɏ�������ׂ���������Ȃ��̂ł����ɂ����Ƃ�
int RippleCount(0);


//******************************************************************
//		�������E���
//******************************************************************
bool sceneMain::Initialize()
{
	tdnView::Init();
	tdnMouse::Initialize(TRUE);
	tdnView::Set(Vector3(.0f,95.0f,-80.0f), Vector3(.0f, .0f, .0f));

	//rip = new GossipRipple(RIPPLE_TYPE::RED, Vector3(-20, 0, 20));

	GossipRippleMgr;
	JudgeMgr;

	// �X�e�[�W���f��������
	school = new iexMesh("Data/Stage/school.imo");
	school->SetScale(0.8f);
	school->SetAngle(PI);
	school->Update();

	// �{�^���Ǘ����񏉊���
	m_pButtonMgr = new IconButtonManager;
	m_pButtonMgr->TextLoad("DATA/Text/IconButton/main.txt");

	// �{�^���̗L���E����
	m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::YES, EN_DIS_TYPE::DISABLE_VANISH);
	m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::NO, EN_DIS_TYPE::DISABLE_VANISH);

	// �V�[�����C���X�e�[�g�}�V��������
	m_pStateMachine = new StateMachine<sceneMain>(this);
	m_pStateMachine->SetGlobalState(sceneMainGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(sceneMainSetPart::GetInstance());
	g_GameState = GAME_STATE::INTRO;

	// �Q�[���N���A�̉摜������
	//lpGameClear = new tdn2DObj("DATA/UI/game_clear.png");
	//lpGameOver = new tdn2DObj("DATA/UI/game_over.png");

	// UI�Ǘ����񏉊���
	UIMgr.Initialize();

	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 6, 0x00000000, 255, 0);

	return true;
}

sceneMain::~sceneMain()
{
	//delete rip;
	GossipRippleMgr.Release();
	JudgeMgr.Release();
	PersonMgr.Reset();
	PersonMgr.Release();
	delete m_pButtonMgr;
	delete school;
	delete m_pStateMachine;

	//delete lpGameClear;
	//delete lpGameOver;

	UIMgr.Release();
}


//******************************************************************
//		�X�V�E�`��
//******************************************************************
bool sceneMain::Update()
{
	// �}�E�X�X�V
	tdnMouse::Update();

	// �X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �{�^���X�V
	m_pButtonMgr->Update(tdnMouse::GetPos());

	// UI�}�l�[�W���[�X�V
	UIMgr.Update();

	// �t�F�[�h�X�V
	Fade::Update();

	// �Q�[���N���A���Q�[���I�[�o�[��ԂȂ�
	//if (m_mode != MODE::MAIN)
	//{
	//	// �܂��t�F�[�h���ĂȂ��Ƃ�
	//	if (Fade::alpha == 0)
	//	{
	//		// �u���b�N�t�F�[�hON
	//		Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
	//	}
	//}

	// ���N���b�N
	if (tdnMouse::GetLeft() == 3)
	{
		// �Q�[���N���A��ԂȂ�
		if (g_GameState == GAME_STATE::GAME_CLEAR)
		{
			// �Z���N�g��ʂɖ߂�
			MainFrame->ChangeScene(new sceneSelect);
			return true;
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

		// �Q�[���I�[�o�[��ԂȂ�
		else if (g_GameState == GAME_STATE::GAME_OVER)
		{
			// ������x�V�[�����C����ǂݍ���
			MainFrame->ChangeScene(new sceneMain);
			return true;
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

		// ���g���C�{�^��
		else if (m_pButtonMgr->GetInButtonNo() == 2)
		{
			// ������x�ǂݍ���
			MainFrame->ChangeScene(new sceneMain);
			return true;
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
	m_pStateMachine->Render();

	UIMgr.Render();

	// �t�F�[�h��UI�ɏ���(�t�F�[�h�̏�ɉ摜��u�����Ƃ����邩��)
	//Fade::Render();

	//if (m_mode == MODE::GAMECLEAR)
	//{
	//	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
	//}
	//else if (m_mode == MODE::GAMEOVER)
	//{
	//	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
	//}

	m_pButtonMgr->Render();
}
