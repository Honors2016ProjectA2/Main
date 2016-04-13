#include "GameOverPersonState.h"
#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\GameOverPerson\GameOverPerson.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
GameOverPersonGlobalState* GameOverPersonGlobalState::GetInstance()
{
	// �����ɕϐ������
	static GameOverPersonGlobalState instance;
	return &instance;
}

// �����
void GameOverPersonGlobalState::Enter(GameOverPerson *pPerson)
{

}

// ���s��
void GameOverPersonGlobalState::Execute(GameOverPerson *pPerson)
{

}

// �o��
void GameOverPersonGlobalState::Exit(GameOverPerson *pPerson)
{

}

void GameOverPersonGlobalState::Render(GameOverPerson * pPerson)
{

}

//
bool GameOverPersonGlobalState::OnMessage(GameOverPerson *pPerson, const Message &msg)
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
StartGameOver* StartGameOver::GetInstance()
{
	// �����ɕϐ������
	static StartGameOver instance;
	return &instance;
}

// �����
void StartGameOver::Enter(GameOverPerson *pPerson)
{

}

// ���s��
void StartGameOver::Execute(GameOverPerson *pPerson)
{
	// ���̃L�����N�^�[�͉\�𗬂��Ȃ�
	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->IsShed() == true)
	{
		pPerson->GetFSM()->ChangeState(ShedGameOver::GetInstance());
	}
}

// �o��
void StartGameOver::Exit(GameOverPerson *pPerson)
{

}

void StartGameOver::Render(GameOverPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool StartGameOver::OnMessage(GameOverPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�\�𗬂���
/***************************************/
ShedGameOver* ShedGameOver::GetInstance()
{
	// �����ɕϐ������
	static ShedGameOver instance;
	return &instance;
}

// �����
void ShedGameOver::Enter(GameOverPerson *pPerson)
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
void ShedGameOver::Execute(GameOverPerson *pPerson)
{
	// �g��
	pPerson->GetRipple()->SetPos(pPerson->GetPos());// ��Ƀv���C���[�Ǐ]
	pPerson->GetRipple()->Update();


	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	if (pPerson->GetRipple()->IsEnd() == true)
	{
		pPerson->GetFSM()->ChangeState(EndGameOver::GetInstance());
	}
}

// �o��
void ShedGameOver::Exit(GameOverPerson *pPerson)
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

	// ������
	// �Q�[���I�[�o�[���W���b�W�N�֑���
	MsgMgr->Dispatch(
		MSG_NO_DELAY,
		pPerson->GetID(),
		ENTITY_ID::JUDGE_MNG,
		GAMEOVER,
		(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
		);
}

void ShedGameOver::Render(GameOverPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool ShedGameOver::OnMessage(GameOverPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	�I��
/***************************************/
EndGameOver* EndGameOver::GetInstance()
{
	// �����ɕϐ������
	static EndGameOver instance;
	return &instance;
}

// �����
void EndGameOver::Enter(GameOverPerson *pPerson)
{

}

// ���s��
void EndGameOver::Execute(GameOverPerson *pPerson)
{

}

// �o��
void EndGameOver::Exit(GameOverPerson *pPerson)
{

}

void EndGameOver::Render(GameOverPerson * pPerson)
{
	pPerson->GetObj()->Render(shader, "copy");
}

bool EndGameOver::OnMessage(GameOverPerson *pPerson, const Message &msg)
{

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

