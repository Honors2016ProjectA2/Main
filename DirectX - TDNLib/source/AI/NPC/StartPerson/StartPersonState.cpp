#include "StartPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\StartPerson\StartPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
StartPersonGlobalState* StartPersonGlobalState::GetInstance()
{
	// �����ɕϐ������
	static StartPersonGlobalState instance;
	return &instance;
}

// �����
void StartPersonGlobalState::Enter(StartPerson *pPerson)
{

}

// ���s��
void StartPersonGlobalState::Execute(StartPerson *pPerson)
{
	// �g��͏�ɍX�V
	pPerson->GetRipple()->Update();
}

// �o��
void StartPersonGlobalState::Exit(StartPerson *pPerson)
{

}

void StartPersonGlobalState::Render(StartPerson * pPerson)
{

}

// ������
// �����œ��ɈӖ��̂Ȃ��g����������߂��󂯂���
bool StartPersonGlobalState::OnMessage(StartPerson *pPerson, const Message &msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::PRODUCT_RIPPLE:

		MyDebugString("ID[%d]�@�����o�p�g���������\n", (int)pPerson->GetID());
		pPerson->GetRipple()->Action();
		// �g��
		pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]


		return true;

		break;
	default:

		break;
	}//end switch

	 // Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	�ŏ�
/***************************************/
FirstStart* FirstStart::GetInstance()
{
	// �����ɕϐ������
	static FirstStart instance;
	return &instance;
}

// �����
void FirstStart::Enter(StartPerson *pPerson)
{

}

// ���s��
void FirstStart::Execute(StartPerson *pPerson)
{
	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(FirstShed::GetInstance());
	}
}

// �o��
void FirstStart::Exit(StartPerson *pPerson)
{

}

void FirstStart::Render(StartPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}


// �������X�^�[�g�p�[�\���͂������Ⴄ�I
bool FirstStart::OnMessage(StartPerson *pPerson, const Message &msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::START_GOSSIP:

		MyDebugString("ID[%d]�@���g��X�^�[�g\n", (int)pPerson->GetID());
		pPerson->GetFSM()->ChangeState(FirstShed::GetInstance());

		return true;

		break;
	default:

		break;
	}//end switch

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�\�𗬂���
/***************************************/
FirstShed* FirstShed::GetInstance()
{
	// �����ɕϐ������
	static FirstShed instance;
	return &instance;
}

// �����
void FirstShed::Enter(StartPerson *pPerson)
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
void FirstShed::Execute(StartPerson *pPerson)
{
	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]



	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(FirstEnd::GetInstance());
	}
}

// �o��
void FirstShed::Exit(StartPerson *pPerson)
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

void FirstShed::Render(StartPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool FirstShed::OnMessage(StartPerson *pPerson, const Message &msg)
{


	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�I��
/***************************************/
FirstEnd* FirstEnd::GetInstance()
{
	// �����ɕϐ������
	static FirstEnd instance;
	return &instance;
}

// �����
void FirstEnd::Enter(StartPerson *pPerson)
{

}

// ���s��
void FirstEnd::Execute(StartPerson *pPerson)
{

}

// �o��
void FirstEnd::Exit(StartPerson *pPerson)
{

}

void FirstEnd::Render(StartPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool FirstEnd::OnMessage(StartPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

