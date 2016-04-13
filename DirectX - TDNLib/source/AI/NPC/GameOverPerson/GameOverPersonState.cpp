#include "GameOverPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\GameOverPerson\GameOverPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	グローバルステート
/***************************************/
GameOverPersonGlobalState* GameOverPersonGlobalState::GetInstance()
{
	// ここに変数を作る
	static GameOverPersonGlobalState instance;
	return &instance;
}

// 入り口
void GameOverPersonGlobalState::Enter(GameOverPerson *pPerson)
{

}

// 実行中
void GameOverPersonGlobalState::Execute(GameOverPerson *pPerson)
{

}

// 出口
void GameOverPersonGlobalState::Exit(GameOverPerson *pPerson)
{

}

void GameOverPersonGlobalState::Render(GameOverPerson * pPerson)
{

}

//
bool GameOverPersonGlobalState::OnMessage(GameOverPerson *pPerson, const Message &msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::Riple_Red:

		//printf("赤い波紋に当たったんご\n");
		//MyDebugString("赤い波紋に当たったんご\n");

		// 赤い波紋に当たったよ

		return true;

		break;
	default:

		break;
	}//end switch

	 // Flaseで返すとグローバルステートなので正真正銘の終り
	return false;
}



/***************************************/
//	最初
/***************************************/
StartGameOver* StartGameOver::GetInstance()
{
	// ここに変数を作る
	static StartGameOver instance;
	return &instance;
}

// 入り口
void StartGameOver::Enter(GameOverPerson *pPerson)
{

}

// 実行中
void StartGameOver::Execute(GameOverPerson *pPerson)
{
	// このキャラクターは噂を流さない
	// m_isShed = true になったら流した場所へ行く
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(ShedGameOver::GetInstance());
	}
}

// 出口
void StartGameOver::Exit(GameOverPerson *pPerson)
{

}

void StartGameOver::Render(GameOverPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartGameOver::OnMessage(GameOverPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	噂を流した
/***************************************/
ShedGameOver* ShedGameOver::GetInstance()
{
	// ここに変数を作る
	static ShedGameOver instance;
	return &instance;
}

// 入り口
void ShedGameOver::Enter(GameOverPerson *pPerson)
{
	// 流したフラグＯＮ!
	//pPerson->SetIsShed(true);
	pPerson->GetRipple()->Action();

	// (仮)噂を流す時にモーションを付ける事だって自由自在
	pPerson->GetObj()->SetMotion(0);

	// 音
	se->Play("波紋出す", Math::WorldToScreen(pPerson->GetPos()));
}

// 実行中
void ShedGameOver::Execute(GameOverPerson *pPerson)
{
	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従
	pPerson->GetRipple()->Update();


	// m_isShed = true になったら流した場所へ行く
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndGameOver::GetInstance());
	}
}

// 出口
void ShedGameOver::Exit(GameOverPerson *pPerson)
{
	// 人マネージャーにMSG送信
	/*
	[内容]波紋内にいたやつ全員
	噂を流すステートへ
	*/

	RIPPLE_INFO ex;
	ex.type = pPerson->GetPersonType();
	ex.pos = pPerson->GetPos();
	ex.size = pPerson->GetRipSize();// (仮)　手打ちでサイズ指定

	// ★★★
	// ゲームオーバーをジャッジ君へ送る
	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::JUDGE_MNG,
		GAMEOVER,
		(void*)&ex	// [追記情報]自分のタイプを送る
		);
}

void ShedGameOver::Render(GameOverPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedGameOver::OnMessage(GameOverPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	終り
/***************************************/
EndGameOver* EndGameOver::GetInstance()
{
	// ここに変数を作る
	static EndGameOver instance;
	return &instance;
}

// 入り口
void EndGameOver::Enter(GameOverPerson *pPerson)
{

}

// 実行中
void EndGameOver::Execute(GameOverPerson *pPerson)
{

}

// 出口
void EndGameOver::Exit(GameOverPerson *pPerson)
{

}

void EndGameOver::Render(GameOverPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndGameOver::OnMessage(GameOverPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

