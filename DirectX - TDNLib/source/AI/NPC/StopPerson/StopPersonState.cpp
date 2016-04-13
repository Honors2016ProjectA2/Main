#include "StopPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\StopPerson\StopPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
StopPersonGlobalState* StopPersonGlobalState::GetInstance()
{
	// �����ɕϐ������
	static StopPersonGlobalState instance;
	return &instance;
}

// �����
void StopPersonGlobalState::Enter(StopPerson *pPerson)
{

}

// ���s��
void StopPersonGlobalState::Execute(StopPerson *pPerson)
{

}

// �o��
void StopPersonGlobalState::Exit(StopPerson *pPerson)
{

}

void StopPersonGlobalState::Render(StopPerson * pPerson)
{

}

//
bool StopPersonGlobalState::OnMessage(StopPerson *pPerson, const Message &msg)
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
StartStop* StartStop::GetInstance()
{
	// �����ɕϐ������
	static StartStop instance;
	return &instance;
}

// �����
void StartStop::Enter(StopPerson *pPerson)
{

}

// ���s��
void StartStop::Execute(StopPerson *pPerson)
{
	// ���̃L�����N�^�[�͉\�𗬂��Ȃ�
	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	//if (pPerson->IsShed() == true)
	//{
	//	pPerson->GetFSM()->ChangeState(ShedStop::GetInstance());
	//}
}

// �o��
void StartStop::Exit(StopPerson *pPerson)
{

}

void StartStop::Render(StopPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartStop::OnMessage(StopPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�\�𗬂���
/***************************************/
ShedStop* ShedStop::GetInstance()
{
	// �����ɕϐ������
	static ShedStop instance;
	return &instance;
}

// �����
void ShedStop::Enter(StopPerson *pPerson)
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
void ShedStop::Execute(StopPerson *pPerson)
{
	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]
	pPerson->GetRipple()->Update();


	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndStop::GetInstance());
	}
}

// �o��
void ShedStop::Exit(StopPerson *pPerson)
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
}

void ShedStop::Render(StopPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedStop::OnMessage(StopPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�I��
/***************************************/
EndStop* EndStop::GetInstance()
{
	// �����ɕϐ������
	static EndStop instance;
	return &instance;
}

// �����
void EndStop::Enter(StopPerson *pPerson)
{

}

// ���s��
void EndStop::Execute(StopPerson *pPerson)
{

}

// �o��
void EndStop::Exit(StopPerson *pPerson)
{

}

void EndStop::Render(StopPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndStop::OnMessage(StopPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

