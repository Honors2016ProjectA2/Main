#include	"SceneMain.h"
#include	"Ready.h"
#include	"End.h"
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
#include	"result.h"
#include	"../Bokusou/Bokusou.h"
#include	"../Shake/Shake.h"
#include	"../UI/UIManager.h"
#include	"../PostEffect/PostEffect.h"
#include	"../particle_2d/particle_2d.h"
#include	"../Effect/EffectManager.h"
#include	"../Niku/Niku.h"
#include	"../UI\ResultUIManager.h"
#include	"result2.h"
#include   "Explain.h"
#include	"Tips.h"
#include	"../TipsCount/TipsCounter.h"

#include "../system/Framework.h"
#include "Scene\Title.h"

namespace{
	namespace SCENE
	{
		enum{
			INIT, EXPLAIN, READY, MAIN, END, RESULT, TIPS, POSE
		};
	}
}

//******************************************************************
//		�������E���
//******************************************************************

bool sceneMain::Initialize()
{
	UIMNG.Init();
	UIMNG.SetTimer(90);
	
	// TipsCount���Z�b�g
	TipsCountMgr->Reset();

	EffectMgr;

	srand(timeGetTime());

	tdnMouse::Initialize(FALSE, true);
	tdnView::Init();

	// ���[���̕��Ƃ��ǂݍ��ނ̂ŁA�X�e�[�W��^�����new
	stage = new StageManager();

	back = new tdn2DObj("DATA/GameHaikei.png");
	explain = new Explain();
	ready = new Ready();
	end = new End();
	tips = new Tips();
	pointer = new MousePointer();
	dataMNG = new DataManager();
	g_pSheepMgr = new SheepManager();
	//result = new Result2();
	RESULT_UIMNG.Init();

	isResultFlag = true;

	// �G�}�l�[�W������k
	EnemyMgr->Initialize();

	byunAlpha = new tdn2DObj("DATA/alpha.png");
	shader2D->SetValue("ByunAlphaMap", byunAlpha);

	tdnSystem::GetDevice()->GetRenderTarget(0, &backUp);
	renderTarget = new tdn2DObj(1280, 720, RENDERTARGET);

	ShakeMgr->Initialize();
	this->Init();

	// �q���}�l�[�W���[������
	BokusouMgr->Initialize();

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
	m_poseIcon.y = 32;
	m_poseIcon.pic->SetARGB(0xffaaaaaa);// �F
	
	return true;
}

sceneMain::~sceneMain()
{
	SAFE_DELETE(back);
	SAFE_DELETE(ready);
	SAFE_DELETE(end);
	SAFE_DELETE(tips);
	SAFE_DELETE(pointer);
	SAFE_DELETE(stage);
	SAFE_DELETE(dataMNG);
	SAFE_DELETE(g_pSheepMgr);
	EnemyMgr->Release();
	SAFE_DELETE(byunAlpha);
	//SAFE_DELETE(result);
	RESULT_UIMNG.Release();
	SAFE_DELETE(renderTarget);
	BokusouMgr->Release();
	PostEffectMgr.Release();
	Particle2dManager::Release();
	UIMNG.Release();
	EffectMgr.Release();
	NikuMgr->Release();
	SAFE_DELETE(explain);

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

bool sceneMain::Update()
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
	case SCENE::EXPLAIN:	ExplainUpdate();	break;
	case SCENE::READY:		ReadyEvent();		break;
	case SCENE::MAIN:		MainUpdate();		break;
	case SCENE::END:		EndEvent();			break;
	case SCENE::RESULT:		ResultUpdate();		break;
	case SCENE::TIPS:		TipsUpdate();		break;
	case SCENE::POSE:		PoseUpdate();		break;
	}

	return true;
}

void sceneMain::DataDelivery()
{
	stage->Reflection(dataMNG, pointer);
	pointer->DataReceive(stage);
	end->DataReceive(stage);
	g_pSheepMgr->Set_pointers(stage, dataMNG);
	EnemyMgr->Set_Pointers(stage, dataMNG);
	//result->Set_MousePointer(pointer);
}


void sceneMain::Init()
{
	stage->Init();
	dataMNG->Init();
	ready->Init();
	end->Init();
	explain->Initialize();
	tips->Init();

//	watchman->Init();
//	m_pSheepMgr->Init();

	FadeControl::Setting(FadeControl::MODE::WHITE_IN, 30.0f);

	// �����̂�������0�ɂ���ƃ��f�B�[�S�[�̏������o��(�f�o�b�O�̎��Z�Ń��f�B�[�S�[�𖳂��ɂ��Ă�)
	state = (0) ? SCENE::MAIN : SCENE::EXPLAIN;
	DataDelivery();
}

void sceneMain::ExplainUpdate()
{
	// PosyEffect
	PostEffectMgr.Update();

	// EffectMGR
	EffectMgr.Update();

	if( explain->Update() ){
		state = SCENE::READY;
		se->Play("�h��", true);
	}
	//stage->Update();
}

void sceneMain::ReadyEvent()
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

void sceneMain::MainUpdate()
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
	BokusouMgr->Update();
	UIMNG.Update();

	if (KeyBoard(KB_N)==3)
	{
		UIMNG.SetTimer(1);
	}
	//
	//// G
	//if (KeyBoard(KB_G) == 3)
	//{
	//	EffectMgr.AddEffect(220, 64, EFFECT_TYPE::EAT);
	//}

	// �^�C����0�ɂȂ�����Q�[���I�[�o�[����
	//if( dataMNG->GetTime() <= 0 ){
	//	state = SCENE::END;
	//	watchman->Reset();
	//	/* �ύX */
	//	m_pSheepMgr->Reset();
	//	result->Set_data(dataMNG->GetScore());
	//	bgm->Stop("MAIN");
	//	se->Play("TIME_UP");
	//}

	// �^�C����0�ɂȂ�����Q�[���I�[�o�[����
	if (UIMNG.GetTimer() <= 0)
	{
		// SE�S�X�g�b�v
		se->Stop_all();

		// �s�s�[
		se->Play("�I���z�C�b�X��");

		state = SCENE::END;

		//UIMNG.SetTimer(120);
	}


	// �|�[�Y
	//if (KeyBoard(KB_P) == 3)
		if (32 > Math::Length(m_poseIcon.x, m_poseIcon.y, tdnMouse::GetPos().x, tdnMouse::GetPos().y))
		{
			if (tdnMouse::GetLeft() == 3)
			{
				state = SCENE::POSE;
				m_poseState = POSE_STATE::START;
			}
		
			// �F
			m_poseIcon.pic->SetARGB(0xffffffff);
		}
		else
		{
			// �F
			m_poseIcon.pic->SetARGB(0xffaaaaaa);
		}
	

	
		
}

void sceneMain::EndEvent()
{
	// PosyEffect
	PostEffectMgr.Update();

	// EffectMGR
	EffectMgr.Update();


	if( end->Update() ){

		
		if (isResultFlag==true)
		{
			isResultFlag = false;
			RESULT_UIMNG.Action();

			// BGM�̉��ʏ���������
			bgm->SetVolume("MAIN", .5f);
		}
		

		state = SCENE::RESULT;
	}
}

void sceneMain::ResultUpdate()
{
	// PosyEffect
	PostEffectMgr.Update();

	// EffectMGR
	EffectMgr.Update();


	//bgm->Play("RESULT");
	if( FadeControl::IsFade() ) return;
	if( FadeControl::IsEndFade() ){
		state = SCENE::TIPS;
		tips->TipsSelect();// �� �q���g�X�V

		//bgm->Stop("MAIN");
		//state = SCENE::INIT;
		//MainFrame->ChangeScene(new Title());
		return;
	}

	if (RESULT_UIMNG.Update()){
		FadeControl::Setting(FadeControl::MODE::FADE_OUT, 30.0f);
	}
}

void sceneMain::TipsUpdate()
{
	// PosyEffect
	PostEffectMgr.Update();

	// EffectMGR
	EffectMgr.Update();

	// �q���g�I�������
	if (tips->Update())
	{
		bgm->Stop("MAIN");
		state = SCENE::INIT;
		MainFrame->ChangeScene(new Title());
	}
}


//******************************************************************
//		�`��
//******************************************************************

void sceneMain::Render()
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
	case SCENE::EXPLAIN:	ExplainRender();	break;
	case SCENE::READY:		ReadyRender();		break;
	case SCENE::MAIN:		MainRender();		break;
	case SCENE::END:		EndRender();		break;
	case SCENE::RESULT:
	{
		stage->RenderFront();
	}break;
	case SCENE::TIPS:
	{
	}break;
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
	case SCENE::RESULT:		ResultRender();		break;
	case SCENE::TIPS:		TipsRender();		break;
	case SCENE::POSE:		PoseRender();		break;
	}

	switch (state) {
	case SCENE::EXPLAIN:	EffectMgr.Render();	break;
	case SCENE::READY:		EffectMgr.Render();	break;
	case SCENE::MAIN:		EffectMgr.Render();	break;
	case SCENE::END:		            		break;
	case SCENE::RESULT:		EffectMgr.Render(); break;
	case SCENE::TIPS:		EffectMgr.Render(); break;
	case SCENE::POSE:		                    break;
	}
	

	//��{�I�ɂ͍Ō�B�������̂ݐ������̌��ɂ���̂ŕʓr
	pointer->Render();
	FadeControl::Render();
#ifdef _DEBUG
	DebugText();
	//CollisionMgr->DebugRender(g_pSheepMgr, dataMNG, stage);
#endif
}

void sceneMain::ExplainRender()
{
	stage->RenderFront();

	NumberEffect.Render();
	explain->Render();

}

void sceneMain::ReadyRender()
{
	stage->RenderFront();
	NumberEffect.Render();
	ready->Render();
}

void sceneMain::MainRender()
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
}

void sceneMain::EndRender()
{
	// �X�e�[�W�̑O�`��
	stage->RenderFront();
	NumberEffect.Render();
	end->Render();
}

void sceneMain::ResultRender()
{
	// �X�e�[�W�̑O�`��
	//stage->RenderFront();
	NumberEffect.Render();
	//result->Render();
	RESULT_UIMNG.Render();
}

void sceneMain::TipsRender()
{
	tips->Render();


}


void sceneMain::DebugText()
{
	//	�f�o�b�O�p_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//DEBUG_TEXT::DebugText("TEST%d, %s, %d, %c", 1, "aaa", 741, 'A');
	//	_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
}

void sceneMain::PoseUpdate()
{

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
	case sceneMain::POSE_STATE::START:

		// ���SE�S�������Ŏ~�߂Ă��炨
		se->Stop_all();
		bgm->SetVolume("MAIN", .5f);

		// �N���b�N����SE
		se->Play("�|�[�Y");

		m_poseState = POSE_STATE::EXE;
		waitTimer = 0;
		againFlag = false;

		break;
	case sceneMain::POSE_STATE::EXE:
	{
		/***************************/
		// �A�Q�C���̓���
		// �����v�Z 
		float len = Math::Length(m_again.x + 240, m_again.y + 64, tdnMouse::GetPos().x, tdnMouse::GetPos().y);
		if (len < 220 && 60 > abs((m_again.y + 64) - tdnMouse::GetPos().y))// �{Y���J�b�g����
		{
			// �F�����̉�
			m_again.pic->GetObj()->SetARGB(0xffffffff);
			if (m_again.isAction == false)
			{
				m_again.isAction = true;
				m_again.pic->Action();
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
		len = Math::Length(m_stop.x + 240, m_stop.y + 64, tdnMouse::GetPos().x, tdnMouse::GetPos().y);
		if (len < 220 && 60 > abs((m_stop.y + 64) - tdnMouse::GetPos().y))// �{Y���J�b�g����
		{
			// �F�����̉�
			m_stop.pic->GetObj()->SetARGB(0xffffffff);
			if (m_stop.isAction == false)
			{
				m_stop.isAction = true;
				m_stop.pic->Action();
			}

			// �N���b�N�����
			if (tdnMouse::GetLeft() == 3)
			{
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
	case sceneMain::POSE_STATE::COUNT:
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
	case sceneMain::POSE_STATE::TITLE:
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

void sceneMain::PoseRender()
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
	case sceneMain::POSE_STATE::COUNT:
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
