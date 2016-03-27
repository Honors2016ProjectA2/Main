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
	pPerson->SetAngle(pPerson->GetAngle() + 0.1f);

	//// ここで波紋がIsEndになったら
	//pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance()->GetInstance());


}

// 出口
void PersonWait::Exit(BasePerson *pPerson)
{

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
//	流した後の待機
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

}

// 実行中
void PersonShedWait::Execute(BasePerson *pPerson)
{
	pPerson->SetAngle(pPerson->GetAngle() + 0.1f);
}

// 出口
void PersonShedWait::Exit(BasePerson *pPerson)
{

}

bool PersonShedWait::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

/***************************************/
//	
/***************************************/
