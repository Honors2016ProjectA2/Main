#include "WaitPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\WaitPerson\WaitPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
WaitPersonGlobalState* WaitPersonGlobalState::GetInstance()
{
	// �����ɕϐ������
	static WaitPersonGlobalState instance;
	return &instance;
}

// �����
void WaitPersonGlobalState::Enter(WaitPerson *pPerson)
{

}

// ���s��
void WaitPersonGlobalState::Execute(WaitPerson *pPerson)
{

}

// �o��
void WaitPersonGlobalState::Exit(WaitPerson *pPerson)
{

}

void WaitPersonGlobalState::Render(WaitPerson * pPerson)
{

}

//
bool WaitPersonGlobalState::OnMessage(WaitPerson *pPerson, const Message &msg)
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
//	�ŏ�
/***************************************/
StartWait* StartWait::GetInstance()
{
	// �����ɕϐ������
	static StartWait instance;
	return &instance;
}

// �����
void StartWait::Enter(WaitPerson *pPerson)
{

}

// ���s��
void StartWait::Execute(WaitPerson *pPerson)
{
	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(ShedWait::GetInstance());
	}
}

// �o��
void StartWait::Exit(WaitPerson *pPerson)
{

}

void StartWait::Render(WaitPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartWait::OnMessage(WaitPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�\�𗬂���
/***************************************/
ShedWait* ShedWait::GetInstance()
{
	// �����ɕϐ������
	static ShedWait instance;
	return &instance;
}

// �����
void ShedWait::Enter(WaitPerson *pPerson)
{
	// �������t���O�n�m!
	//pPerson->SetIsShed(true);
	pPerson->GetRipple()->Action();

	// (��)�\�𗬂����Ƀ��[�V������t���鎖�����Ď��R����
	pPerson->GetObj()->SetMotion(0);

	// ��
	se->Play("�g��o��", Math::WorldToScreen(pPerson->GetPos()));
}

// ���s��
void ShedWait::Execute(WaitPerson *pPerson)
{
	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]
	pPerson->GetRipple()->Update();


	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndWait::GetInstance());
	}
}

// �o��
void ShedWait::Exit(WaitPerson *pPerson)
{
	// �l�}�l�[�W���[��MSG���M
	/*
	[���e]�g����ɂ�����S��
	�\�𗬂��X�e�[�g��
	*/

	RIPPLE_INFO ex;
	ex.type = pPerson->GetPersonType();
	ex.pos = pPerson->GetPos();
	ex.size = pPerson->GetRipSize();// (��)�@��ł��ŃT�C�Y�w��

	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::PERSON_MNG,
		RIPPLE_VS_PERSON,
		(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
		);

	// �W���b�W�N�ɗ��������Ƃ�`����
	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::JUDGE_MNG,
		MESSAGE_TYPE::SHED_GOSSIP,
		nullptr	// [�ǋL���]�����̃^�C�v�𑗂�
		);
}

void ShedWait::Render(WaitPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedWait::OnMessage(WaitPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�I��
/***************************************/
EndWait* EndWait::GetInstance()
{
	// �����ɕϐ������
	static EndWait instance;
	return &instance;
}

// �����
void EndWait::Enter(WaitPerson *pPerson)
{

}

// ���s��
void EndWait::Execute(WaitPerson *pPerson)
{

}

// �o��
void EndWait::Exit(WaitPerson *pPerson)
{

}

void EndWait::Render(WaitPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndWait::OnMessage(WaitPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

