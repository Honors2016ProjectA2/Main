#include "WaitPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\WaitPerson\WaitPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	グローバルステート
/***************************************/
WaitPersonGlobalState* WaitPersonGlobalState::GetInstance()
{
	// ここに変数を作る
	static WaitPersonGlobalState instance;
	return &instance;
}

// 入り口
void WaitPersonGlobalState::Enter(WaitPerson *pPerson)
{

}

// 実行中
void WaitPersonGlobalState::Execute(WaitPerson *pPerson)
{

}

// 出口
void WaitPersonGlobalState::Exit(WaitPerson *pPerson)
{

}

void WaitPersonGlobalState::Render(WaitPerson * pPerson)
{

}

//
bool WaitPersonGlobalState::OnMessage(WaitPerson *pPerson, const Message &msg)
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
StartWait* StartWait::GetInstance()
{
	// ここに変数を作る
	static StartWait instance;
	return &instance;
}

// 入り口
void StartWait::Enter(WaitPerson *pPerson)
{

}

// 実行中
void StartWait::Execute(WaitPerson *pPerson)
{
	// m_isShed = true になったら流した場所へ行く
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(ShedWait::GetInstance());
	}
}

// 出口
void StartWait::Exit(WaitPerson *pPerson)
{

}

void StartWait::Render(WaitPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartWait::OnMessage(WaitPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	噂を流した
/***************************************/
ShedWait* ShedWait::GetInstance()
{
	// ここに変数を作る
	static ShedWait instance;
	return &instance;
}

// 入り口
void ShedWait::Enter(WaitPerson *pPerson)
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
void ShedWait::Execute(WaitPerson *pPerson)
{
	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従
	pPerson->GetRipple()->Update();


	// m_isShed = true になったら流した場所へ行く
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndWait::GetInstance());
	}
}

// 出口
void ShedWait::Exit(WaitPerson *pPerson)
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

	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::PERSON_MNG,
		RIPPLE_VS_PERSON,
		(void*)&ex	// [追記情報]自分のタイプを送る
		);

	// ジャッジ君に流したことを伝える
	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::JUDGE_MNG,
		MESSAGE_TYPE::SHED_GOSSIP,
		nullptr	// [追記情報]自分のタイプを送る
		);
}

void ShedWait::Render(WaitPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedWait::OnMessage(WaitPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	終り
/***************************************/
EndWait* EndWait::GetInstance()
{
	// ここに変数を作る
	static EndWait instance;
	return &instance;
}

// 入り口
void EndWait::Enter(WaitPerson *pPerson)
{

}

// 実行中
void EndWait::Execute(WaitPerson *pPerson)
{

}

// 出口
void EndWait::Exit(WaitPerson *pPerson)
{

}

void EndWait::Render(WaitPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndWait::OnMessage(WaitPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

