#include "StrongPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\StrongPerson\StrongPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	グローバルステート
/***************************************/
StrongPersonGlobalState* StrongPersonGlobalState::GetInstance()
{
	// ここに変数を作る
	static StrongPersonGlobalState instance;
	return &instance;
}

// 入り口
void StrongPersonGlobalState::Enter(StrongPerson *pPerson)
{

}

// 実行中
void StrongPersonGlobalState::Execute(StrongPerson *pPerson)
{

}

// 出口
void StrongPersonGlobalState::Exit(StrongPerson *pPerson)
{

}

void StrongPersonGlobalState::Render(StrongPerson * pPerson)
{

}

//
bool StrongPersonGlobalState::OnMessage(StrongPerson *pPerson, const Message &msg)
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
StartStrong* StartStrong::GetInstance()
{
	// ここに変数を作る
	static StartStrong instance;
	return &instance;
}

// 入り口
void StartStrong::Enter(StrongPerson *pPerson)
{

}

// 実行中
void StartStrong::Execute(StrongPerson *pPerson)
{
	// m_isShed = true になったら流した場所へ行く
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(ShedStrong::GetInstance());
	}
}

// 出口
void StartStrong::Exit(StrongPerson *pPerson)
{

}

void StartStrong::Render(StrongPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartStrong::OnMessage(StrongPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	噂を流した
/***************************************/
ShedStrong* ShedStrong::GetInstance()
{
	// ここに変数を作る
	static ShedStrong instance;
	return &instance;
}

// 入り口
void ShedStrong::Enter(StrongPerson *pPerson)
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
void ShedStrong::Execute(StrongPerson *pPerson)
{
	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従
	pPerson->GetRipple()->Update();


	// m_isShed = true になったら流した場所へ行く
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndStrong::GetInstance());
	}
}

// 出口
void ShedStrong::Exit(StrongPerson *pPerson)
{
	// 人マネージャーにMSG送信
	/*
	[内容]波紋内にいたやつ全員
	噂を流すステートへ
	*/

	RIPPLE_INFO ex;
	ex.type = pPerson->GetPersonType();
	ex.pos = pPerson->GetPos();
	// ★★★　この人はサイズがでかい
	ex.size = pPerson->GetRipSize();// (仮)　手打ちでサイズ指定

	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::PERSON_MNG,
		RIPPLE_VS_PERSON,
		(void*)&ex	// [追記情報]自分のタイプを送る
		);
}

void ShedStrong::Render(StrongPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedStrong::OnMessage(StrongPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	終り
/***************************************/
EndStrong* EndStrong::GetInstance()
{
	// ここに変数を作る
	static EndStrong instance;
	return &instance;
}

// 入り口
void EndStrong::Enter(StrongPerson *pPerson)
{

}

// 実行中
void EndStrong::Execute(StrongPerson *pPerson)
{

}

// 出口
void EndStrong::Exit(StrongPerson *pPerson)
{

}

void EndStrong::Render(StrongPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndStrong::OnMessage(StrongPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

