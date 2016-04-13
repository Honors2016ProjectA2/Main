#include "StopPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\StopPerson\StopPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	グローバルステート
/***************************************/
StopPersonGlobalState* StopPersonGlobalState::GetInstance()
{
	// ここに変数を作る
	static StopPersonGlobalState instance;
	return &instance;
}

// 入り口
void StopPersonGlobalState::Enter(StopPerson *pPerson)
{

}

// 実行中
void StopPersonGlobalState::Execute(StopPerson *pPerson)
{

}

// 出口
void StopPersonGlobalState::Exit(StopPerson *pPerson)
{

}

void StopPersonGlobalState::Render(StopPerson * pPerson)
{

}

//
bool StopPersonGlobalState::OnMessage(StopPerson *pPerson, const Message &msg)
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
StartStop* StartStop::GetInstance()
{
	// ここに変数を作る
	static StartStop instance;
	return &instance;
}

// 入り口
void StartStop::Enter(StopPerson *pPerson)
{

}

// 実行中
void StartStop::Execute(StopPerson *pPerson)
{
	// このキャラクターは噂を流さない
	// m_isShed = true になったら流した場所へ行く
	//if (pPerson->IsShed() == true)
	//{
	//	pPerson->GetFSM()->ChangeState(ShedStop::GetInstance());
	//}
}

// 出口
void StartStop::Exit(StopPerson *pPerson)
{

}

void StartStop::Render(StopPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartStop::OnMessage(StopPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	噂を流した
/***************************************/
ShedStop* ShedStop::GetInstance()
{
	// ここに変数を作る
	static ShedStop instance;
	return &instance;
}

// 入り口
void ShedStop::Enter(StopPerson *pPerson)
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
void ShedStop::Execute(StopPerson *pPerson)
{
	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従
	pPerson->GetRipple()->Update();


	// m_isShed = true になったら流した場所へ行く
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndStop::GetInstance());
	}
}

// 出口
void ShedStop::Exit(StopPerson *pPerson)
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
}

void ShedStop::Render(StopPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedStop::OnMessage(StopPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	終り
/***************************************/
EndStop* EndStop::GetInstance()
{
	// ここに変数を作る
	static EndStop instance;
	return &instance;
}

// 入り口
void EndStop::Enter(StopPerson *pPerson)
{

}

// 実行中
void EndStop::Execute(StopPerson *pPerson)
{

}

// 出口
void EndStop::Exit(StopPerson *pPerson)
{

}

void EndStop::Render(StopPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndStop::OnMessage(StopPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

