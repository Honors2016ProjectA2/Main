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
#include "Tutorial.h"
#include "../Camera/Camera.h"
#include "../Stage/Stage.h"

//******************************************************************
//		グローバル変数
//******************************************************************
//GossipRipple* rip;
iexMesh* school;

// 現状、誰に持たせるべきか分からないのでここにおいとく
int RippleCount(0);

// イントロ飛ばすのめんどくさかったらfalseにすると幸せになれる
static const bool IntroON = true;


//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneMain::Initialize()
{
	// マウス初期化
	tdnMouse::Initialize(TRUE);

	// カメラ初期化
	CameraMgr.Initialize(IntroON);
	//tdnView::Init();
	// tdnView::Set(Vector3(.0f, 95.0f, -80.0f), Vector3(.0f, .0f, .0f));


	//rip = new GossipRipple(RIPPLE_TYPE::RED, Vector3(-20, 0, 20));

	GossipRippleMgr;
	JudgeMgr;

	// ★ステージの読み込みをここに移植しました。(JudgeManagerのクリア条件の設定をこの中でやっているので、それより前にクリア条件を参照するとうまくいかなくなったため)
	// ステージの番号に応じて人間を読み込む
	RippleCount = StageMgr.LoadPerson();
	// でばっぐ用
	//PersonMgr.AddPerson(PERSON_TYPE::GAMEOVER, Vector3(20, 0, 20));
	//PersonMgr.AddPerson(PERSON_TYPE::START, Vector3(25, 0, 0));
	//PersonMgr.AddPerson(PERSON_TYPE::WAIT, Vector3(-25, 0, 0));
	//PersonMgr.AddPerson(PERSON_TYPE::START, Vector3(-50, 0, 0));
	//PersonMgr.AddPerson(PERSON_TYPE::GOAL, Vector3(-80, 0, 0));

	// ステージモデル初期化
	school = new iexMesh("Data/Stage/school.imo");
	school->SetScale(0.8f);
	school->SetAngle(PI);
	school->Update();

	// ボタン管理さん初期化
	m_pButtonMgr = new IconButtonManager;
	m_pButtonMgr->TextLoad("DATA/Text/IconButton/main.txt");

	// ボタンの有効・無効
	//m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::YES, EN_DIS_TYPE::DISABLE_VANISH);
	//m_pButtonMgr->SetEnDis((UINT)BUTTON_ID::NO, EN_DIS_TYPE::DISABLE_VANISH);

	// シーンメインステートマシン初期化
	m_pStateMachine = new StateMachine<sceneMain>(this);
	m_pStateMachine->SetGlobalState(sceneMainGlobalState::GetInstance());// グローバル
	if (IntroON) m_pStateMachine->SetCurrentState(sceneMainIntro::GetInstance());
	else         m_pStateMachine->SetCurrentState(sceneMainSetPart::GetInstance());
	g_GameState = GAME_STATE::INTRO;

	// ゲームクリアの画像初期化
	//lpGameClear = new tdn2DObj("DATA/UI/game_clear.png");
	//lpGameOver = new tdn2DObj("DATA/UI/game_over.png");

	// UI管理さん初期化
	UIMgr.Initialize();

	// フェード初期化
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
	CameraMgr.Release();
	//delete lpGameClear;
	//delete lpGameOver;

	UIMgr.Release();
}


//******************************************************************
//		更新・描画
//******************************************************************
bool sceneMain::Update()
{
	// マウス更新
	tdnMouse::Update();

	// ステートマシン更新
	m_pStateMachine->Update();

	// ボタン更新
	m_pButtonMgr->Update(tdnMouse::GetPos());

	// UIマネージャー更新
	UIMgr.Update();

	// カメラ更新
	CameraMgr.Update();

	// ジャッジマネージャー更新
	JudgeMgr.Update();

	// フェード更新
	Fade::Update();

	// ゲームクリアかゲームオーバー状態なら
	//if (m_mode != MODE::MAIN)
	//{
	//	// まだフェードしてないとき
	//	if (Fade::alpha == 0)
	//	{
	//		// ブラックフェードON
	//		Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
	//	}
	//}

	// 左クリック
	if (tdnMouse::GetLeft() == 3)
	{
		// ゲームクリア状態なら
		if (g_GameState == GAME_STATE::GAME_CLEAR)
		{
			// セレクト画面に戻る
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

		// ゲームオーバー状態なら
		else if (g_GameState == GAME_STATE::GAME_OVER)
		{
			switch ((BUTTON_ID)m_pButtonMgr->GetInButtonNo())
			{
			case BUTTON_ID::CONTINUE_YES:	// リトライ
				// もう一度シーンメインを読み込む
				MainFrame->ChangeScene(new sceneMain);
				return true;
				break;
			case BUTTON_ID::CONTINUE_NO:	// やめる
				// ステージセレクト画面に飛ぶ
				MainFrame->ChangeScene(new sceneSelect);
				return true;
				break;
			}
		}

		// リトライボタン
		else if (m_pButtonMgr->GetInButtonNo() == 2)
		{
			// もう一度読み込む
			MainFrame->ChangeScene(new sceneMain);
			return true;
		}
	}


	return true;	
}

void sceneMain::Render()
{
	//tdnView::Activate();
	//tdnView::Clear(0xff005522);
	CameraMgr.Activate();

	school->Render();

	//rip->Render();
	GossipRippleMgr.Render();

	PersonMgr.Render();
	m_pStateMachine->Render();

	//UIMgr.Render();	// イントロで表示させないようにしたかったのでStateのRender2Dに移植しました

	m_pStateMachine->Render2D();

	// フェードはUIに書く(フェードの上に画像を置くことがあるから)
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
