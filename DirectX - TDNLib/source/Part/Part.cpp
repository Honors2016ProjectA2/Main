#include "TDNLIB.h"
#include "../Scene/SceneMain.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "../AI/Message/Message.h"
#include "../Stage/Stage.h"
#include "Part.h"
#include "../IconButton/IconButton.h"

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
	m_RippleCount = Stage::LoadPerson();
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

	if (tdnMouse::GetLeft() == 3 && m_RippleCount > 0)
	{
		FOR(PersonMgr.GetPersonSize())
		{
			if ((Math::WorldToScreen(PersonMgr.GetPerson(i)->GetPos()) - tdnMouse::GetPos()).Length() < 50)
			{
				m_RippleCount--;
				PersonMgr.GetPerson(i)->ActionGossipRipple();
			}
		}
		//PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	if (PersonMgr.isJudgeMoment())
	{
		if (PersonMgr.isAllShed())
		{
			pMain->SetMode(sceneMain::MODE::GAMECLEAR);
			pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());
		}
		else if (m_RippleCount == 0)
		{
			pMain->SetMode(sceneMain::MODE::GAMEOVER);
			pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
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

bool sceneMainGameOver::OnMessage(sceneMain *pMain, const Message &msg)
{
	// 出ていけ！
	return false;
}