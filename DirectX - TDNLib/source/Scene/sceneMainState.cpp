#include "TDNLIB.h"
#include "../Scene/SceneMain.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "../AI/Message/Message.h"
#include "../Stage/Stage.h"
#include "sceneMainState.h"
#include "../IconButton/IconButton.h"
#include "../UI/UI.h"
#include "../Sound/SoundManager.h"

// sceneMainのグローバル変数から
extern int RippleCount;

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
//			メイン
//*****************************************************************************************************************************
sceneMainGame* sceneMainGame::GetInstance()
{
	// ここに変数を作る
	static sceneMainGame instance;
	return &instance;
}

// 入り口
void sceneMainGame::Enter(sceneMain *pMain)
{
	// ステージの番号に応じて人間を読み込む
	RippleCount = Stage::LoadPerson();
}

// 実行中
void sceneMainGame::Execute(sceneMain *pMain)
{
	if (KeyBoard('Z') == 1)
	{
		//	rip->Action();
		float ramX = (float)(rand() % 100) - 50;
		float ramZ = (float)(rand() % 100) - 50;

		GossipRippleMgr.AddRipple(RIPPLE_TYPE::RED, Vector3(ramX, 0, ramZ));
	}
	if (KeyBoard('X') == 1)
	{	
		// セット
		PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	if (tdnMouse::GetLeft() == 3 && RippleOK)
	{
		FOR(PersonMgr.GetPersonSize())
		{
			if (!PersonMgr.GetPerson(i)->IsShed() && (Math::WorldToScreen(PersonMgr.GetPerson(i)->GetPos()) - tdnMouse::GetPos()).Length() < 50)
			{
				RippleCount--;

				// 噂連鎖が止まるまで使えなくする
				RippleOK = false;

				// セット
				PersonMgr.GetPerson(i)->ActionGossipRipple();

				// UIの吹き出し
				UIMgr.PushHukidashi(PersonMgr.GetPerson(i)->GetPos(), HUKIDASHI_TYPE::ORIGIN);

				// 音のトーン戻す
				se->SetTone("波紋出す", 0);

				break;
			}
		}
		//PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	// 噂連鎖が止まったら
	if (PersonMgr.isJudgeMoment())
	{
		// クリックで流せるようになる
		RippleOK = true;

		if (PersonMgr.isAllShed())
		{
			pMain->SetMode(sceneMain::MODE::GAMECLEAR);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());
		}
		else if (RippleCount == 0)
		{
			pMain->SetMode(sceneMain::MODE::GAMEOVER);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetFSM()->ChangeState(sceneMainGameOver::GetInstance());
		}
	}

	//rip->Update();
	GossipRippleMgr.Update();

	PersonMgr.Update();
}

// 出口
void sceneMainGame::Exit(sceneMain *pMain)
{

}

void sceneMainGame::Render(sceneMain * pMain)
{
}

bool sceneMainGame::OnMessage(sceneMain *pMain, const Message &msg)
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