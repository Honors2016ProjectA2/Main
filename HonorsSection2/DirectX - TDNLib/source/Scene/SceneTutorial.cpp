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
//		初期化・解放
//******************************************************************

bool sceneTutorial::Initialize()
{
	// チュートリアルステップ初期亜k
	m_TutorialStep = 0;
	m_bPause = false;
	m_WaitTimer = 0;

	UIMNG.Init();
	UIMNG.SetTimer(540);
	
	// TipsCountリセット
	TipsCountMgr->Reset();

	EffectMgr;

	srand(timeGetTime());

	tdnMouse::Initialize(FALSE);
	tdnView::Init();

	// レーンの幅とか読み込むので、ステージを真っ先にnew
	stage = new StageManager(true);

	back = new tdn2DObj("DATA/GameHaikei.png");
	ready = new Ready(true);
	pointer = new MousePointer();
	dataMNG = new DataManager();
	g_pSheepMgr = new SheepManager(true);
	//result = new Result2();
	RESULT_UIMNG.Init();

	isResultFlag = true;

	// 敵マネージャ初期k
	EnemyMgr->Initialize(true);

	tdnSystem::GetDevice()->GetRenderTarget(0, &backUp);
	renderTarget = new tdn2DObj(1280, 720, RENDERTARGET);

	ShakeMgr->Initialize();
	this->Init();

	// 牧草マネージャー初期化
	BokusouMgr->Initialize(true);

	// 肉マネージャー初期化
	NikuMgr->Initialize();

	// BGM
	bgm->Play("MAIN");
	bgm->SetVolume("MAIN", 1);

	// ポストエフェクト
	PostEffectMgr;

	// パーティクル初期化
	Particle2dManager::Initialize("DATA/Effect/particle.png", 5000, 4, 4);

	// 全部のドア閉まってる！！
	g_CreateSheepFloor = -1;

	// ポーズ
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
	m_poseIcon.pic->SetARGB(0xffaaaaaa);// 色
	
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
//		処理
//******************************************************************

bool sceneTutorial::Update()
{
	tdnMouse::Update();
	pointer->Update();
	FadeControl::Update();
	Particle2dManager::Update();


	/*　データ受け渡し　*/
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
	case 0:	// ゲームスタートして羊が出てくるまでの待機期間
	{
				g_pSheepMgr->m_bCatchOK = false;	// 羊掴む昨日オフ
				stage->m_bDogUpdate = false;		// 犬の機能オフ
				NikuMgr->m_bClickOK = false;		// 肉クリックして焼く機能オフ
				if (++m_WaitTimer > 24)
				{ 
					m_WaitTimer = 0; 
					m_TutorialStep++; 
					m_bPause = true;
				}
	}
		break;

	case 1:	// 羊がレーンから出てくるよ！！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++; 
			m_bPause = false;
		}
		break;

	case 2:	// 羊がレーンに入るまでの待機時間
	{
				if (++m_WaitTimer > 110)
				{
					m_WaitTimer = 0; 
					m_TutorialStep++;
					m_bPause = true;
				}
	}
		break;

	case 3:	// 羊が右まで逃げたらスコアはいるよ！！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++; 
			m_bPause = false;
		}
		break;

	case 4:	// 適当な待機時間
		if (++m_WaitTimer > 110)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 5:	// 犬をクリックすると流れを変えられるぞ！
	{
				if (tdnMouse::GetLeft() == 3)
				{
					const Vector2 ClickPoint(stage->m_Doglists[2][0]->GetPos().x + 64, stage->m_Doglists[2][0]->GetPos().y + 64);
					if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
					{
						m_TutorialStep++;
						m_bPause = false;

						EffectMgr.AddEffect((int)stage->m_Doglists[2][0]->GetPos().x + 64, (int)stage->m_Doglists[2][0]->GetPos().y + 64, EFFECT_TYPE::DOG_EFFECT);
						se->Play("犬", stage->m_Doglists[2][0]->GetPos());
						stage->m_Doglists[2][0]->Change();	// 犬のON_OFF

						stage->m_Doglists[0][0]->bEnable = false;
						stage->m_Doglists[0][1]->bEnable = false;
						stage->m_Doglists[1][0]->bEnable = true;
						stage->m_Doglists[1][1]->bEnable = true;
						stage->m_Doglists[2][1]->bEnable = false;
					}
				}
	}
		break;

	case 6:	// この間に羊が曲がっている
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 7:	// 羊の流れが変わったな
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 8:	// 適当な待機時間
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 9:	// もう一度クリックすると解除するぞ！
	{
				if (tdnMouse::GetLeft() == 3)
				{
					const Vector2 ClickPoint(stage->m_Doglists[2][0]->GetPos().x + 64, stage->m_Doglists[2][0]->GetPos().y + 64);
					if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
					{
						m_TutorialStep++;
						m_bPause = false;

						se->Play("犬", stage->m_Doglists[2][0]->GetPos());
						stage->m_Doglists[2][0]->Change();	// 犬のON_OFF

						// レーンを有効・無効化
						stage->m_Doglists[1][0]->bEnable = false;
						stage->m_Doglists[1][1]->bEnable = false;
						stage->m_Doglists[2][0]->bEnable = true;
						stage->m_Doglists[2][1]->bEnable = true;
					}
				}
				break;
	}
	case 10: // 解除してるのを見てる時間
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 11: // 戻ったねー！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 12: // 狼ポップアップ出るまでの待機時間
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->WarningPopUp(2);	// 羊レーンにポップアップ出す
		}
		break;

	case 13: // 狼ポップアップ中
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 14:// オオカミが来るぞ！小屋をクリックして回避しよう！
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

	case 15: // 一定時間後に狼を出す
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->Create(2);	// ポップアップレーンにオオカミ出す
		}
		break;
		
	case 16: // オオカミ経過するまで待つ
		if (EnemyMgr->GetList()->empty())
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 17: // オオカミ回避できたね
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 18: // 適当な待機時間
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 19: // 牧草ゲージがあるぞ！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 20: // 牧草ゲージ溜まるまで待つ
		BokusouMgr->Update();
		if (++m_WaitTimer > 300)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 21: // 牧草ゲージが溜まって、牧草が実体化するぞ！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;
	case 22: // 牧草ベジエ待つ
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

	case 23: // 牧草が実体化したぞ！犬を使って羊を牧草にぶつけてみよう！
	{
				 if (tdnMouse::GetLeft() == 3)
				 {
					 const Vector2 ClickPoint(stage->m_Doglists[1][0]->GetPos().x + 64, stage->m_Doglists[1][0]->GetPos().y + 64);
					 if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
					 {
						 m_TutorialStep++;
						 m_bPause = false;

						 EffectMgr.AddEffect((int)stage->m_Doglists[1][0]->GetPos().x + 64, (int)stage->m_Doglists[1][0]->GetPos().y + 64, EFFECT_TYPE::DOG_EFFECT);
						 se->Play("犬", stage->m_Doglists[1][0]->GetPos());
						 stage->m_Doglists[1][0]->Change();	// 犬のON_OFF

						 // 上の右以外無効化
						 stage->m_Doglists[0][0]->bEnable = false;
						 stage->m_Doglists[0][1]->bEnable = true;
						 stage->m_Doglists[1][1]->bEnable = false;
						 stage->m_Doglists[2][0]->bEnable = false;
						 stage->m_Doglists[2][1]->bEnable = false;
					 }
				 }
	}
		break;

	case 24: // 羊が牧草ヒットするまで待つ
		if (!g_pSheepMgr->GetFatList()->empty())
		{
			if (++m_WaitTimer > 90)
			{
				m_WaitTimer = 0;
				m_TutorialStep++;
				m_bPause = true;

				// この1フレームの更新で牧草を消す
				BokusouMgr->Update();
			}
		}
		break;
		
	case 25: // 羊が太った！羊の群れで流しこもう！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 26: // デブ羊押してる間のところ
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
		
	case 27: // スコアがたくさん入ったぞ！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 28: // 適当な待機時間
		if (++m_WaitTimer > 60)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 29: // 1匹の犠牲でタイムを増やそう！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 30: // この間にプレイヤーは掴んで火ののところへ持っていかせる
		g_pSheepMgr->m_bCatchOK = true;
		if (NikuMgr->GetYakiniku())
		{
			m_TutorialStep++;
			m_bPause = true;
			g_pSheepMgr->m_bCatchOK = false;
		}
		break;

	case 31: // お見事！羊を焼けたね！肉には焼き加減があるぞ！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;
	case 32: // 肉がパーフェクトになるまで待つ
		if (++m_WaitTimer > 350)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 33: // このタイミングがパーフェクトだ！焼き羊をクリックしてみよう！
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

	case 34:// 肉バウンド中
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->WarningPopUp(1);	// ポップアップ出す
		}
		break;

	case 35:// 狼ポップアップ中
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 36:// 肉をドラックアンドドロップでオオカミのレーンにおいてみよう！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 37:// プレイヤーに肉ドラッグアンドドロップさせる
		NikuMgr->m_bClickOK = true;
		if (NikuMgr->GetNiku()->isSeted() && NikuMgr->GetNiku()->GetFloor() == 1)
		{
			m_TutorialStep++;
			NikuMgr->m_bClickOK = false;
		}
		break;

	case 38:// 少し待って狼出現
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			EnemyMgr->Create(1);
		}
		break;

	case 39: // 走って肉食うまでの空白
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

	case 40: // Delicious！オオカミが肉を食ってるぞ！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 41: // デブ生成されるまでの空白
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
		// オオカミが太ったぞ！太った羊と同じ要領で、群れで押し込もう！
		if (tdnMouse::GetLeft() == 3)
		{
			const Vector2 ClickPoint(stage->m_Doglists[1][0]->GetPos().x + 64, stage->m_Doglists[1][0]->GetPos().y + 64);
			if ((tdnMouse::GetPos() - ClickPoint).LengthSq() < 64 * 64)
			{
				m_TutorialStep++;
				m_bPause = false;

				se->Play("犬", stage->m_Doglists[1][0]->GetPos());
				stage->m_Doglists[1][0]->Change();	// 犬のON_OFF

				stage->m_Doglists[0][1]->bEnable = false;
				stage->m_Doglists[1][0]->bEnable = true;
				stage->m_Doglists[1][1]->bEnable = true;
			}
		}
		break;

	case 43: // オオカミが入るまでストップ
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

	case 44: // 太った狼を押し出すとタイムが回復するぞ！
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
		}
		break;

	case 45:
		// 適当な待機時間
		if (++m_WaitTimer > 120)
		{
			m_WaitTimer = 0;
			m_TutorialStep++;
			m_bPause = true;
		}
		break;

	case 46: // 基本ルールはこれで終了。良い羊焼きライフを
		if (tdnMouse::GetLeft() == 3)
		{
			m_TutorialStep++;
			m_bPause = false;
			FadeControl::Setting(FadeControl::MODE::WHITE_OUT, 4);
		}
		break;

	case 47: // フェード終了まち
		if (FadeControl::IsEndFade())
		{
			bgm->Stop("MAIN");
			// シーン終了フラグ
			return true;
		}
		break;
	default: // 例外
		assert(0);
		break;
	}

	// 時が止まってなかったら更新
	if (!m_bPause)
	{
		// PosyEffect
		PostEffectMgr.Update();

		// EffectMGR
		EffectMgr.Update();


		/*　当たり判定　*/
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
//		描画
//******************************************************************

void sceneTutorial::Render()
{
	if (state == SCENE::INIT) return;
	tdnView::Clear(0xffffffff);
	tdnView::Activate();


/**********************************************/
//	この間がゲーム画面
/**************************************************/
	renderTarget->RenderTarget();
	//back->Render(0, 0);

	// ステージの後ろ描画
	stage->RenderBack();

	stage->Render();

	UIMNG.Render();
	m_poseIcon.pic->Render(m_poseIcon.x - 32, m_poseIcon.y - 32);

	// ステート描画
	switch (state) {
	case SCENE::READY:		ReadyRender();		break;
	case SCENE::MAIN:		MainRender();		break;
	}

/************************************************/

/******************************/
	// ポストエフェクト効果始まり	
	PostEffectMgr.BloomBigin();
	renderTarget->Render((int)ShakeMgr->move.x, (int)ShakeMgr->move.y);// レンダーターゲット																 
	PostEffectMgr.BloomEnd();
	/*****************************ポストエフェクトおわり*/

	/******************************/
	// ブラ―効果始まり	
	PostEffectMgr.RadialBigin();
	renderTarget->Render((int)ShakeMgr->move.x, (int)ShakeMgr->move.y);// レンダーターゲット
	PostEffectMgr.RadialEnd();
	/*****************************ブラ―おわり*/

	tdnSystem::GetDevice()->SetRenderTarget(0, backUp);
	//renderTarget->Render((int)ShakeMgr->move.x, (int)ShakeMgr->move.y); //これは描画されていない

	// ポストエフェクトたち
	PostEffectMgr.RadialRender(); // ◎これが放射ブラ―を掛けた真の描画元

	//if (KeyBoard(KB_ENTER))
	{
		PostEffectMgr.BloomRender();
	}
	
	// ↑

	switch (state) {
	case SCENE::READY:		EffectMgr.Render();	break;
	case SCENE::MAIN:		EffectMgr.Render();	break;
	}
	

	//基本的には最後。説明時のみ説明書の後ろにするので別途
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
	// 焼肉描画
	NikuMgr->RenderYakiniku();

	g_pSheepMgr->Render();
	EnemyMgr->Render();
	BokusouMgr->Render();

	// ステージの前描画
	stage->RenderFront();

	NikuMgr->RenderNiku();

	Particle2dManager::Render();

	// デブたち描画
	g_pSheepMgr->RenderFat();
	EnemyMgr->RenderFat();
	stage->RenderFront2();
	BokusouMgr->RenderFront();
	NikuMgr->RenderFront();
	NumberEffect.Render();



	/*★★チュートリアル関連の描画★★*/

	// 時が止まってたら暗くなる(この辺は自由にどうぞ)
	if (m_bPause)
	{
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
	}

	switch (m_TutorialStep)
	{
	case 0:	// ゲームスタートして羊が出てくるまでの待機期間
		break;

	case 1:	// 羊がレーンから出てくるよ！！
		tdnText::Draw(640, 320, 0xffffffff, "ようこそ、羊焼きワールドへ");
		break;

	case 2:	// 羊がレーンに入るまでの待機時間
		break;

	case 3:	// 羊が右まで逃げたらスコアはいるよ！！
		tdnText::Draw(640, 320, 0xffffffff, "羊が右まで逃げたらスコアはいるよ！！");
		break;

	case 4:	// 適当な待機時間
		break;

	case 5:	// 犬をクリックすると流れを変えられるぞ！ ※犬クリックコマンド
		tdnText::Draw(640, 320, 0xffffffff, "犬をクリックすると流れを変えられるぞ！");
		break;

	case 6:	// この間に羊が曲がっている
		break;

	case 7:	// 羊の流れが変わったな
		tdnText::Draw(640, 320, 0xffffffff, "羊の流れが変わったな");
		break;

	case 8:	// 適当な待機時間
		break;

	case 9:	// もう一度クリックすると解除するぞ！ ※犬クリックコマンド
		tdnText::Draw(640, 320, 0xffffffff, "もう一度犬をクリックすると解除するぞ！");
		break;
	case 10: // 解除してるのを見てる時間
		break;

	case 11: // 戻ったねー！
		tdnText::Draw(640, 320, 0xffffffff, "犬が戻ったねー！");
		break;

	case 12: // 狼ポップアップ出るまでの待機時間
		break;

	case 13: // 狼ポップアップ中
		break;

	case 14:// オオカミが来るぞ！小屋をクリックして回避しよう！ ※ここで真ん中の小屋をクリックするコマンド
		tdnText::Draw(640, 320, 0xffffffff, "オオカミが来るぞ！小屋をクリックして回避しよう！");
		break;

	case 15: // 一定時間後に狼を出す
		break;

	case 16: // オオカミ経過するまで待つ
		break;

	case 17: // オオカミ回避できたね
		tdnText::Draw(640, 320, 0xffffffff, "オオカミ回避できたね");
		break;

	case 18: // 適当な待機時間
		break;

	case 19: // 牧草ゲージがあるぞ！
		tdnText::Draw(640, 320, 0xffffffff, "牧草ゲージがあるぞ！");
		break;

	case 20: // 牧草ゲージ溜まるまで待つ
		break;

	case 21: // 牧草ゲージが溜まって、牧草が実体化するぞ！
		tdnText::Draw(640, 320, 0xffffffff, "牧草ゲージが溜まって、牧草が実体化するぞ！");
		break;
	case 22: // 牧草ベジエ待つ
		break;

	case 23: // 牧草が実体化したぞ！犬を使って羊を牧草にぶつけてみよう！　 ※ここで犬クリックコマンド
		tdnText::Draw(640, 320, 0xffffffff, "牧草が実体化したぞ！犬を使って羊を牧草にぶつけてみよう！");
		break;

	case 24: // 羊が牧草ヒットするまで待つ
		break;

	case 25: // 羊が太った！羊の群れで流しこもう！
		tdnText::Draw(640, 320, 0xffffffff, "羊が太ったぞ！羊の群れで流しこもう！");
		break;

	case 26: // デブ羊押してる間のところ
		break;

	case 27: // スコアがたくさん入ったぞ！
		tdnText::Draw(640, 320, 0xffffffff, "スコアがたくさん入ったぞ！");
		break;

	case 28: // 適当な待機時間
		break;

	case 29: // 1匹の犠牲でタイムを増やそう！
		tdnText::Draw(640, 320, 0xffffffff, "1匹の犠牲でタイムを増やそう！ドラッグアンドドロップで火に持っていこう");
		break;

	case 30: // ※★このステートでプレイヤーに羊をドラッグアンドドロップで焼かせる！
		break;

	case 31: // お見事！羊を焼けたね！肉には焼き加減があるぞ！
		tdnText::Draw(640, 320, 0xffffffff, "お見事！羊を焼けたね！肉には焼き加減があるぞ！");
		break;
	case 32: // 肉がパーフェクトになるまで待つ(操作受付はしない)
		break;

	case 33: // このタイミングがパーフェクトだ！焼き羊をクリックしてみよう！ ※ここで焼き羊クリックコマンド
		tdnText::Draw(640, 320, 0xffffffff, "このタイミングがパーフェクトだ！焼き羊をクリックしてみよう！");
		break;

	case 34:// 肉バウンド中
		break;

	case 35:// 狼ポップアップ中
		break;

	case 36:// 肉をドラックアンドドロップでオオカミのレーンにおいてみよう！
		tdnText::Draw(640, 320, 0xffffffff, "肉をドラックアンドドロップでオオカミのレーンにおいてみよう！");
		break;

	case 37:// ※★このステートでプレイヤーに「真ん中レーンに」肉ドラッグアンドドロップさせる！！！
		break;

	case 38:// 少し待って狼出現
		break;

	case 39: // 走って肉食うまでの空白
		break;

	case 40: // Delicious！オオカミが肉を食ってるぞ！
		tdnText::Draw(640, 320, 0xffffffff, "Delicious！オオカミが肉を食ってるぞ！");
		break;

	case 41: // デブオオカミが生成されるまでの空白
		break;

	case 42:
		// オオカミが太ったぞ！太った羊と同じ要領で、群れで押し込もう！ ※ここで犬クリックコマンド
		tdnText::Draw(640, 320, 0xffffffff, "オオカミが太ったぞ！太った羊と同じ要領で、群れで押し込もう");
		break;

	case 43: // オオカミが入るまでの待機時間
		break;

	case 44: // 太った狼を押し出すとタイムが回復するぞ！
		tdnText::Draw(640, 320, 0xffffffff, "太った狼を押し出すとタイムが回復するぞ！");
		break;

	case 45:
		// 適当な待機時間
		break;

	case 46: // 基本ルールはこれで終了。良い羊焼きライフを
		tdnText::Draw(640, 320, 0xffffffff, "基本ルールはこれで終了。良い羊焼きライフを");
		break;

	case 47: // フェード終了まち
		break;
	}
}

void sceneTutorial::DebugText()
{
	//	デバッグ用_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//DEBUG_TEXT::DebugText("TEST%d, %s, %d, %c", 1, "aaa", 741, 'A');
	//	_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
}

void sceneTutorial::PoseUpdate()
{
	// カーソル戻す
	pointer->SetState(MousePointer::STATE::NOMAL);

	// ウェイトタイマー
	static int waitTimer = 0;
	static bool againFlag = false;

	// ポーズ
	if (KeyBoard(KB_P) == 3)
	{
		state = SCENE::MAIN;
	}




	m_again.pic->Update();
	m_again.rip->Update();

	m_stop.pic->Update();
	m_stop.rip->Update();


	// 数値
	m_poseTimer.one->Update();
	m_poseTimer.two->Update();
	m_poseTimer.three->Update();



	switch (m_poseState)
	{
	case sceneTutorial::POSE_STATE::START:

		// 後でSE全部ここで止めてもらお
		se->Stop_all();
		bgm->SetVolume("MAIN", .5f);

		// クリックしたSE
		se->Play("ポーズ");

		m_poseState = POSE_STATE::EXE;
		waitTimer = 0;
		againFlag = false;
		pointer->SetState(MousePointer::STATE::NOMAL);

		break;
	case sceneTutorial::POSE_STATE::EXE:
	{
		/***************************/
		// アゲインの動き
		// 距離計算 
		float len = Math::Length((float)m_again.x + 240, (float)m_again.y + 64, tdnMouse::GetPos().x, tdnMouse::GetPos().y);
		if (len < 220 && 60 > abs((m_again.y + 64) - tdnMouse::GetPos().y))// ＋Yをカットする
		{
			// 色を実体化
			m_again.pic->GetObj()->SetARGB(0xffffffff);
			if (m_again.isAction == false)
			{
				m_again.isAction = true;
				m_again.pic->Action();

				// カーソルを合わせたSEの再生
				se->Play("カーソル");
			}

			// クリックすると
			if (tdnMouse::GetLeft() == 3)
			{
				// 選択クリック音
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

		//★
		// わっしょいしょいのしょいのすけ
		if (againFlag == true)
		{
			waitTimer++;
			if (waitTimer > 22)
			{
				waitTimer = 0;
				m_poseState = POSE_STATE::COUNT;	//  カウントステートへ
			}

		}
		/****************************/

		/***************************/
		// ストップの動き
		// 距離計算 
		len = Math::Length((float)m_stop.x + 240, (float)m_stop.y + 64, tdnMouse::GetPos().x, tdnMouse::GetPos().y);
		if (len < 220 && 60 > abs((m_stop.y + 64) - tdnMouse::GetPos().y))// ＋Yをカットする
		{
			// 色を実体化
			m_stop.pic->GetObj()->SetARGB(0xffffffff);
			if (m_stop.isAction == false)
			{
				m_stop.isAction = true;
				m_stop.pic->Action();

				// カーソルを合わせたSEの再生
				se->Play("カーソル");
			}

			// クリックすると
			if (tdnMouse::GetLeft() == 3)
			{
				// 選択クリック音
				se->Play("キャンセル");

				m_stop.rip->Action();
				FadeControl::Setting(FadeControl::MODE::FADE_OUT, 40.0f);
				m_poseState = POSE_STATE::TITLE;	//  タイトルステートへ
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
			// BGMの音戻す
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

	// 焼肉描画
	NikuMgr->RenderYakiniku();

	g_pSheepMgr->Render();
	EnemyMgr->Render();
	BokusouMgr->Render();

	// ステージの前描画
	stage->RenderFront();

	NikuMgr->RenderNiku();

	Particle2dManager::Render();

	// デブたち描画
	g_pSheepMgr->RenderFat();
	EnemyMgr->RenderFat();
	stage->RenderFront2();
	BokusouMgr->RenderFront();
	NikuMgr->RenderFront();
	NumberEffect.Render();

	// ステージの前描画
	//stage->RenderFront();
	//NumberEffect.Render();

	EffectMgr.Render();

	switch (m_poseState)
	{
	case sceneTutorial::POSE_STATE::COUNT:
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x55000000); //黒い板

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
		tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000); //黒い板

		m_again.pic->Render(m_again.x, m_again.y);
		m_again.rip->Render(m_again.x, m_again.y, RS::ADD);

		m_stop.pic->Render(m_stop.x, m_stop.y);
		m_stop.rip->Render(m_stop.x, m_stop.y, RS::ADD);
		break;
	}
	

	//tdnPolygon::Rect(400, 300, 120, 120, RS::COPY, 0xff00ffff);
	//tdnPolygon::Rect(400, 500, 120, 120, RS::COPY, 0xffff00ff);



}
