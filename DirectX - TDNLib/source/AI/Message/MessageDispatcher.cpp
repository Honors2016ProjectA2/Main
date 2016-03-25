#include "MessageDispatcher.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\Entity\EntityManager.h"


// シングルトン
MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance;

	return &instance;
}


//
void MessageDispatcher::Discharge(BaseGameEntity* pReceiver,
	const Message& telegram)
{
	if (!pReceiver->HandleMessage(telegram))
	{
		// ここに来たということは受信者側が対応してなくて弾かれた可能性がある
		printf("メッセージが処理されません\n");
	}
}

// メッセージ発送
void MessageDispatcher::DispatchMessage(double  delay,
	int    sender,
	int    receiver,
	int    msg,
	void*  ExtraInfo)
{

	//	送信者と受信者のアドレス取得
	BaseGameEntity* pSender = EntityMgr->GetEntityFromID(sender);
	BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

	//受信者のIDが有効か
	if (pReceiver == NULL)
	{
		printf("[警告]このIDを持つ受信者はいません\n");
		return;
	}

	//　電報作成
	Message telegram(delay, sender, receiver, msg, ExtraInfo);

	//もし遅延がなかったらすぐに送信！                     
	if (delay <= 0.0f)
	{
		printf("メッセージ送信\n");

		// 受信者に電報を送るぜ
		Discharge(pReceiver, telegram);
	}

	//else calculate the time when the telegram should be dispatched
	//else
	//{
	//	double CurrentTime = Clock->GetCurrentTime();

	//	telegram.DispatchTime = CurrentTime + delay;

	//	//and put it in the queue
	//	PriorityQ.insert(telegram);

	//	cout << "\nDelayed telegram from " << GetNameOfEntity(pSender->ID()) << " recorded at time "
	//		<< Clock->GetCurrentTime() << " for " << GetNameOfEntity(pReceiver->ID())
	//		<< ". Msg is " << MsgToStr(msg);

	//}
}


void MessageDispatcher::DispatchDelayedMessages()
{


	////get current time
	//double CurrentTime = Clock->GetCurrentTime();

	////now peek at the queue to see if any telegrams need dispatching.
	////remove all telegrams from the front of the queue that have gone
	////past their sell by date
	//while (!PriorityQ.empty() &&
	//	(PriorityQ.begin()->DispatchTime < CurrentTime) &&
	//	(PriorityQ.begin()->DispatchTime > 0))
	//{
	//	//read the telegram from the front of the queue
	//	const Telegram& telegram = *PriorityQ.begin();

	//	//find the recipient
	//	BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);

	//	cout << "\nQueued telegram ready for dispatch: Sent to "
	//		<< GetNameOfEntity(pReceiver->ID()) << ". Msg is " << MsgToStr(telegram.Msg);

	//	//send the telegram to the recipient
	//	Discharge(pReceiver, telegram);

	//	//remove it from the queue
	//	PriorityQ.erase(PriorityQ.begin());
	//}
}


