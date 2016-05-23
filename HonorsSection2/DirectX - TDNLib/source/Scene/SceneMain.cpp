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

#include	"../UI/UIManager.h"


namespace{
	namespace SCENE{
		enum{
			INIT, READY, MAIN, END, RESULT,
		};
	}

	namespace SHAKE{
		const float WIDTH = 0.05f;
		const int MAX_TIME = 60;
		const int WAIT_1 = 20;
		const int WAIT_2 = 10;
	}
}

//******************************************************************
//		初期化・解放
//******************************************************************

bool sceneMain::Initialize()
{
	srand(timeGetTime());

	tdnMouse::Initialize(FALSE);
	tdnView::Init();

	// レーンの幅とか読み込むので、ステージを真っ先にnew
	stage = new StageManager();

	back = new tdn2DObj("DATA/GameHaikei.png");
	ready = new Ready();
	end = new End();
	pointer = new MousePointer();
	dataMNG = new DataManager();
	m_pSheepMgr = new SheepManager();
	watchman = new Watchman_mng();
	result = new Result();

	byunAlpha = new tdn2DObj("DATA/alpha.png");
	shader2D->SetValue("ByunAlphaMap", byunAlpha);

	tdnSystem::GetDevice()->GetRenderTarget(0, &backUp);
	renderTarget = new tdn2DObj(1280, 720, RENDERTARGET);

	shake.Init();
	this->Init();

	// 牧草マネージャー初期化
	BokusouMgr->Initialize();

	bgm->Play("MAIN");

	return true;
}

void sceneMain::Shake::Init()
{
	move = Vector2(0,0);
	timer = 0;
	beforeFloor = 0;
}

sceneMain::~sceneMain()
{
	SAFE_DELETE(back);
	SAFE_DELETE(ready);
	SAFE_DELETE(end);
	SAFE_DELETE(pointer);
	SAFE_DELETE(stage);
	SAFE_DELETE(dataMNG);
	SAFE_DELETE(m_pSheepMgr);
	SAFE_DELETE(watchman);
	SAFE_DELETE(byunAlpha);
	SAFE_DELETE(result);
	SAFE_DELETE(renderTarget);
	BokusouMgr->Release();
	UIMNG.Release();}

//******************************************************************
//		処理
//******************************************************************

bool sceneMain::Update()
{
	tdnMouse::Update();
	pointer->Update();
	FadeControl::Update();

	switch(state){
	case SCENE::INIT:		Init();				break;
	case SCENE::READY:		ReadyEvent();		break;
	case SCENE::MAIN:		MainUpdate();		break;
	case SCENE::END:		EndEvent();			break;
	case SCENE::RESULT:		ResultUpdate();		break;
	}

	/*　データ受け渡し　*/
	DataDelivery();
	/*　当たり判定　*/
	CollisionMgr->Update(m_pSheepMgr, watchman, dataMNG, stage);

	return true;
}

void sceneMain::DataDelivery()
{
	stage->Reflection(dataMNG, pointer);
	pointer->DataReceive(stage);
	end->DataReceive(stage);
	m_pSheepMgr->Set_pointers(stage, dataMNG);
	watchman->Set_Pointers(stage, dataMNG);
	result->Set_MousePointer(pointer);
	shake.SetFloor(stage->floor);
}


void sceneMain::Init()
{
	stage->Init();
	dataMNG->Init();
	ready->Init();
	end->Init();
//	watchman->Init();
//	m_pSheepMgr->Init();

	FadeControl::Setting(FadeControl::MODE::FADE_IN, 30.0f);

	// ここのかっこを0にするとレディーゴーの処理が出る(デバッグの時短でレディーゴーを無しにしてる)
	state = (1) ? SCENE::MAIN : SCENE::READY;
	DataDelivery();
}

void sceneMain::ReadyEvent()
{
	if( ready->Update() ){
		state = SCENE::MAIN;
		m_pSheepMgr->Start();
	}
	stage->Update();
}

void sceneMain::MainUpdate()
{
	dataMNG->Update();
	stage->Update();
	m_pSheepMgr->Update();
	watchman->Update();
	shake.Update();
	BokusouMgr->Update();

	UIMNG.Update();


	// タイムが0になったらゲームオーバー処理
	//if( dataMNG->GetTime() <= 0 ){
	//	state = SCENE::END;
	//	watchman->Reset();
	//	/* 変更 */
	//	m_pSheepMgr->Reset();
	//	result->Set_data(dataMNG->GetScore());
	//	bgm->Stop("MAIN");
	//	se->Play("TIME_UP");
	//}
}

void sceneMain::EndEvent()
{
	if( end->Update() ){
		state = SCENE::RESULT;
	}
}

void sceneMain::ResultUpdate()
{
	bgm->Play("RESULT");
	if( FadeControl::IsFade() ) return;
	if( FadeControl::IsEndFade() ){
		state = SCENE::INIT;
		bgm->Stop("RESULT");
	}

	if (result->Update()){
		FadeControl::Setting(FadeControl::MODE::FADE_OUT, 30.0f);
	}
}

void sceneMain::Shake::Update()
{
	if( waitTimer > 0 ){
		waitTimer--;
		return;
	}

	if( timer > 0 ){
		move = Vector2(timer*SHAKE::WIDTH*(rand()%3-1), timer*SHAKE::WIDTH*(rand()%3-1));
		timer--;
	}else{
		move = Vector2(0, 0);
	}	
}

void sceneMain::Shake::SetFloor(int floor)
{
	if( floor > beforeFloor ){
		timer = SHAKE::MAX_TIME;
		if( floor == 1 ){
			waitTimer = SHAKE::WAIT_1;
		}else if( floor == 2 ){
			waitTimer = SHAKE::WAIT_2;
		}
	}
	beforeFloor = floor;
}


//******************************************************************
//		描画
//******************************************************************

void sceneMain::Render()
{
	if (state == SCENE::INIT) return;
	tdnView::Clear(0xffffffff);
	tdnView::Activate();

	renderTarget->RenderTarget();
	back->Render(0, 0);

	stage->Render();

	switch (state){
	case SCENE::READY:		ReadyRender();		break;
	case SCENE::MAIN:		MainRender();		break;
	case SCENE::END:		EndRender();		break;
	case SCENE::RESULT:		ResultRender();		break;
	}

	UIMNG.Render();

	tdnSystem::GetDevice()->SetRenderTarget(0, backUp);
	renderTarget->Render((int)shake.move.x, (int)shake.move.y, 1280, 720, 0, 0, 1280, 720);

	//基本的には最後。説明時のみ説明書の後ろにするので別途
	pointer->Render();
	FadeControl::Render();
#ifdef _DEBUG
	DebugText();
	CollisionMgr->DebugRender(m_pSheepMgr, watchman, dataMNG, stage);
#endif
}

void sceneMain::ReadyRender()
{
	ready->Render();
}

void sceneMain::MainRender()
{
	BokusouMgr->Render();
	m_pSheepMgr->Render();
	watchman->Render();
}

void sceneMain::EndRender()
{
	end->Render();
}

void sceneMain::ResultRender()
{
	result->Render();
}


void sceneMain::DebugText()
{
	//	デバッグ用_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//DEBUG_TEXT::DebugText("TEST%d, %s, %d, %c", 1, "aaa", 741, 'A');
	//	_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
}