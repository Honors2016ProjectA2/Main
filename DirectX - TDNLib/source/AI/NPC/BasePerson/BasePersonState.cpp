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

	//if ()
	//{

	//}
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
//	�����Ă鎞�̑ҋ@
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
	// �������t���O�n�m!
	pPerson->SetIsShed(true);
	pPerson->GetRipple()->Action();


}

// ���s��
void PersonShedWait::Execute(BasePerson *pPerson)
{
	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]
	pPerson->GetRipple()->Update();

	// �g�䂪�o�I�������
	if (pPerson->GetRipple()->IsEnd()==true)
	{
		// �I���
		pPerson->GetFSM()->ChangeState(PersonEndWait::GetInstance());
	}

}

// �o��
void PersonShedWait::Exit(BasePerson *pPerson)
{
	// �l�}�l�[�W���[��MSG���M
	/*
		[���e]�g����ɂ�����S��
		�\�𗬂��X�e�[�g��
	*/

	RIPPLE_INFO ex;
	ex.type = pPerson->GetPersonType();
	ex.pos = pPerson->GetPos();
	ex.size = 30;// (��)�@��ł��ŃT�C�Y�w��

	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::PERSON_MNG,
		RIPPLE_VS_PERSON,
		(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
		);

}

bool PersonShedWait::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	��������̑ҋ@
/***************************************/

PersonEndWait* PersonEndWait::GetInstance()
{
	// �����ɕϐ������
	static PersonEndWait instance;
	return &instance;
}

// �����
void PersonEndWait::Enter(BasePerson *pPerson)
{


}

// ���s��
void PersonEndWait::Execute(BasePerson *pPerson)
{
	pPerson->SetAngle(pPerson->GetAngle() + 0.1f);
}

// �o��
void PersonEndWait::Exit(BasePerson *pPerson)
{

}

bool PersonEndWait::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

