#include "GoalPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\GoalPerson\GoalPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	グローバルステート
/***************************************/
GoalPersonGlobalState* GoalPersonGlobalState::GetInstance()
{
	// ここに変数を作る
	static GoalPersonGlobalState instance;
	return &instance;
}

// 入り口
void GoalPersonGlobalState::Enter(GoalPerson *pPerson)
{

}

// 実行中
void GoalPersonGlobalState::Execute(GoalPerson *pPerson)
{

}

// 出口
void GoalPersonGlobalState::Exit(GoalPerson *pPerson)
{

}

void GoalPersonGlobalState::Render(GoalPerson * pPerson)
{

}

//
bool GoalPersonGlobalState::OnMessage(GoalPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートなので正真正銘の終り
	return false;
}



/***************************************/
//	最初
/***************************************/
GoalStart* GoalStart::GetInstance()
{
	// ここに変数を作る
	static GoalStart instance;
	return &instance;
}

// 入り口
void GoalStart::Enter(GoalPerson *pPerson)
{

}

// 実行中
void GoalStart::Execute(GoalPerson *pPerson)
{
	// m_isShed = true になったら流した場所へ行く
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(GoalShed::GetInstance());
	}
}

// 出口
void GoalStart::Exit(GoalPerson *pPerson)
{

}

void GoalStart::Render(GoalPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}


bool GoalStart::OnMessage(GoalPerson *pPerson, const Message &msg)
{

	 // Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	噂を流した
/***************************************/
GoalShed* GoalShed::GetInstance()
{
	// ここに変数を作る
	static GoalShed instance;
	return &instance;
}

// 入り口
void GoalShed::Enter(GoalPerson *pPerson)
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
void GoalShed::Execute(GoalPerson *pPerson)
{
	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従
	pPerson->GetRipple()->Update();


	// m_isShed = true になったら流した場所へ行く
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(GoalEnd::GetInstance());
	}
}

/*
	★ゴールの人はここが違う
*/
// 出口
void GoalShed::Exit(GoalPerson *pPerson)
{
	// 人マネージャーにMSG送信
	/*
	[内容]波紋内にいたやつ全員
	噂を流すステートへ
	*/

	RIPPLE_INFO ex;
	ex.type = pPerson->GetPersonType();
	ex.pos = pPerson->GetPos();
	ex.size = 30;// (仮)　手打ちでサイズ指定

	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::PERSON_MNG,
		RIPPLE_VS_PERSON,
		(void*)&ex	// [追記情報]自分のタイプを送る
		);

	// ジャッジ君にゴールを知らせる
	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::JUDGE_MNG,
		GOAL_GOSSIP,
		(void*)&ex	// [追記情報]自分のタイプを送る
		);
}

void GoalShed::Render(GoalPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool GoalShed::OnMessage(GoalPerson *pPerson, const Message &msg)
{


	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	終り
/***************************************/
GoalEnd* GoalEnd::GetInstance()
{
	// ここに変数を作る
	static GoalEnd instance;
	return &instance;
}

// 入り口
void GoalEnd::Enter(GoalPerson *pPerson)
{

}

// 実行中
void GoalEnd::Execute(GoalPerson *pPerson)
{

}

// 出口
void GoalEnd::Exit(GoalPerson *pPerson)
{

}

void GoalEnd::Render(GoalPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool GoalEnd::OnMessage(GoalPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



