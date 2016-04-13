#include "StartPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\StartPerson\StartPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	グローバルステート
/***************************************/
StartPersonGlobalState* StartPersonGlobalState::GetInstance()
{
	// ここに変数を作る
	static StartPersonGlobalState instance;
	return &instance;
}

// 入り口
void StartPersonGlobalState::Enter(StartPerson *pPerson)
{

}

// 実行中
void StartPersonGlobalState::Execute(StartPerson *pPerson)
{
	// 波紋は常に更新
	pPerson->GetRipple()->Update();
}

// 出口
void StartPersonGlobalState::Exit(StartPerson *pPerson)
{

}

void StartPersonGlobalState::Render(StartPerson * pPerson)
{

}

// ★★★
// ここで特に意味のない波紋をだす命令を受けつける
bool StartPersonGlobalState::OnMessage(StartPerson *pPerson, const Message &msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::PRODUCT_RIPPLE:

		MyDebugString("ID[%d]　が演出用波紋をだした\n", (int)pPerson->GetID());
		pPerson->GetRipple()->Action();
		// 波紋
		pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従


		return true;

		break;
	default:

		break;
	}//end switch

	 // Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	最初
/***************************************/
FirstStart* FirstStart::GetInstance()
{
	// ここに変数を作る
	static FirstStart instance;
	return &instance;
}

// 入り口
void FirstStart::Enter(StartPerson *pPerson)
{

}

// 実行中
void FirstStart::Execute(StartPerson *pPerson)
{
	// m_isShed = true になったら流した場所へ行く
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(FirstShed::GetInstance());
	}
}

// 出口
void FirstStart::Exit(StartPerson *pPerson)
{

}

void FirstStart::Render(StartPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}


// ★★★スタートパーソンはここが違う！
bool FirstStart::OnMessage(StartPerson *pPerson, const Message &msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::START_GOSSIP:

		MyDebugString("ID[%d]　が波紋スタート\n", (int)pPerson->GetID());
		pPerson->GetFSM()->ChangeState(FirstShed::GetInstance());

		return true;

		break;
	default:

		break;
	}//end switch

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	噂を流した
/***************************************/
FirstShed* FirstShed::GetInstance()
{
	// ここに変数を作る
	static FirstShed instance;
	return &instance;
}

// 入り口
void FirstShed::Enter(StartPerson *pPerson)
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
void FirstShed::Execute(StartPerson *pPerson)
{
	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従



	// m_isShed = true になったら流した場所へ行く
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(FirstEnd::GetInstance());
	}
}

// 出口
void FirstShed::Exit(StartPerson *pPerson)
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

void FirstShed::Render(StartPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool FirstShed::OnMessage(StartPerson *pPerson, const Message &msg)
{


	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	終り
/***************************************/
FirstEnd* FirstEnd::GetInstance()
{
	// ここに変数を作る
	static FirstEnd instance;
	return &instance;
}

// 入り口
void FirstEnd::Enter(StartPerson *pPerson)
{

}

// 実行中
void FirstEnd::Execute(StartPerson *pPerson)
{

}

// 出口
void FirstEnd::Exit(StartPerson *pPerson)
{

}

void FirstEnd::Render(StartPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool FirstEnd::OnMessage(StartPerson *pPerson, const Message &msg)
{

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

