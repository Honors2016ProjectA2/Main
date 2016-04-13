#include "StrongPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\StrongPerson\StrongPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
StrongPersonGlobalState* StrongPersonGlobalState::GetInstance()
{
	// �����ɕϐ������
	static StrongPersonGlobalState instance;
	return &instance;
}

// �����
void StrongPersonGlobalState::Enter(StrongPerson *pPerson)
{

}

// ���s��
void StrongPersonGlobalState::Execute(StrongPerson *pPerson)
{

}

// �o��
void StrongPersonGlobalState::Exit(StrongPerson *pPerson)
{

}

void StrongPersonGlobalState::Render(StrongPerson * pPerson)
{

}

//
bool StrongPersonGlobalState::OnMessage(StrongPerson *pPerson, const Message &msg)
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
StartStrong* StartStrong::GetInstance()
{
	// �����ɕϐ������
	static StartStrong instance;
	return &instance;
}

// �����
void StartStrong::Enter(StrongPerson *pPerson)
{

}

// ���s��
void StartStrong::Execute(StrongPerson *pPerson)
{
	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(ShedStrong::GetInstance());
	}
}

// �o��
void StartStrong::Exit(StrongPerson *pPerson)
{

}

void StartStrong::Render(StrongPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartStrong::OnMessage(StrongPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�\�𗬂���
/***************************************/
ShedStrong* ShedStrong::GetInstance()
{
	// �����ɕϐ������
	static ShedStrong instance;
	return &instance;
}

// �����
void ShedStrong::Enter(StrongPerson *pPerson)
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
void ShedStrong::Execute(StrongPerson *pPerson)
{
	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]
	pPerson->GetRipple()->Update();


	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndStrong::GetInstance());
	}
}

// �o��
void ShedStrong::Exit(StrongPerson *pPerson)
{
	// �l�}�l�[�W���[��MSG���M
	/*
	[���e]�g����ɂ�����S��
	�\�𗬂��X�e�[�g��
	*/

	RIPPLE_INFO ex;
	ex.type = pPerson->GetPersonType();
	ex.pos = pPerson->GetPos();
	// �������@���̐l�̓T�C�Y���ł���
	ex.size = pPerson->GetRipSize();// (��)�@��ł��ŃT�C�Y�w��

	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::PERSON_MNG,
		RIPPLE_VS_PERSON,
		(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
		);
}

void ShedStrong::Render(StrongPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedStrong::OnMessage(StrongPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�I��
/***************************************/
EndStrong* EndStrong::GetInstance()
{
	// �����ɕϐ������
	static EndStrong instance;
	return &instance;
}

// �����
void EndStrong::Enter(StrongPerson *pPerson)
{

}

// ���s��
void EndStrong::Execute(StrongPerson *pPerson)
{

}

// �o��
void EndStrong::Exit(StrongPerson *pPerson)
{

}

void EndStrong::Render(StrongPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndStrong::OnMessage(StrongPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

