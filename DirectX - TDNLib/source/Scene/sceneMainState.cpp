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
#include "../Camera/Camera.h"

// sceneMainのグローバル変数から
extern int RippleCount;

// ☆ゲームのステートをグローバルで持つ！
// UI等の様々な所で必要になることが多いのだが、ここでメンバ変数として持つと、sceneMain(MainFrame)にアクセスする必要があって不可能なのでここに置くことにした
GAME_STATE g_GameState;

//*****************************************************************************************************************************
//
//				パート担当
//
//*****************************************************************************************************************************
/***************************************/
//	グローバルステート
/***************************************/
sceneMainGlobalState* sceneMainGlobalState::GetInstance()
{
	// ここに変数を作る
	static sceneMainGlobalState instance;
	return &instance;
}

// 入り口
void sceneMainGlobalState::Enter(sceneMain *pMain)
{

}

// 実行中
void sceneMainGlobalState::Execute(sceneMain *pMain)
{

}

// 出口
void sceneMainGlobalState::Exit(sceneMain *pMain)
{

}

void sceneMainGlobalState::Render(sceneMain * pMain)
{
}

//
bool sceneMainGlobalState::OnMessage(sceneMain *pMain, const Message &msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	////case MESSAGE_TYPE::
	//
	//	break;
	//default:
	//
	//	break;
	//}//end switch

	// Flaseで返すとグローバルステートなので正真正銘の終り
	return false;
}


//*****************************************************************************************************************************
//			ゲームメインのイントロ
//*****************************************************************************************************************************
sceneMainIntro* sceneMainIntro::GetInstance()
{
	// ここに変数を作る
	static sceneMainIntro instance;
	return &instance;
}

// 入り口
void sceneMainIntro::Enter(sceneMain *pMain)
{
	// タイマー初期化
	m_timer = 0;

	// ステップ初期化
	m_step = 0;

	// 画像の位置初期化
	m_ImagePos = Vector2(-1024, 320);

	// (TODO)★★★tdn2DObjはシングルトンデストラクタで消されるとエラーが出るのでステート内のExitで消しているが、
	// 無論Exitに行くまでにゲームが終了するともれなくメモリリークが発生する
	if (JudgeMgr.GetClearFlag() == CLEAR_FLAG::GOAL_PERSON)
		m_pImage = new tdn2DObj("DATA/UI/goal_personIntro.png");
	else if (JudgeMgr.GetClearFlag() == CLEAR_FLAG::ALL_SHED)
		m_pImage = new tdn2DObj("DATA/UI/all_shedIntro.png");
	else assert(0);

	// ボタン全部OFF
	for (UINT i = 0; i < BUTTON_ID::MAX; i++)
	{
		pMain->GetButtonMgr()->SetEnDis(i, EN_DIS_TYPE::DISABLE_VANISH);
	}
}

// 実行中
void sceneMainIntro::Execute(sceneMain *pMain)
{
	GossipRippleMgr.Update();

	PersonMgr.Update();

	// 左クリックでイントロスキップ
	if (tdnMouse::GetLeft() == 3)
	{
		// ゲームに移行
		pMain->GetFSM()->ChangeState(sceneMainSetPart::GetInstance());
	}

	// カメラさんがイントロ中なのでまだ動かない(ここのイントロパートはカメラ演出が終わってからのパート)
	if (CameraMgr.isIntro()) return;

	switch (m_step)
	{
	case 0:
		/* フェードを入れるだけの仕事 */

		// ブラックフェードON
		Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);

		// 次のステップへ
		NextStep();
		break;

	case 1:
		/* クリア条件の画像を左から右へ移動させるだけの仕事 */

	{
			  // 目標座標(中央揃え的な計算で出す)
			  const int GoalX = tdnSystem::GetScreenSize().right / 2 - m_pImage->GetWidth() / 2;

			  // 特急A列車でGOGO！！  真ん中に到着したら
			  if ((m_ImagePos.x += 12) > GoalX)
			  {
				  m_ImagePos.x = (float)GoalX;
				  // 次のステップへ
				  NextStep();
			  }
	}
		break;

	case 2:
		/* クリア条件を3秒眺めるだけの仕事 */

		if (++m_timer > 180)NextStep();
		break;

	case 3:
		/* クリア条件の画像をさよならー */

		// 特急A列車でGOGO！！
		if ((m_ImagePos.x += 12) > tdnSystem::GetScreenSize().right + 320)
		{
			// フェード解除
			Fade::Set(Fade::FLAG::FADE_IN, 4, 0x00000000, 128, 0);

			// 次のステップへ
			NextStep();
		}
		break;

	case 4:
		/* フェード消えるまで待つだけの仕事 */

		// フェードが終わったら
		if (Fade::isFadeStop())
		{
			// ゲームに移行
			pMain->GetFSM()->ChangeState(sceneMainSetPart::GetInstance());
		}
		break;
	}
}

// 出口
void sceneMainIntro::Exit(sceneMain *pMain)
{
	// (TODO)★ Enter参照
	delete m_pImage;
}

void sceneMainIntro::Render(sceneMain * pMain)
{

}

void sceneMainIntro::Render2D(sceneMain *pMain)
{
	Fade::Render();

	// フェードの上に画像を置く
	m_pImage->Render((int)m_ImagePos.x, (int)m_ImagePos.y);

	tdnText::Draw(640, 620, 0x88ffffff, "イントロ再生中: 左クリックでスキップ");
}

bool sceneMainIntro::OnMessage(sceneMain *pMain, const Message &msg)
{
	// 出ていけ！
	return false;
}

//*****************************************************************************************************************************
//			人間を設置してスタートを押すパート
//*****************************************************************************************************************************
sceneMainSetPart* sceneMainSetPart::GetInstance()
{
	// ここに変数を作る
	static sceneMainSetPart instance;
	return &instance;
}

sceneMainSetPart::~sceneMainSetPart()
{
	SAFE_DELETE(m_pTutorial);
};

// 入り口
void sceneMainSetPart::Enter(sceneMain *pMain)
{
	// チュートリアルポインタを空にする(これをしないと何故かメモリリークしてしまう)
	SAFE_DELETE(m_pTutorial);

	// ★チュートリアルのステージならチュートリアル実行ポインタをnew
	int StageNo(StageMgr.GetStageNo());
	if (StageNo < 1)
	{
		m_pTutorial = new TutorialManager;
		m_pTutorial->Initialize(pMain, StageNo);
	}
	else
	{
		// ボタン有効化
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RETRY, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::START, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_WHITE);	// 青いボタンを選択状態に
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::BLUE;

		// コンティニュー用のボタン等は非表示(無効化)する
		pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_YES, EN_DIS_TYPE::DISABLE_VANISH);
		pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_NO, EN_DIS_TYPE::DISABLE_VANISH);
	}

	// 設置するときに表示するメッシュの初期化
	m_HoldMesh = nullptr;


	// ※西田くんへ　ここで設置する人間のタイプを設定してるので、完成したらここのWAITをそれぞれのタイプに設定してください

	ButtonPersonMap[SELECT_BUTTON_COLOR::BLUE] = PERSON_TYPE::WAIT;		// 一番左のボタン(多分WAITで大丈夫)
	ButtonPersonMap[SELECT_BUTTON_COLOR::RED] = PERSON_TYPE::STOP;		// 真ん中のボタン(噂を広げるのを止めるやつ)
	ButtonPersonMap[SELECT_BUTTON_COLOR::GREEN] = PERSON_TYPE::STRONG;	// 右のボタン(1.5倍の範囲を持つやつ)
}

// 実行中
void sceneMainSetPart::Execute(sceneMain *pMain)
{
	if (m_pTutorial)
	{
		// チュートリアルの説明終わって後はゲーム進行って時
		if (!m_pTutorial->Update(pMain))
		{
			delete m_pTutorial, m_pTutorial = nullptr;
		}
		return;
	}

	// キーボードによる設置人間変更
	if (KeyBoardTRG(KB_1))
	{
		ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::BLUE);
	}
	else if (KeyBoardTRG(KB_2))
	{
		ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::RED);
	}
	else if (KeyBoardTRG(KB_3))
	{
		ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::GREEN);
	}

	// ホイールによる設置人間変更
	else if (tdnMouse::GetWheelFlag() == WHEEL_FLAG::UP)
	{
		if (m_SelectButtonColor + 1 >= (int)SELECT_BUTTON_COLOR::MAX)
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)0);
		}
		else
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)(m_SelectButtonColor + 1));
		}
	}
	else if (tdnMouse::GetWheelFlag() == WHEEL_FLAG::DOWN)
	{
		if (m_SelectButtonColor == 0)
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)2);
		}
		else
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)(m_SelectButtonColor - 1));
		}
	}


	// ボタン範囲内でなければ、設置するオブジェクトを参照(描画する)
	if (pMain->GetButtonMgr()->GetInButtonNo() == IconButtonManager::NOT_IN_BUTTON && RippleCount > 0)
	{
		// (TODO)今は人間の何かしらのオブジェを持ってくる、本来は選択したボタンに応じて人を変える
		m_HoldMesh = PersonMgr.GetPerson(0)->GetObj();
		m_HoldMeshPos = Math::ScreenToWorldPlate(tdnMouse::GetPos());
	}
	else
	{
		m_HoldMesh = nullptr;
	}

	// 左クリック
	if (tdnMouse::GetLeft() == 3)
	{
		// ★ボタン判定
		switch (pMain->GetButtonMgr()->GetInButtonNo())
		{
			/* 青ボタンクリック */
		case BUTTON_ID::BLUE:

			// 設置人間変更
			ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::BLUE);
			//if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::DISABLE_WHITE);
			//}
			//else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
			//}
			//pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_WHITE);
			//m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::BLUE;
			break;

			/* 赤ボタンクリック */
		case BUTTON_ID::RED:

			// 設置人間変更
			ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::RED);
			//if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
			//}
			//else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
			//}
			//pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::DISABLE_WHITE);
			//m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::RED;
			break;

			/* 緑ボタンクリック */
		case BUTTON_ID::GREEN:

			// 設置人間変更
			ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::GREEN);
			//if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
			//}
			//else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
			//}
			//pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::DISABLE_WHITE);
			//m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::GREEN;
			break;

			/* スタートボタンクリック */
		case BUTTON_ID::START:
			// 音階を初期にする
			se->SetTone("波紋出す", 0);

			// ★波紋流しスタート！
			PersonMgr.StartGossip();

			// ボタン無効にする！
			//pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(3, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(4, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(5, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(6, EN_DIS_TYPE::DISABLE_BLACK);

			// ステートを波紋流れてるのを見るだけのステートにする
			pMain->GetFSM()->ChangeState(sceneMainGossip::GetInstance());
			break;

			/* 何もボタンに入っていないとき */
		case IconButtonManager::NOT_IN_BUTTON:

			// 設置回数が残っているなら
			if (RippleCount > 0)
			{
				RippleCount--;

				// 選んだボタンの種類に応じて人間を設置する
				PersonMgr.AddPerson(ButtonPersonMap[(SELECT_BUTTON_COLOR)m_SelectButtonColor], m_HoldMeshPos);
			}
			break;
		}
	}
	//rip->Update();
	GossipRippleMgr.Update();

	PersonMgr.Update();
}

// 出口
void sceneMainSetPart::Exit(sceneMain *pMain)
{

}

void sceneMainSetPart::Render(sceneMain * pMain)
{
	if (m_pTutorial)
	{
		m_pTutorial->Render(pMain);
		return;
	}

	if (m_HoldMesh)
	{
		m_HoldMesh->SetPos(m_HoldMeshPos);
		m_HoldMesh->Update();
		m_HoldMesh->Render(RS::COPY, .5f);
	}
}

void sceneMainSetPart::Render2D(sceneMain *pMain)
{
	// UI描画
	UIMgr.Render();

	// チュートリアルでの描画
	if (m_pTutorial)
	{
		m_pTutorial->Render2D(pMain);
	}
}

bool sceneMainSetPart::OnMessage(sceneMain *pMain, const Message &msg)
{
	// 出ていけ！
	return false;
}

void sceneMainSetPart::ChangeSelectButton(sceneMain *pMain, SELECT_BUTTON_COLOR next)
{
	// 選んでる色と同じだから出ていけ！
	if ((int)next == m_SelectButtonColor) return;

	switch (next)
	{
		/* 青ボタン */
	case SELECT_BUTTON_COLOR::BLUE:
		if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
		}
		else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
		}
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, (BYTE)((BYTE)EN_DIS_TYPE::DISABLE_WHITE | (BYTE)EN_DIS_TYPE::BORDERING));
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::BLUE;
		break;

		/* 赤ボタン */
	case SELECT_BUTTON_COLOR::RED:
		if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
		}
		else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
		}
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, (BYTE)((BYTE)EN_DIS_TYPE::DISABLE_WHITE | (BYTE)EN_DIS_TYPE::BORDERING));
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::RED;
		break;

		/* 緑ボタン */
	case SELECT_BUTTON_COLOR::GREEN:
		if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
		}
		else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
		}
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, (BYTE)((BYTE)EN_DIS_TYPE::DISABLE_WHITE | (BYTE)EN_DIS_TYPE::BORDERING));
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::GREEN;
		break;
	}
}


//*****************************************************************************************************************************
//			うわさが流れてるパート
//*****************************************************************************************************************************
sceneMainGossip* sceneMainGossip::GetInstance()
{
	// ここに変数を作る
	static sceneMainGossip instance;
	return &instance;
}

// 入り口
void sceneMainGossip::Enter(sceneMain *pMain)
{

}

// 実行中
void sceneMainGossip::Execute(sceneMain *pMain)
{
	if (KeyBoard('Z') == 1)
	{
		//	rip->Action();
		float ramX = (float)(rand() % 100) - 50;
		float ramZ = (float)(rand() % 100) - 50;

		GossipRippleMgr.AddRipple(RIPPLE_TYPE::RED, Vector3(ramX, 0, ramZ));
	}
	
	// 左クリック
	if (tdnMouse::GetLeft() == 3)
	{
		// ボタン判定
		switch (pMain->GetButtonMgr()->GetInButtonNo())
		{
		case BUTTON_ID::RETRY:
			break;
		}

		//FOR(PersonMgr.GetPersonSize())
		//{
		//	if (!PersonMgr.GetPerson(i)->IsShed() && (Math::WorldToScreen(PersonMgr.GetPerson(i)->GetPos()) - tdnMouse::GetPos()).Length() < 50)
		//	{
		//		RippleCount--;
		//
		//		// 噂連鎖が止まるまで使えなくする
		//		RippleOK = false;
		//
		//		// セット
		//		PersonMgr.GetPerson(i)->ActionGossipRipple();
		//
		//		// UIの吹き出し
		//		UIMgr.PushHukidashi(PersonMgr.GetPerson(i)->GetPos(), HUKIDASHI_TYPE::ORIGIN);
		//
		//		// 音のトーン戻す
		//		se->SetTone("波紋出す", 0);
		//
		//		break;
		//	}
		//}
		//PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	// 噂連鎖が止まったら
	//if (PersonMgr.isJudgeMoment())
	//{
		// 全員流したら
		//if (PersonMgr.isAllShed())
		//{
		//	// このセットモードはシーンメインの描画で分岐させてるから
		//	g_GameState = GAME_STATE::GAME_CLEAR;
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
		//	pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
		//
		//	// ステートチェンジ
		//	pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());
		//
		//	// ブラックフェードON
		//	Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		//}
		//
		//// 使用回数が0になったら
		//else if (RippleCount == 0)
		//{
		//	g_GameState = GAME_STATE::GAME_OVER;
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
		//	pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
		//
		//	// ステートチェンジ
		//	pMain->GetFSM()->ChangeState(sceneMainGameOver::GetInstance());
		//
		//	// ブラックフェードON
		//	Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		//}
	//}

	// ★ジャッジくんが判定を出してたら、モード切替
	if (JudgeMgr.isGameOver())
	{
		if (Fade::alpha == 0)
		{
			// ボタン有効・無効設定
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);

			// ブラックフェードON
			Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		}
		else if (Fade::alpha == 128)
		{
			// グローバルのゲームステートをチェンジ
			g_GameState = GAME_STATE::GAME_OVER;

			// ステートチェンジ(ゲームステート→ゲームオーバーステートに)
			pMain->GetFSM()->ChangeState(sceneMainGameOver::GetInstance());
		}
	}
	else if (JudgeMgr.isGameClear())
	{
		if (Fade::alpha == 0)
		{
			// ボタン有効・無効設定
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);

			// ブラックフェードON
			Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		}
		else if (Fade::alpha == 128)
		{
			// グローバルのゲームステートをチェンジ
			g_GameState = GAME_STATE::GAME_CLEAR;

			// ステートチェンジ(ゲームステート→クリアステートに)
			pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());
		}
	}


	//rip->Update();
	GossipRippleMgr.Update();

	PersonMgr.Update();
}

// 出口
void sceneMainGossip::Exit(sceneMain *pMain)
{

}

void sceneMainGossip::Render(sceneMain * pMain)
{
}

void sceneMainGossip::Render2D(sceneMain *pMain)
{
	// UI描画
	UIMgr.Render();
}

bool sceneMainGossip::OnMessage(sceneMain *pMain, const Message &msg)
{
	// 出ていけ！
	return false;
}


//*****************************************************************************************************************************
//			ゲームクリア時
//*****************************************************************************************************************************
sceneMainGameClear* sceneMainGameClear::GetInstance()
{
	// ここに変数を作る
	static sceneMainGameClear instance;
	return &instance;
}

// 入り口
void sceneMainGameClear::Enter(sceneMain *pMain)
{

}

// 実行中
void sceneMainGameClear::Execute(sceneMain *pMain)
{

}

// 出口
void sceneMainGameClear::Exit(sceneMain *pMain)
{

}

void sceneMainGameClear::Render(sceneMain * pMain)
{
}

void sceneMainGameClear::Render2D(sceneMain *pMain)
{
	// UI描画
	UIMgr.Render();
}

bool sceneMainGameClear::OnMessage(sceneMain *pMain, const Message &msg)
{
	// 出ていけ！
	return false;
}



//*****************************************************************************************************************************
//			ゲームオーバー
//*****************************************************************************************************************************
sceneMainGameOver* sceneMainGameOver::GetInstance()
{
	// ここに変数を作る
	static sceneMainGameOver instance;
	return &instance;
}

// 入り口
void sceneMainGameOver::Enter(sceneMain *pMain)
{
	// ★コンティニュー用のボタン有効化
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_YES, EN_DIS_TYPE::ENABLE);
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_NO, EN_DIS_TYPE::ENABLE);
}

// 実行中
void sceneMainGameOver::Execute(sceneMain *pMain)
{

}

// 出口
void sceneMainGameOver::Exit(sceneMain *pMain)
{

}

void sceneMainGameOver::Render(sceneMain * pMain)
{

}

void sceneMainGameOver::Render2D(sceneMain *pMain)
{
	// UI描画
	UIMgr.Render();
}

bool sceneMainGameOver::OnMessage(sceneMain *pMain, const Message &msg)
{
	// 出ていけ！
	return false;
}