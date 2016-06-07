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


#include "../system/Framework.h"
#include "Scene\Title.h"

namespace{
	namespace SCENE
	{
		enum{
			INIT,EXPLAIN, READY, MAIN, END, RESULT,
		};
	}
}

//******************************************************************
//		�������E���
//******************************************************************

bool sceneMain::Initialize()
{
	UIMNG.Init();
	UIMNG.SetTimer(120);
	

	EffectMgr;

	srand(timeGetTime());

	tdnMouse::Initialize(FALSE);
	tdnView::Init();

	// ���[���̕��Ƃ��ǂݍ��ނ̂ŁA�X�e�[�W��^�����new
	stage = new StageManager();

	back = new tdn2DObj("DATA/GameHaikei.png");
	explain = new Explain();
	ready = new Ready();
	end = new End();
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

	bgm->Play("MAIN");

	// �|�X�g�G�t�F�N�g
	PostEffectMgr;

	// �p�[�e�B�N��������
	Particle2dManager::Initialize("DATA/Effect/particle.png", 5000, 4, 4);

	return true;
}

sceneMain::~sceneMain()
{
	SAFE_DELETE(back);
	SAFE_DELETE(ready);
	SAFE_DELETE(end);
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
	/*�@�����蔻��@*/
	CollisionMgr->Update(g_pSheepMgr, dataMNG, stage);

	// PosyEffect
	PostEffectMgr.Update();

	// EffectMGR
	EffectMgr.Update();
	if (KeyBoard(KB_F) == 3)
	{
		EffectMgr.AddEffect(300, 300, EFFECT_TYPE::NOTICE);
	}

	switch (state) {
	case SCENE::INIT:		Init();				break;
	case SCENE::EXPLAIN:	ExplainUpdate();	break;
	case SCENE::READY:		ReadyEvent();		break;
	case SCENE::MAIN:		MainUpdate();		break;
	case SCENE::END:		EndEvent();			break;
	case SCENE::RESULT:		ResultUpdate();		break;
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

//	watchman->Init();
//	m_pSheepMgr->Init();

	FadeControl::Setting(FadeControl::MODE::WHITE_IN, 30.0f);

	// �����̂�������0�ɂ���ƃ��f�B�[�S�[�̏������o��(�f�o�b�O�̎��Z�Ń��f�B�[�S�[�𖳂��ɂ��Ă�)
	state = (0) ? SCENE::MAIN : SCENE::EXPLAIN;
	DataDelivery();
}

void sceneMain::ExplainUpdate()
{
	if( explain->Update() ){
		state = SCENE::READY;
	}
	stage->Update();
}

void sceneMain::ReadyEvent()
{
	if( ready->Update() ){
		state = SCENE::MAIN;
		//g_pSheepMgr->Start();
	}
	stage->Update();
}

void sceneMain::MainUpdate()
{
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
		EffectMgr.AddEffect( 220, 64, EFFECT_TYPE::BURN);
	}

	// G
	if (KeyBoard(KB_G) == 3)
	{
		EffectMgr.AddEffect(220, 64, EFFECT_TYPE::EAT);
	}

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

		UIMNG.SetTimer(120);
	}

	if (KeyBoard(KB_SPACE) == 3)
	{
		UIMNG.AddTimer(10);
	}
		
}

void sceneMain::EndEvent()
{
	if( end->Update() ){

		
		if (isResultFlag==true)
		{
			isResultFlag = false;
			RESULT_UIMNG.Action();
		}
		

		state = SCENE::RESULT;
	}
}

void sceneMain::ResultUpdate()
{
	//bgm->Play("RESULT");
	if( FadeControl::IsFade() ) return;
	if( FadeControl::IsEndFade() ){
		//state = SCENE::INIT;
		bgm->Stop("MAIN");
		state = SCENE::INIT;
		MainFrame->ChangeScene(new Title());
		return;
	}

	if (RESULT_UIMNG.Update()){
		FadeControl::Setting(FadeControl::MODE::FADE_OUT, 30.0f);
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

	renderTarget->RenderTarget();
	//back->Render(0, 0);


	// �X�e�[�W�̌��`��
	stage->RenderBack();

	stage->Render();

	UIMNG.Render();



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
	renderTarget->Render((int)ShakeMgr->move.x, (int)ShakeMgr->move.y);

	// �|�X�g�G�t�F�N�g����
	PostEffectMgr.RadialRender();

	//if (KeyBoard(KB_ENTER))
	{
		PostEffectMgr.BloomRender();
	}
	




	// �X�e�[�g�`��
	switch (state) {
	case SCENE::EXPLAIN:	ExplainRender();	break;
	case SCENE::READY:		ReadyRender();		break;
	case SCENE::MAIN:		MainRender();		break;
	case SCENE::END:		EndRender();		break;
	case SCENE::RESULT:		ResultRender();		break;
	}

	EffectMgr.Render();

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
	explain->Render();

}

void sceneMain::ReadyRender()
{
	stage->RenderFront();
	ready->Render();
}

void sceneMain::MainRender()
{
	BokusouMgr->Render();
	g_pSheepMgr->Render();
	EnemyMgr->Render();
	NikuMgr->Render();

	// �X�e�[�W�̑O�`��
	stage->RenderFront();
	Particle2dManager::Render();

	// �f�u�����`��
	g_pSheepMgr->RenderFat();
	EnemyMgr->RenderFat();
}

void sceneMain::EndRender()
{
	// �X�e�[�W�̑O�`��
	stage->RenderFront();
	end->Render();
}

void sceneMain::ResultRender()
{
	// �X�e�[�W�̑O�`��
	stage->RenderFront();
	//result->Render();
	RESULT_UIMNG.Render();
}


void sceneMain::DebugText()
{
	//	�f�o�b�O�p_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//DEBUG_TEXT::DebugText("TEST%d, %s, %d, %c", 1, "aaa", 741, 'A');
	//	_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
}