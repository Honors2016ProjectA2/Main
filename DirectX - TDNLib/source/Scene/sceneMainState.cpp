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
	m_timer = 0;
}

// 実行中
void sceneMainIntro::Execute(sceneMain *pMain)
{
	// 一定時間経過で、ゲームに移行
	if (++m_timer > 180)
	{
		pMain->GetFSM()->ChangeState(sceneMainSetPart::GetInstance());
	}
}

// 出口
void sceneMainIntro::Exit(sceneMain *pMain)
{

}

void sceneMainIntro::Render(sceneMain * pMain)
{
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0x80000000);
	tdnText::Draw(640, 320, 0xffffffff, "ゲームイントロ");
	tdnText::Draw(640, 360, 0xffffffff, "%.1f", 3.0f - m_timer / 60.0f);
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
	// ステージの番号に応じて人間を読み込む
	RippleCount = StageMgr.LoadPerson();
	//PersonMgr.AddPerson(PERSON_TYPE::WAIT, Vector3(0, 0, 0));
	//PersonMgr.AddPerson(PERSON_TYPE::START, Vector3(25, 0, 0));
	//PersonMgr.AddPerson(PERSON_TYPE::WAIT, Vector3(-25, 0, 0));
	//PersonMgr.AddPerson(PERSON_TYPE::START, Vector3(-50, 0, 0));
	//PersonMgr.AddPerson(PERSON_TYPE::GOAL, Vector3(-80, 0, 0));

	// ★チュートリアルのステージならチュートリアル実行ポインタをnew
	int StageNo(StageMgr.GetStageNo());
	if (StageNo < 1)
	{
		if (m_pTutorial) delete m_pTutorial;
		m_pTutorial = new TutorialManager;
		m_pTutorial->Initialize(pMain, StageNo);
	}
	else
	{
		// 青いボタンを選択状態に
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_WHITE);
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::BLUE;

		m_pTutorial = nullptr;
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
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_WHITE);
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
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::DISABLE_WHITE);
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
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::DISABLE_WHITE);
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
	if (JudgeMgr.isGameClear())
	{
		// グローバルのゲームステートをチェンジ
		g_GameState = GAME_STATE::GAME_CLEAR;

		// ボタン有効・無効設定
		//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
		//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);

		// ステートチェンジ(ゲームステート→クリアステートに)
		pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());

		// ブラックフェードON
		Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
	}
	else if (JudgeMgr.isGameOver())
	{
		// グローバルのゲームステートをチェンジ
		g_GameState = GAME_STATE::GAME_OVER;

		// ボタン有効・無効設定
		//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
		//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);

		// ステートチェンジ(ゲームステート→ゲームオーバーステートに)
		pMain->GetFSM()->ChangeState(sceneMainGameOver::GetInstance());

		// ブラックフェードON
		Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
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

bool sceneMainGameOver::OnMessage(sceneMain *pMain, const Message &msg)
{
	// 出ていけ！
	return false;
}