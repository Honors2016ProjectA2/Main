#include "BasePersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"


/***************************************/
//	グローバルステート
/***************************************/
PersonGlobalState* PersonGlobalState::GetInstance()
{
	// ここに変数を作る
	static PersonGlobalState instance;
	return &instance;
}

// 入り口
void PersonGlobalState::Enter(BasePerson *pPerson)
{

}

// 実行中
void PersonGlobalState::Execute(BasePerson *pPerson)
{

}

// 出口
void PersonGlobalState::Exit(BasePerson *pPerson)
{

}

void PersonGlobalState::Render(BasePerson * pPerson)
{
	
}

//
bool PersonGlobalState::OnMessage(BasePerson *pPerson, const Message &msg)
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
//	待機
/***************************************/
PersonWait* PersonWait::GetInstance()
{
	// ここに変数を作る
	static PersonWait instance;
	return &instance;
}

// 入り口
void PersonWait::Enter(BasePerson *pPerson)
{

}

// 実行中
void PersonWait::Execute(BasePerson *pPerson)
{
	//pPerson->SetAngle(pPerson->GetAngle() + 0.1f);

	//// ここで波紋がIsEndになったら
	//pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance()->GetInstance());

	//if ()
	//{

	//}

	// m_isShed = true になったら流した場所へ行く
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance());
	}
}

// 出口
void PersonWait::Exit(BasePerson *pPerson)
{

}

void PersonWait::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool PersonWait::OnMessage(BasePerson *pPerson, const Message &msg)
{	
	// 流もう流していたら
	//if (pPerson->IsShed() == false)
	//{
	//	MyDebugString("ID番号　%d にMSGが届きましたかが、噂を流した後なので弾きました\n", pPerson->GetID());
	//	return false;
	//}

	//// メッセージタイプ
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::Riple_Red:

	//	// 自分の噂の波紋を出す。
	//	pPerson->ActionGossipRipple();

	//	return true;

	//	break;
	//default:

	//	break;
	//}//end switch
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	流してる時の待機
/***************************************/

PersonShedWait* PersonShedWait::GetInstance()
{
	// ここに変数を作る
	static PersonShedWait instance;
	return &instance;
}

// 入り口
void PersonShedWait::Enter(BasePerson *pPerson)
{
	// 流したフラグＯＮ!
	//pPerson->SetIsShed(true);
	pPerson->GetRipple()->Action();

	// (仮)噂を流す時にモーションを付ける事だって自由自在
	pPerson->GetObj()->SetMotion(0);

}

// 実行中
void PersonShedWait::Execute(BasePerson *pPerson)
{
	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従
	pPerson->GetRipple()->Update();

	// 波紋が出終わったら
	if (pPerson->GetRipple()->IsEnd()==true)
	{
		// 終わる
		pPerson->GetFSM()->ChangeState(PersonEndWait::GetInstance());
	}

}

// 出口
void PersonShedWait::Exit(BasePerson *pPerson)
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

}

void PersonShedWait::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool PersonShedWait::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	流した後の待機
/***************************************/

PersonEndWait* PersonEndWait::GetInstance()
{
	// ここに変数を作る
	static PersonEndWait instance;
	return &instance;
}

// 入り口
void PersonEndWait::Enter(BasePerson *pPerson)
{
	// (仮)噂を流す時にモーションを付ける事だって自由自在
	pPerson->GetObj()->SetMotion(0);
}

// 実行中
void PersonEndWait::Execute(BasePerson *pPerson)
{

}

// 出口
void PersonEndWait::Exit(BasePerson *pPerson)
{

}

void PersonEndWait::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "shed");
}

bool PersonEndWait::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	走る
/***************************************/

PersonRun* PersonRun::GetInstance()
{
	// ここに変数を作る
	static PersonRun instance;
	return &instance;
}

// 入り口
void PersonRun::Enter(BasePerson *pPerson)
{

	pPerson->m_orgPos = pPerson->GetPos();
	pPerson->m_len = 30;
	pPerson->m_trunPos = pPerson->m_orgPos + Vector3(pPerson->m_len, 0, 0);
	pPerson->m_trunFlag = false;

	// (仮)噂を流す時にモーションを付ける事だって自由自在
	pPerson->GetObj()->SetMotion(1);
}

// 実行中
void PersonRun::Execute(BasePerson *pPerson)
{
	// ターンフラグ
	if (pPerson->m_trunFlag == true)
	{
		pPerson->SetAngle(-PI / 2);
		pPerson->SetMove(Vector3(-0.25f,0,0));
		// その距離以下なら
		if (pPerson->m_orgPos.x >= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = false;
		}
	}
	else
	{
		pPerson->SetAngle(PI / 2);
		pPerson->SetMove(Vector3(+0.25f, 0, 0));
		// その距離以下なら
		if (pPerson->m_trunPos.x <= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = true;
		}
	}




	// m_isShed = true になったら流した場所へ行く
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(PersonShedRun::GetInstance());
	}
}

// 出口
void PersonRun::Exit(BasePerson *pPerson)
{

}

void PersonRun::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool PersonRun::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	流してる時の走っている状態
/***************************************/

PersonShedRun* PersonShedRun::GetInstance()
{
	// ここに変数を作る
	static PersonShedRun instance;
	return &instance;
}

// 入り口
void PersonShedRun::Enter(BasePerson *pPerson)
{
	// 流したフラグＯＮ!
	//pPerson->SetIsShed(true);
	pPerson->GetRipple()->Action();

	// (仮)噂を流す時にモーションを付ける事だって自由自在
	pPerson->GetObj()->SetMotion(1);

}

// 実行中
void PersonShedRun::Execute(BasePerson *pPerson)
{

	// 波紋
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// 常にプレイヤー追従
	pPerson->GetRipple()->Update();


	// ターンフラグ
	if (pPerson->m_trunFlag == true)
	{
		pPerson->SetAngle(-PI / 2);
		pPerson->SetMove(Vector3(-0.25f, 0, 0));
		// その距離以下なら
		if (pPerson->m_orgPos.x >= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = false;
		}
	}
	else
	{
		pPerson->SetAngle(PI / 2);
		pPerson->SetMove(Vector3(+0.25f, 0, 0));
		// その距離以下なら
		if (pPerson->m_trunPos.x <= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = true;
		}
	}



	// 波紋が出終わったら
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		// 終わる
		pPerson->GetFSM()->ChangeState(PersonEndRun::GetInstance());
	}

}

// 出口
void PersonShedRun::Exit(BasePerson *pPerson)
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

}

void PersonShedRun::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool PersonShedRun::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	流した後の待機
/***************************************/

PersonEndRun* PersonEndRun::GetInstance()
{
	// ここに変数を作る
	static PersonEndRun instance;
	return &instance;
}

// 入り口
void PersonEndRun::Enter(BasePerson *pPerson)
{
	// (仮)噂を流す時にモーションを付ける事だって自由自在
	pPerson->GetObj()->SetMotion(1);
}

// 実行中
void PersonEndRun::Execute(BasePerson *pPerson)
{
	// ターンフラグ
	if (pPerson->m_trunFlag == true)
	{
		pPerson->SetAngle(-PI / 2);
		pPerson->SetMove(Vector3(-0.25f, 0, 0));
		// その距離以下なら
		if (pPerson->m_orgPos.x >= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = false;
		}
	}
	else
	{
		pPerson->SetAngle(PI / 2);
		pPerson->SetMove(Vector3(+0.25f, 0, 0));
		// その距離以下なら
		if (pPerson->m_trunPos.x <= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = true;
		}
	}
}

// 出口
void PersonEndRun::Exit(BasePerson *pPerson)
{

}

void PersonEndRun::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "shed");
}

bool PersonEndRun::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}