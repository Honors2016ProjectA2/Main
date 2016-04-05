#include "GoalPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\GoalPerson\GoalPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
GoalPersonGlobalState* GoalPersonGlobalState::GetInstance()
{
	// �����ɕϐ������
	static GoalPersonGlobalState instance;
	return &instance;
}

// �����
void GoalPersonGlobalState::Enter(GoalPerson *pPerson)
{

}

// ���s��
void GoalPersonGlobalState::Execute(GoalPerson *pPerson)
{

}

// �o��
void GoalPersonGlobalState::Exit(GoalPerson *pPerson)
{

}

void GoalPersonGlobalState::Render(GoalPerson * pPerson)
{

}

//
bool GoalPersonGlobalState::OnMessage(GoalPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g�Ȃ̂Ő��^�����̏I��
	return false;
}



/***************************************/
//	�ŏ�
/***************************************/
GoalStart* GoalStart::GetInstance()
{
	// �����ɕϐ������
	static GoalStart instance;
	return &instance;
}

// �����
void GoalStart::Enter(GoalPerson *pPerson)
{

}

// ���s��
void GoalStart::Execute(GoalPerson *pPerson)
{
	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(GoalShed::GetInstance());
	}
}

// �o��
void GoalStart::Exit(GoalPerson *pPerson)
{

}

void GoalStart::Render(GoalPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}


bool GoalStart::OnMessage(GoalPerson *pPerson, const Message &msg)
{

	 // Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�\�𗬂���
/***************************************/
GoalShed* GoalShed::GetInstance()
{
	// �����ɕϐ������
	static GoalShed instance;
	return &instance;
}

// �����
void GoalShed::Enter(GoalPerson *pPerson)
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
void GoalShed::Execute(GoalPerson *pPerson)
{
	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]
	pPerson->GetRipple()->Update();


	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(GoalEnd::GetInstance());
	}
}

/*
	���S�[���̐l�͂������Ⴄ
*/
// �o��
void GoalShed::Exit(GoalPerson *pPerson)
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

	// �W���b�W�N�ɃS�[����m�点��
	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::JUDGE_MNG,
		GOAL_GOSSIP,
		(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
		);
}

void GoalShed::Render(GoalPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool GoalShed::OnMessage(GoalPerson *pPerson, const Message &msg)
{


	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�I��
/***************************************/
GoalEnd* GoalEnd::GetInstance()
{
	// �����ɕϐ������
	static GoalEnd instance;
	return &instance;
}

// �����
void GoalEnd::Enter(GoalPerson *pPerson)
{

}

// ���s��
void GoalEnd::Execute(GoalPerson *pPerson)
{

}

// �o��
void GoalEnd::Exit(GoalPerson *pPerson)
{

}

void GoalEnd::Render(GoalPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool GoalEnd::OnMessage(GoalPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



