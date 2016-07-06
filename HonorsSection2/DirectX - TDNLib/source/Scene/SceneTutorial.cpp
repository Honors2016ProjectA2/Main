#include	"SceneTutorial.h"
#include	"Ready.h"
#include	"MousePointer.h"
#include	"../Stage/StageMNG.h"
#include	"../CurvePoint/CurvePoint.h"
#include	"../system/FadeCtrl.h"
#include	"../Sound/SoundManager.h"
#include	"../Collision/Collision.h"
#include	"../Data/DataMNG.h"
#include	"../Sheep/Sheep.h"
#include	"../Enemy/watchman.h"
#include	"../system/system.h"
#include	"../Bokusou/Bokusou.h"
#include	"../Shake/Shake.h"
#include	"../UI/UIManager.h"
#include	"../PostEffect/PostEffect.h"
#include	"../particle_2d/particle_2d.h"
#include	"../Effect/EffectManager.h"
#include	"../Niku/Niku.h"
#include	"../UI\ResultUIManager.h"
#include	"../TipsCount/TipsCounter.h"

#include "../system/Framework.h"
#include "Scene\Title.h"

namespace{
	namespace SCENE
	{
		enum{
			INIT, READY, MAIN
		};
	}
}

//******************************************************************
//		�������E���
//******************************************************************

bool sceneTutorial::Initialize()
{
	// �`���[�g���A���X�e�b�v������k
	m_TutorialStep = 0;
	m_bPause = false;
	m_WaitTimer = 0;

	UIMNG.Init();
	UIMNG.SetTimer(540);
	
	// TipsCount���Z�b�g
	TipsCountMgr->Reset();

	EffectMgr;

	srand(timeGetTime());

	tdnMouse::Initialize(FALSE);
	tdnView::Init();

	// ���[���̕��Ƃ��ǂݍ��ނ̂ŁA�X�e�[�W��^�����new
	stage = new StageManager(true);

	back = new tdn2DObj("DATA/GameHaikei.png");
	ready = new Ready(true);
	pointer = new MousePointer();
	dataMNG = new DataManager();
	g_pSheepMgr = new SheepManager(true);
	//result = new Result2();
	RESULT_UIMNG.Init();

	isResultFlag = true;

	// �G�}�l�[�W������k
	EnemyMgr->Initialize(true);

	tdnSystem::GetDevice()->GetRenderTarget(0, &backUp);
	renderTarget = new tdn2DObj(1280, 720, RENDERTARGET);

	ShakeMgr->Initialize();
	this->Init();

	// �q���}�l�[�W���[������
	BokusouMgr->Initialize(true);

	// ���}�l�[�W���[������
	NikuMgr->Initialize();

	// BGM
	bgm->Play("MAIN");
	bgm->SetVolume("MAIN", 1);

	// �|�X�g�G�t�F�N�g
	PostEffectMgr;

	// �p�[�e�B�N��������
	Particle2dManager::Initialize("DATA/Effect/particle.png", 5000, 4, 4);

	// �S���̃h�A�܂��Ă�I�I
	g_CreateSheepFloor = -1;

	// �|�[�Y
	m_again.pic = new tdn2DAnim("Data/UI/Pose/again.png");
	m_again.pic->OrderShake(12, 0, 18, 4);
	m_again.x = 400;
	m_again.y = 200;
	m_again.isAction = false;
	m_again.rip = new tdn2DAnim("Data/UI/Pose/again.png");
	m_again.rip->OrderRipple(12, 1.0, 0.02f);

	m_stop.pic = new tdn2DAnim("Data/UI/Pose/stop.png");
	m_stop.pic->OrderShake(12, 0, 18, 4);
	m_stop.x = 400;
	m_stop.y = 400;
	m_stop.isAction = false;
	m_stop.rip = new tdn2DAnim("Data/UI/Pose/stop.png");
	m_stop.rip->OrderRipple(12, 1.0, 0.02f);

	m_poseState = POSE_STATE::START;

	//m_again->Action();
	//m_stop->Action();

	m_poseTimer.one = new tdn2DAnim("Data/UI/Pose/1.png");
	m_poseTimer.one->OrderShrink(18, 1.0f, 1.5f);
	m_poseTimer.one->OrderRipple(28, 1.0, 0.01f);

	m_poseTimer.two = new tdn2DAnim("Data/UI/Pose/2.png");
	m_poseTimer.two->OrderShrink(18, 1.0f, 1.5f);
	m_poseTimer.two->OrderRipple(28, 1.0, 0.01f);

	m_poseTimer.three = new tdn2DAnim("Data/UI/Pose/3.png");
	m_poseTimer.three->OrderShrink(18, 1.0f, 1.5f);
	m_poseTimer.three->OrderRipple(28, 1.0, 0.01f);

	m_poseIcon.pic = new tdn2DObj("Data/UI/Pose/poseIcon.png");
	m_poseIcon.x = 1240;
	m_poseIcon.y = 42;
	m_poseIcon.pic->SetARGB(0xffaaaaaa);// �F
	
	return true;
}

sceneTutorial::~sceneTutorial()
{
	SAFE_DELETE(back);
	SAFE_DELETE(ready);
	SAFE_DELETE(pointer);
	SAFE_DELETE(stage);
	SAFE_DELETE(dataMNG);
	SAFE_DELETE(g_pSheepMgr);
	EnemyMgr->Release();
	RESULT_UIMNG.Release();
	SAFE_DELETE(renderTarget);
	BokusouMgr->Release();
	PostEffectMgr.Release();
	Particle2dManager::Release();
	UIMNG.Release();
	EffectMgr.Release();
	NikuMgr->Release();

	SAFE_DELETE(m_stop.pic);
	SAFE_DELETE(m_again.pic);
	SAFE_DELETE(m_stop.rip);
	SAFE_DELETE(m_again.rip);

	SAFE_DELETE(m_poseTimer.one);
	SAFE_DELETE(m_poseTimer.two);
	SAFE_DELETE(m_poseTimer.three);

	SAFE_DELETE(m_poseIcon.pic);
}

//******************************************************************
//		����
//******************************************************************

bool sceneTutorial::Update()
{
	tdnMouse::Update();
	pointer->Update();
	FadeControl::Update();
	Particle2dManager::Update();


	/*�@�f�[�^�󂯓n���@*/
	DataDelivery();

	//// PosyEffect
	//PostEffectMgr.Update();

	//// EffectMGR
	//EffectMgr.Update();
	//if (KeyBoard(KB_F) == 3)
	//{
	//	EffectMgr.AddEffect(300, 300, EFFECT_TYPE::NOTICE);
	//}

	switch (state) {
	case SCENE::INIT:		Init();				break;
	case SCENE::READY:		ReadyEvent();		break;
	case SCENE::MAIN:
		if (MainUpdate())MainFrame->ChangeScene(new Title);
		break;
	}

	return true;
}

void sceneTutorial::DataDelivery()
{
	stage->Reflection(dataMNG, pointer);
	pointer->DataReceive(stage);
	g_pSheepMgr->Set_pointers(stage, dataMNG);
	EnemyMgr->Set_Pointers(stage, dataMNG);
	//result->Set_MousePointer(pointer);
}


void sceneTutorial::Init()
{
	stage->Init();
	dataMNG->Init();
	ready->Init();

//	watchman->Init();
//	m_pSheepMgr->Init();

	FadeControl::Setting(FadeControl::MODE::WHITE_IN, 30.0f);

	state = SCENE::READY;
	DataDelivery();
}

void sceneTutorial::ReadyEvent()
{
	// PosyEffect
	PostEffectMgr.Update();

	// EffectMGR
	EffectMgr.Update();

	if( ready->Update() ){
		state = SCENE::MAIN;
		//g_pSheepMgr->Start();
		stage->Reset();
	}
	//stage->Update();
}

bool sceneTutorial::MainUpdate()
{
	switch (m_TutorialStep)
	{
	case 0:	// �Q�[���X�^�[�g���ėr���o�Ă���܂ł̑ҋ@����
	{
				g_pSheepMgr->m_bCatchOK = false;	// �r�͂ލ���I�t
				stage->m_bDogUpdate = false;		// ���̋@�\�I�t
				NikuMgr->m_bClickOK = false;		// ���N���b�N���ďĂ��@�\�I�t
				if (++m_WaitTimer > 24)
				{ 
					m_WaitTimer = 0; 
					m_TutorialStep++; 
					m_bPause = true;
				}
	}
		break;

	case 1:	// �r�����[������o�Ă����I�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++; 
			m_bPause = false;
		}
		break;

	case 2:	// �r�����[���ɓ���܂ł̑ҋ@����
	{
				if (++m_WaitTimer > 110)
				{
					m_WaitTimer = 0; 
					m_TutorialStep++;
					m_bPause = true;
				}
	}
		break;

	case 3:	// �r���E�܂œ�������X�R�A�͂����I�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++; 
			m_bPause = false;
		}
		break;

	case 4:	// �K���ȑҋ@����
		if (++m_WaitTimer > 110)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 5:	// �����N���b�N����Ɨ����ς����邼�I
	{
				if (tdnMouse::GetLeft() == 3)
				{
					const Vector2 ClickPoint(stage->m_Doglists[2][0]->GetPos().x + 64, stage->m_Doglists[2][0]->GetPos().y + 64);
					if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
					{
						m_TutorialStep++;
						m_bPause = false;

						EffectMgr.AddEffect((int)stage->m_Doglists[2][0]->GetPos().x + 64, (int)stage->m_Doglists[2][0]->GetPos().y + 64, EFFECT_TYPE::DOG_EFFECT);
						se->Play("��", stage->m_Doglists[2][0]->GetPos());
						stage->m_Doglists[2][0]->Change();	// ����ON_OFF

						stage->m_Doglists[0][0]->bEnable = false;
						stage->m_Doglists[0][1]->bEnable = false;
						stage->m_Doglists[1][0]->bEnable = true;
						stage->m_Doglists[1][1]->bEnable = true;
						stage->m_Doglists[2][1]->bEnable = false;
					}
				}
	}
		break;

	case 6:	// ���̊Ԃɗr���Ȃ����Ă���
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 7:	// �r�̗��ꂪ�ς������
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 8:	// �K���ȑҋ@����
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 9:	// ������x�N���b�N����Ɖ������邼�I
	{
				if (tdnMouse::GetLeft() == 3)
				{
					const Vector2 ClickPoint(stage->m_Doglists[2][0]->GetPos().x + 64, stage->m_Doglists[2][0]->GetPos().y + 64);
					if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
					{
						m_TutorialStep++;
						m_bPause = false;

						se->Play("��", stage->m_Doglists[2][0]->GetPos());
						stage->m_Doglists[2][0]->Change();	// ����ON_OFF

						// ���[����L���E������
						stage->m_Doglists[1][0]->bEnable = false;
						stage->m_Doglists[1][1]->bEnable = false;
						stage->m_Doglists[2][0]->bEnable = true;
						stage->m_Doglists[2][1]->bEnable = true;
					}
				}
				break;
	}
	case 10: // �������Ă�̂����Ă鎞��
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 11: // �߂����ˁ[�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 12: // �T�|�b�v�A�b�v�o��܂ł̑ҋ@����
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->WarningPopUp(2);	// �r���[���Ƀ|�b�v�A�b�v�o��
		}
		break;

	case 13: // �T�|�b�v�A�b�v��
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 14:// �I�I�J�~�����邼�I�������N���b�N���ĉ�����悤�I
	{
				if (tdnMouse::GetLeft() == 3)
				{
					int y = (int)tdnMouse::GetPos().y;
					if (y > STAGE_POS_Y[1] && y < STAGE_POS_Y[1] + LANE_WIDTH && tdnMouse::GetPos().x < 150)
					{
						m_TutorialStep++;
						m_bPause = false;
					}
				}
	}
		break;

	case 15: // ��莞�Ԍ�ɘT���o��
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->Create(2);	// �|�b�v�A�b�v���[���ɃI�I�J�~�o��
		}
		break;
		
	case 16: // �I�I�J�~�o�߂���܂ő҂�
		if (EnemyMgr->GetList()->empty())
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 17: // �I�I�J�~����ł�����
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 18: // �K���ȑҋ@����
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 19: // �q���Q�[�W�����邼�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 20: // �q���Q�[�W���܂�܂ő҂�
		BokusouMgr->Update();
		if (++m_WaitTimer > 300)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 21: // �q���Q�[�W�����܂��āA�q�������̉����邼�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;
	case 22: // �q���x�W�G�҂�
		BokusouMgr->Update();
		if (!BokusouMgr->GetList()->empty())
		{
			if (++m_WaitTimer > 90)
			{
				m_WaitTimer = 0;
				m_TutorialStep++;
				m_bPause = true;
			}
		}
		break;

	case 23: // �q�������̉��������I�����g���ėr��q���ɂԂ��Ă݂悤�I
	{
				 if (tdnMouse::GetLeft() == 3)
				 {
					 const Vector2 ClickPoint(stage->m_Doglists[1][0]->GetPos().x + 64, stage->m_Doglists[1][0]->GetPos().y + 64);
					 if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
					 {
						 m_TutorialStep++;
						 m_bPause = false;

						 EffectMgr.AddEffect((int)stage->m_Doglists[1][0]->GetPos().x + 64, (int)stage->m_Doglists[1][0]->GetPos().y + 64, EFFECT_TYPE::DOG_EFFECT);
						 se->Play("��", stage->m_Doglists[1][0]->GetPos());
						 stage->m_Doglists[1][0]->Change();	// ����ON_OFF

						 // ��̉E�ȊO������
						 stage->m_Doglists[0][0]->bEnable = false;
						 stage->m_Doglists[0][1]->bEnable = true;
						 stage->m_Doglists[1][1]->bEnable = false;
						 stage->m_Doglists[2][0]->bEnable = false;
						 stage->m_Doglists[2][1]->bEnable = false;
					 }
				 }
	}
		break;

	case 24: // �r���q���q�b�g����܂ő҂�
		if (!g_pSheepMgr->GetFatList()->empty())
		{
			if (++m_WaitTimer > 90)
			{
				m_WaitTimer = 0;
				m_TutorialStep++;
				m_bPause = true;

				// ����1�t���[���̍X�V�Ŗq��������
				BokusouMgr->Update();
			}
		}
		break;
		
	case 25: // �r���������I�r�̌Q��ŗ����������I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 26: // �f�u�r�����Ă�Ԃ̂Ƃ���
		if (g_pSheepMgr->GetFatList()->empty())
		{
			if (++m_WaitTimer > 60)
			{
				m_WaitTimer = 0;
				m_TutorialStep++;
				m_bPause = true;
			}
		}
		break;
		
	case 27: // �X�R�A������������������I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 28: // �K���ȑҋ@����
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 29: // 1�C�̋]���Ń^�C���𑝂₻���I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 30: // ���̊ԂɃv���C���[�͒͂�ŉ΂̂̂Ƃ���֎����Ă�������
		g_pSheepMgr->m_bCatchOK = true;
		if (NikuMgr->GetYakiniku())
		{
			m_TutorialStep++;
			m_bPause = true;
			g_pSheepMgr->m_bCatchOK = false;
		}
		break;

	case 31: // �������I�r���Ă����ˁI���ɂ͏Ă����������邼�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;
	case 32: // �����p�[�t�F�N�g�ɂȂ�܂ő҂�
		if (++m_WaitTimer > 350)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 33: // ���̃^�C�~���O���p�[�t�F�N�g���I�Ă��r���N���b�N���Ă݂悤�I
	{
				 if (tdnMouse::GetLeft() == 3)
				 {
					 const Vector2 ClickPoint(YAKINIKU_AREA.x + 30, YAKINIKU_AREA.y + 96);
					 const Vector2 mPos = tdnMouse::GetPos();
					 if (mPos.x > ClickPoint.x && mPos.x < ClickPoint.x + 190 && mPos.y > ClickPoint.y && mPos.y < ClickPoint.y + 100)
					 {
						 m_TutorialStep++;
						 m_bPause = false;
						 NikuMgr->CreateNiku();
					 }
				 }
	}
		break;

	case 34:// ���o�E���h��
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->WarningPopUp(1);	// �|�b�v�A�b�v�o��
		}
		break;

	case 35:// �T�|�b�v�A�b�v��
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 36:// �����h���b�N�A���h�h���b�v�ŃI�I�J�~�̃��[���ɂ����Ă݂悤�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 37:// �v���C���[�ɓ��h���b�O�A���h�h���b�v������
		NikuMgr->m_bClickOK = true;
		if (NikuMgr->GetNiku()->isSeted() && NikuMgr->GetNiku()->GetFloor() == 1)
		{
			m_TutorialStep++;
			NikuMgr->m_bClickOK = false;
		}
		break;

	case 38:// �����҂��ĘT�o��
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->Create(1);
		}
		break;

	case 39: // �����ē��H���܂ł̋�
		if ((*EnemyMgr->GetList()->begin())->GetMode() == Enemy::Wolf::MODE::NIKU)
		{
			if (++m_WaitTimer > 90)
			{
				m_WaitTimer = 0;
				m_TutorialStep++;
				m_bPause = true;
			}
		}
		break;

	case 40: // Delicious�I�I�I�J�~������H���Ă邼�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 41: // �f�u���������܂ł̋�
		if (!EnemyMgr->GetFatList()->empty())
		{
			if (++m_WaitTimer > 90)
			{
				m_WaitTimer = 0;
				m_TutorialStep++;
				m_bPause = true;
			}
		}
		break;

	case 42:
		// �I�I�J�~�����������I�������r�Ɠ����v�̂ŁA�Q��ŉ����������I
		if (tdnMouse::GetLeft() == 3)
		{
			const Vector2 ClickPoint(stage->m_Doglists[1][0]->GetPos().x + 64, stage->m_Doglists[1][0]->GetPos().y + 64);
			if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
			{
				m_TutorialStep++;
				m_bPause = false;

				se->Play("��", stage->m_Doglists[1][0]->GetPos());
				stage->m_Doglists[1][0]->Change();	// ����ON_OFF

				stage->m_Doglists[0][1]->bEnable = false;
				stage->m_Doglists[1][0]->bEnable = true;
				stage->m_Doglists[1][1]->bEnable = true;
			}
		}
		break;

	case 43: // �I�I�J�~������܂ŃX�g�b�v
		if (EnemyMgr->GetFatList()->empty())
		{
			if (++m_WaitTimer > 90)
			{
				m_WaitTimer = 0;
				m_TutorialStep++;
				m_bPause = true;
			}
		}
		break;

	case 44: // �������T�������o���ƃ^�C�����񕜂��邼�I
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 45:
		// �K���ȑҋ@����
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 46: // ��{���[���͂���ŏI���B�ǂ��r�Ă����C�t��
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
			FadeControl::Setting(FadeControl::MODE::WHITE_OUT, 4);
		}
		break;

	case 47: // �t�F�[�h�I���܂�
		if (FadeControl::IsEndFade())
		{
			bgm->Stop("MAIN");
			// �V�[���I���t���O
			return true;
		}
		break;
	default: // ��O
		assert(0);
		break;
	}

	// �����~�܂��ĂȂ�������X�V
	if (!m_bPause)
	{
		// PosyEffect
		PostEffectMgr.Update();

		// EffectMGR
		EffectMgr.Update();


		/*�@�����蔻��@*/
		CollisionMgr->Update(g_pSheepMgr, dataMNG, stage);

		dataMNG->Update();
		NikuMgr->Update();
		stage->Update();
		g_pSheepMgr->Update();
		EnemyMgr->Update();
		ShakeMgr->Update();
		UIMNG.Update();

	}

	return false;
}


//******************************************************************
//		�`��
//******************************************************************

void sceneTutorial::Render()
{
	if (state == SCENE::INIT) return;
	tdnView::Clear(0xffffffff);
	tdnView::Activate();


/**********************************************/
//	���̊Ԃ��Q�[�����
/**************************************************/
	renderTarget->RenderTarget();
	//back->Render(0, 0);

	// �X�e�[�W�̌��`��
	stage->RenderBack();

	stage->Render();

	UIMNG.Render();
	m_poseIcon.pic->Render(m_poseIcon.x - 32, m_poseIcon.y - 32);

	// �X�e�[�g�`��
	switch (state) {
	case SCENE::READY:		ReadyRender();		break;
	case SCENE::MAIN:		MainRender();		break;
	}

/************************************************/

/******************************/
	// �|�X�g�G�t�F�N�g���ʎn�܂�	
	PostEffectMgr.BloomBigin();
	renderTarget->Render((int)ShakeMgr->move.x, (int)ShakeMgr->move.y);// �����_�[�^�[�Q�b�g																 
	PostEffectMgr.BloomEnd();
	/*****************************�|�X�g�G�t�F�N�g�����*/

	/******************************/
	// �u���\���ʎn�܂�	
	PostEffectMgr.RadialBigin();
	renderTarget->Render((int)ShakeMgr->move.x, (int)ShakeMgr->move.y);// �����_�[�^�[�Q�b�g
	PostEffectMgr.RadialEnd();
	/*****************************�u���\�����*/

	tdnSystem::GetDevice()->SetRenderTarget(0, backUp);
	//renderTarget->Render((int)ShakeMgr->move.x, (int)ShakeMgr->move.y); //����͕`�悳��Ă��Ȃ�

	// �|�X�g�G�t�F�N�g����
	PostEffectMgr.RadialRender(); // �����ꂪ���˃u���\���|�����^�̕`�挳

	//if (KeyBoard(KB_ENTER))
	{
		PostEffectMgr.BloomRender();
	}
	
	// ��

	switch (state) {
	case SCENE::READY:		EffectMgr.Render();	break;
	case SCENE::MAIN:		EffectMgr.Render();	break;
	}
	

	//��{�I�ɂ͍Ō�B�������̂ݐ������̌��ɂ���̂ŕʓr
	pointer->Render();
	FadeControl::Render();
#ifdef _DEBUG
	DebugText();
	//CollisionMgr->DebugRender(g_pSheepMgr, dataMNG, stage);
#endif
}

void sceneTutorial::ReadyRender()
{
	stage->RenderFront();
	NumberEffect.Render();
	ready->Render();
}

void sceneTutorial::MainRender()
{
	// �ē��`��
	NikuMgr->RenderYakiniku();

	g_pSheepMgr->Render();
	EnemyMgr->Render();
	BokusouMgr->Render();

	// �X�e�[�W�̑O�`��
	stage->RenderFront();

	NikuMgr->RenderNiku();

	Particle2dManager::Render();

	// �f�u�����`��
	g_pSheepMgr->RenderFat();
	EnemyMgr->RenderFat();
	stage->RenderFront2();
	BokusouMgr->RenderFront();
	NikuMgr->RenderFront();
	NumberEffect.Render();



	/*�����`���[�g���A���֘A�̕`�恚��*/

	// �����~�܂��Ă���Â��Ȃ�(���̕ӂ͎��R�ɂǂ���)
	if (m_bPause)
	{
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
	}

	switch (m_TutorialStep)
	{
	case 0:	// �Q�[���X�^�[�g���ėr���o�Ă���܂ł̑ҋ@����
		break;

	case 1:	// �r�����[������o�Ă����I�I
		tdnText::Draw(640, 320, 0xffffffff, "�悤�����A�r�Ă����[���h��");
		break;

	case 2:	// �r�����[���ɓ���܂ł̑ҋ@����
		break;

	case 3:	// �r���E�܂œ�������X�R�A�͂����I�I
		tdnText::Draw(640, 320, 0xffffffff, "�r���E�܂œ�������X�R�A�͂����I�I");
		break;

	case 4:	// �K���ȑҋ@����
		break;

	case 5:	// �����N���b�N����Ɨ����ς����邼�I �����N���b�N�R�}���h
		tdnText::Draw(640, 320, 0xffffffff, "�����N���b�N����Ɨ����ς����邼�I");
		break;

	case 6:	// ���̊Ԃɗr���Ȃ����Ă���
		break;

	case 7:	// �r�̗��ꂪ�ς������
		tdnText::Draw(640, 320, 0xffffffff, "�r�̗��ꂪ�ς������");
		break;

	case 8:	// �K���ȑҋ@����
		break;

	case 9:	// ������x�N���b�N����Ɖ������邼�I �����N���b�N�R�}���h
		tdnText::Draw(640, 320, 0xffffffff, "������x�����N���b�N����Ɖ������邼�I");
		break;
	case 10: // �������Ă�̂����Ă鎞��
		break;

	case 11: // �߂����ˁ[�I
		tdnText::Draw(640, 320, 0xffffffff, "�����߂����ˁ[�I");
		break;

	case 12: // �T�|�b�v�A�b�v�o��܂ł̑ҋ@����
		break;

	case 13: // �T�|�b�v�A�b�v��
		break;

	case 14:// �I�I�J�~�����邼�I�������N���b�N���ĉ�����悤�I �������Ő^�񒆂̏������N���b�N����R�}���h
		tdnText::Draw(640, 320, 0xffffffff, "�I�I�J�~�����邼�I�������N���b�N���ĉ�����悤�I");
		break;

	case 15: // ��莞�Ԍ�ɘT���o��
		break;

	case 16: // �I�I�J�~�o�߂���܂ő҂�
		break;

	case 17: // �I�I�J�~����ł�����
		tdnText::Draw(640, 320, 0xffffffff, "�I�I�J�~����ł�����");
		break;

	case 18: // �K���ȑҋ@����
		break;

	case 19: // �q���Q�[�W�����邼�I
		tdnText::Draw(640, 320, 0xffffffff, "�q���Q�[�W�����邼�I");
		break;

	case 20: // �q���Q�[�W���܂�܂ő҂�
		break;

	case 21: // �q���Q�[�W�����܂��āA�q�������̉����邼�I
		tdnText::Draw(640, 320, 0xffffffff, "�q���Q�[�W�����܂��āA�q�������̉����邼�I");
		break;
	case 22: // �q���x�W�G�҂�
		break;

	case 23: // �q�������̉��������I�����g���ėr��q���ɂԂ��Ă݂悤�I�@ �������Ō��N���b�N�R�}���h
		tdnText::Draw(640, 320, 0xffffffff, "�q�������̉��������I�����g���ėr��q���ɂԂ��Ă݂悤�I");
		break;

	case 24: // �r���q���q�b�g����܂ő҂�
		break;

	case 25: // �r���������I�r�̌Q��ŗ����������I
		tdnText::Draw(640, 320, 0xffffffff, "�r�����������I�r�̌Q��ŗ����������I");
		break;

	case 26: // �f�u�r�����Ă�Ԃ̂Ƃ���
		break;

	case 27: // �X�R�A������������������I
		tdnText::Draw(640, 320, 0xffffffff, "�X�R�A������������������I");
		break;

	case 28: // �K���ȑҋ@����
		break;

	case 29: // 1�C�̋]���Ń^�C���𑝂₻���I
		tdnText::Draw(640, 320, 0xffffffff, "1�C�̋]���Ń^�C���𑝂₻���I�h���b�O�A���h�h���b�v�ŉ΂Ɏ����Ă�����");
		break;

	case 30: // �������̃X�e�[�g�Ńv���C���[�ɗr���h���b�O�A���h�h���b�v�ŏĂ�����I
		break;

	case 31: // �������I�r���Ă����ˁI���ɂ͏Ă����������邼�I
		tdnText::Draw(640, 320, 0xffffffff, "�������I�r���Ă����ˁI���ɂ͏Ă����������邼�I");
		break;
	case 32: // �����p�[�t�F�N�g�ɂȂ�܂ő҂�(�����t�͂��Ȃ�)
		break;

	case 33: // ���̃^�C�~���O���p�[�t�F�N�g���I�Ă��r���N���b�N���Ă݂悤�I �������ŏĂ��r�N���b�N�R�}���h
		tdnText::Draw(640, 320, 0xffffffff, "���̃^�C�~���O���p�[�t�F�N�g���I�Ă��r���N���b�N���Ă݂悤�I");
		break;

	case 34:// ���o�E���h��
		break;

	case 35:// �T�|�b�v�A�b�v��
		break;

	case 36:// �����h���b�N�A���h�h���b�v�ŃI�I�J�~�̃��[���ɂ����Ă݂悤�I
		tdnText::Draw(640, 320, 0xffffffff, "�����h���b�N�A���h�h���b�v�ŃI�I�J�~�̃��[���ɂ����Ă݂悤�I");
		break;

	case 37:// �������̃X�e�[�g�Ńv���C���[�Ɂu�^�񒆃��[���Ɂv���h���b�O�A���h�h���b�v������I�I�I
		break;

	case 38:// �����҂��ĘT�o��
		break;

	case 39: // �����ē��H���܂ł̋�
		break;

	case 40: // Delicious�I�I�I�J�~������H���Ă邼�I
		tdnText::Draw(640, 320, 0xffffffff, "Delicious�I�I�I�J�~������H���Ă邼�I");
		break;

	case 41: // �f�u�I�I�J�~�����������܂ł̋�
		break;

	case 42:
		// �I�I�J�~�����������I�������r�Ɠ����v�̂ŁA�Q��ŉ����������I �������Ō��N���b�N�R�}���h
		tdnText::Draw(640, 320, 0xffffffff, "�I�I�J�~�����������I�������r�Ɠ����v�̂ŁA�Q��ŉ���������");
		break;

	case 43: // �I�I�J�~������܂ł̑ҋ@����
		break;

	case 44: // �������T�������o���ƃ^�C�����񕜂��邼�I
		tdnText::Draw(640, 320, 0xffffffff, "�������T�������o���ƃ^�C�����񕜂��邼�I");
		break;

	case 45:
		// �K���ȑҋ@����
		break;

	case 46: // ��{���[���͂���ŏI���B�ǂ��r�Ă����C�t��
		tdnText::Draw(640, 320, 0xffffffff, "��{���[���͂���ŏI���B�ǂ��r�Ă����C�t��");
		break;

	case 47: // �t�F�[�h�I���܂�
		break;
	}
}

void sceneTutorial::DebugText()
{
	//	�f�o�b�O�p_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//DEBUG_TEXT::DebugText("TEST%d, %s, %d, %c", 1, "aaa", 741, 'A');
	//	_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
}

void sceneTutorial::PoseUpdate()
{
	// �J�[�\���߂�
	pointer->SetState(MousePointer::STATE::NOMAL);

	// �E�F�C�g�^�C�}�[
	static int waitTimer = 0;
	static bool againFlag = false;

	// �|�[�Y
	if (KeyBoard(KB_P) == 3)
	{
		state = SCENE::MAIN;
	}




	m_again.pic->Update();
	m_again.rip->Update();

	m_stop.pic->Update();
	m_stop.rip->Update();


	// ���l
	m_poseTimer.one->Update();
	m_poseTimer.two->Update();
	m_poseTimer.three->Update();



	switch (m_poseState)
	{
	case sceneTutorial::POSE_STATE::START:

		// ���SE�S�������Ŏ~�߂Ă��炨
		se->Stop_all();
		bgm->SetVolume("MAIN", .5f);

		// �N���b�N����SE
		se->Play("�|�[�Y");

		m_poseState = POSE_STATE::EXE;
		waitTimer = 0;
		againFlag = false;
		pointer->SetState(MousePointer::STATE::NOMAL);

		break;
	case sceneTutorial::POSE_STATE::EXE:
	{
		/***************************/
		// �A�Q�C���̓���
		// �����v�Z 
		float len = Math::Length((float)m_again.x + 240, (float)m_again.y + 64, tdnMouse::GetPos().x, tdnMouse::GetPos().y);
		if (len < 220 && 60 > abs((m_again.y + 64) - tdnMouse::GetPos().y))// �{Y���J�b�g����
		{
			// �F�����̉�
			m_again.pic->GetObj()->SetARGB(0xffffffff);
			if (m_again.isAction == false)
			{
				m_again.isAction = true;
				m_again.pic->Action();

				// �J�[�\�������킹��SE�̍Đ�
				se->Play("�J�[�\��");
			}

			// �N���b�N�����
			if (tdnMouse::GetLeft() == 3)
			{
				// �I���N���b�N��
				se->Play("CLICK");

				m_again.rip->Action();
				againFlag = true;
				
			}

		}
		else
		{
			m_again.pic->GetObj()->SetARGB(0xffaaaaaa);
			m_again.isAction = false;
		}

		//��
		// ������傢���傢�̂��傢�̂���
		if (againFlag == true)
		{
			waitTimer++;
			if (waitTimer > 22)
			{
				waitTimer = 0;
				m_poseState = POSE_STATE::COUNT;	//  �J�E���g�X�e�[�g��
			}

		}
		/****************************/

		/***************************/
		// �X�g�b�v�̓���
		// �����v�Z 
		len = Math::Length((float)m_stop.x + 240, (float)m_stop.y + 64, tdnMouse::GetPos().x, tdnMouse::GetPos().y);
		if (len < 220 && 60 > abs((m_stop.y + 64) - tdnMouse::GetPos().y))// �{Y���J�b�g����
		{
			// �F�����̉�
			m_stop.pic->GetObj()->SetARGB(0xffffffff);
			if (m_stop.isAction == false)
			{
				m_stop.isAction = true;
				m_stop.pic->Action();

				// �J�[�\�������킹��SE�̍Đ�
				se->Play("�J�[�\��");
			}

			// �N���b�N�����
			if (tdnMouse::GetLeft() == 3)
			{
				// �I���N���b�N��
				se->Play("�L�����Z��");

				m_stop.rip->Action();
				FadeControl::Setting(FadeControl::MODE::FADE_OUT, 40.0f);
				m_poseState = POSE_STATE::TITLE;	//  �^�C�g���X�e�[�g��
			}
		}
		else
		{
			m_stop.pic->GetObj()->SetARGB(0xffaaaaaa);
			m_stop.isAction = false;
		}
		/****************************/

	}
		break;
	case sceneTutorial::POSE_STATE::COUNT:
		waitTimer++;
		if (waitTimer == 1)
		{
			m_poseTimer.three->Action();		
		}
		if (waitTimer == 32)
		{
			m_poseTimer.two->Action();
		}
		if (waitTimer == 64)
		{
			m_poseTimer.one->Action();
		}		
		if (waitTimer == 96)
		{
			// BGM�̉��߂�
			bgm->SetVolume("MAIN", 1.0f);

			state = SCENE::MAIN;
		}

		break;
	case sceneTutorial::POSE_STATE::TITLE:
		//
		if (FadeControl::IsFade()) return;
		if (FadeControl::IsEndFade())
		{
			bgm->Stop("MAIN");
			state = SCENE::INIT;
			MainFrame->ChangeScene(new Title());
			return;
		}

		break;
	default:
		break;
	}




}

void sceneTutorial::PoseRender()
{

	// �ē��`��
	NikuMgr->RenderYakiniku();

	g_pSheepMgr->Render();
	EnemyMgr->Render();
	BokusouMgr->Render();

	// �X�e�[�W�̑O�`��
	stage->RenderFront();

	NikuMgr->RenderNiku();

	Particle2dManager::Render();

	// �f�u�����`��
	g_pSheepMgr->RenderFat();
	EnemyMgr->RenderFat();
	stage->RenderFront2();
	BokusouMgr->RenderFront();
	NikuMgr->RenderFront();
	NumberEffect.Render();

	// �X�e�[�W�̑O�`��
	//stage->RenderFront();
	//NumberEffect.Render();

	EffectMgr.Render();

	switch (m_poseState)
	{
	case sceneTutorial::POSE_STATE::COUNT:
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x55000000); //������

		m_poseTimer.one->Render  (550, 250);
		m_poseTimer.two->Render  (550, 250);
		m_poseTimer.three->Render(550, 250);
		m_poseTimer.one->Render(550, 250);
		m_poseTimer.two->Render(550, 250);
		m_poseTimer.three->Render(550, 250);
		m_poseTimer.one->Render(550, 250);
		m_poseTimer.two->Render(550, 250);
		m_poseTimer.three->Render(550, 250);
		break;
	default:
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000); //������

		m_again.pic->Render(m_again.x, m_again.y);
		m_again.rip->Render(m_again.x, m_again.y, RS::ADD);

		m_stop.pic->Render(m_stop.x, m_stop.y);
		m_stop.rip->Render(m_stop.x, m_stop.y, RS::ADD);
		break;
	}
	

	//tdnPolygon::Rect(400, 300, 120, 120, RS::COPY, 0xff00ffff);
	//tdnPolygon::Rect(400, 500, 120, 120, RS::COPY, 0xffff00ff);



}
