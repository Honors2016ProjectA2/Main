#include "MessageDispatcher.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\Entity\EntityManager.h"


// �V���O���g��
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
		// �����ɗ����Ƃ������Ƃ͎�M�ґ����Ή����ĂȂ��Ēe���ꂽ�\��������
		printf("���b�Z�[�W����������܂���\n");
	}
}

// ���b�Z�[�W����
void MessageDispatcher::DispatchMessage(double  delay,
	int    sender,
	int    receiver,
	int    msg,
	void*  ExtraInfo)
{

	//	���M�҂Ǝ�M�҂̃A�h���X�擾
	BaseGameEntity* pSender = EntityMgr->GetEntityFromID(sender);
	BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

	//��M�҂�ID���L����
	if (pReceiver == NULL)
	{
		printf("[�x��]����ID������M�҂͂��܂���\n");
		return;
	}

	//�@�d��쐬
	Message telegram(delay, sender, receiver, msg, ExtraInfo);

	//�����x�����Ȃ������炷���ɑ��M�I                     
	if (delay <= 0.0f)
	{
		printf("���b�Z�[�W���M\n");

		// ��M�҂ɓd��𑗂邺
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


