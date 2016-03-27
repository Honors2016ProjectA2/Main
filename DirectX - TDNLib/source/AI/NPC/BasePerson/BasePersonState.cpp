#include "BasePersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"


/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
PersonGlobalState* PersonGlobalState::GetInstance()
{
	// �����ɕϐ������
	static PersonGlobalState instance;
	return &instance;
}

// �����
void PersonGlobalState::Enter(BasePerson *pPerson)
{

}

// ���s��
void PersonGlobalState::Execute(BasePerson *pPerson)
{

}

// �o��
void PersonGlobalState::Exit(BasePerson *pPerson)
{

}

//
bool PersonGlobalState::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::Riple_Red:

		//printf("�Ԃ��g��ɓ���������\n");
		//MyDebugString("�Ԃ��g��ɓ���������\n");

		// �Ԃ��g��ɓ���������

		return true;

		break;
	default:

		break;
	}//end switch

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g�Ȃ̂Ő��^�����̏I��
	return false;
}



/***************************************/
//	�ҋ@
/***************************************/
PersonWait* PersonWait::GetInstance()
{
	// �����ɕϐ������
	static PersonWait instance;
	return &instance;
}

// �����
void PersonWait::Enter(BasePerson *pPerson)
{

}

// ���s��
void PersonWait::Execute(BasePerson *pPerson)
{
	pPerson->SetAngle(pPerson->GetAngle() + 0.1f);

	//// �����Ŕg�䂪IsEnd�ɂȂ�����
	//pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance()->GetInstance());


}

// �o��
void PersonWait::Exit(BasePerson *pPerson)
{

}

bool PersonWait::OnMessage(BasePerson *pPerson, const Message &msg)
{	
	// �����������Ă�����
	//if (pPerson->IsShed() == false)
	//{
	//	MyDebugString("ID�ԍ��@%d ��MSG���͂��܂��������A�\�𗬂�����Ȃ̂Œe���܂���\n", pPerson->GetID());
	//	return false;
	//}

	//// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::Riple_Red:

	//	// �����̉\�̔g����o���B
	//	pPerson->ActionGossipRipple();

	//	return true;

	//	break;
	//default:

	//	break;
	//}//end switch
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	��������̑ҋ@
/***************************************/

PersonShedWait* PersonShedWait::GetInstance()
{
	// �����ɕϐ������
	static PersonShedWait instance;
	return &instance;
}

// �����
void PersonShedWait::Enter(BasePerson *pPerson)
{

}

// ���s��
void PersonShedWait::Execute(BasePerson *pPerson)
{
	pPerson->SetAngle(pPerson->GetAngle() + 0.1f);
}

// �o��
void PersonShedWait::Exit(BasePerson *pPerson)
{

}

bool PersonShedWait::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

/***************************************/
//	
/***************************************/
