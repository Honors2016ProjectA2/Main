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

void PersonGlobalState::Render(BasePerson * pPerson)
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
	//pPerson->SetAngle(pPerson->GetAngle() + 0.1f);

	//// �����Ŕg�䂪IsEnd�ɂȂ�����
	//pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance()->GetInstance());

	//if ()
	//{

	//}

	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance());
	}
}

// �o��
void PersonWait::Exit(BasePerson *pPerson)
{

}

void PersonWait::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
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
	//pPerson->SetIsShed(true);
	pPerson->GetRipple()->Action();

	// (��)�\�𗬂����Ƀ��[�V������t���鎖�����Ď��R����
	pPerson->GetObj()->SetMotion(0);

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

void PersonShedWait::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
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
	// (��)�\�𗬂����Ƀ��[�V������t���鎖�����Ď��R����
	pPerson->GetObj()->SetMotion(0);
}

// ���s��
void PersonEndWait::Execute(BasePerson *pPerson)
{

}

// �o��
void PersonEndWait::Exit(BasePerson *pPerson)
{

}

void PersonEndWait::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "shed");
}

bool PersonEndWait::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	����
/***************************************/

PersonRun* PersonRun::GetInstance()
{
	// �����ɕϐ������
	static PersonRun instance;
	return &instance;
}

// �����
void PersonRun::Enter(BasePerson *pPerson)
{

	pPerson->m_orgPos = pPerson->GetPos();
	pPerson->m_len = 30;
	pPerson->m_trunPos = pPerson->m_orgPos + Vector3(pPerson->m_len, 0, 0);
	pPerson->m_trunFlag = false;

	// (��)�\�𗬂����Ƀ��[�V������t���鎖�����Ď��R����
	pPerson->GetObj()->SetMotion(1);
}

// ���s��
void PersonRun::Execute(BasePerson *pPerson)
{
	// �^�[���t���O
	if (pPerson->m_trunFlag == true)
	{
		pPerson->SetAngle(-PI / 2);
		pPerson->SetMove(Vector3(-0.25f,0,0));
		// ���̋����ȉ��Ȃ�
		if (pPerson->m_orgPos.x >= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = false;
		}
	}
	else
	{
		pPerson->SetAngle(PI / 2);
		pPerson->SetMove(Vector3(+0.25f, 0, 0));
		// ���̋����ȉ��Ȃ�
		if (pPerson->m_trunPos.x <= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = true;
		}
	}




	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(PersonShedRun::GetInstance());
	}
}

// �o��
void PersonRun::Exit(BasePerson *pPerson)
{

}

void PersonRun::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool PersonRun::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	�����Ă鎞�̑����Ă�����
/***************************************/

PersonShedRun* PersonShedRun::GetInstance()
{
	// �����ɕϐ������
	static PersonShedRun instance;
	return &instance;
}

// �����
void PersonShedRun::Enter(BasePerson *pPerson)
{
	// �������t���O�n�m!
	//pPerson->SetIsShed(true);
	pPerson->GetRipple()->Action();

	// (��)�\�𗬂����Ƀ��[�V������t���鎖�����Ď��R����
	pPerson->GetObj()->SetMotion(1);

}

// ���s��
void PersonShedRun::Execute(BasePerson *pPerson)
{

	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]
	pPerson->GetRipple()->Update();


	// �^�[���t���O
	if (pPerson->m_trunFlag == true)
	{
		pPerson->SetAngle(-PI / 2);
		pPerson->SetMove(Vector3(-0.25f, 0, 0));
		// ���̋����ȉ��Ȃ�
		if (pPerson->m_orgPos.x >= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = false;
		}
	}
	else
	{
		pPerson->SetAngle(PI / 2);
		pPerson->SetMove(Vector3(+0.25f, 0, 0));
		// ���̋����ȉ��Ȃ�
		if (pPerson->m_trunPos.x <= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = true;
		}
	}



	// �g�䂪�o�I�������
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		// �I���
		pPerson->GetFSM()->ChangeState(PersonEndRun::GetInstance());
	}

}

// �o��
void PersonShedRun::Exit(BasePerson *pPerson)
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

void PersonShedRun::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool PersonShedRun::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	��������̑ҋ@
/***************************************/

PersonEndRun* PersonEndRun::GetInstance()
{
	// �����ɕϐ������
	static PersonEndRun instance;
	return &instance;
}

// �����
void PersonEndRun::Enter(BasePerson *pPerson)
{
	// (��)�\�𗬂����Ƀ��[�V������t���鎖�����Ď��R����
	pPerson->GetObj()->SetMotion(1);
}

// ���s��
void PersonEndRun::Execute(BasePerson *pPerson)
{
	// �^�[���t���O
	if (pPerson->m_trunFlag == true)
	{
		pPerson->SetAngle(-PI / 2);
		pPerson->SetMove(Vector3(-0.25f, 0, 0));
		// ���̋����ȉ��Ȃ�
		if (pPerson->m_orgPos.x >= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = false;
		}
	}
	else
	{
		pPerson->SetAngle(PI / 2);
		pPerson->SetMove(Vector3(+0.25f, 0, 0));
		// ���̋����ȉ��Ȃ�
		if (pPerson->m_trunPos.x <= pPerson->GetPos().x)
		{
			pPerson->m_trunFlag = true;
		}
	}
}

// �o��
void PersonEndRun::Exit(BasePerson *pPerson)
{

}

void PersonEndRun::Render(BasePerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "shed");
}

bool PersonEndRun::OnMessage(BasePerson *pPerson, const Message &msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}