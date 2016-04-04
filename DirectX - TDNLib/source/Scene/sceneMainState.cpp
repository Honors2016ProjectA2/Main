#include "TDNLIB.h"
#include "../Scene/SceneMain.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "../AI/Message/Message.h"
#include "../Stage/Stage.h"
#include "sceneMainState.h"
#include "../IconButton/IconButton.h"
#include "../UI/UI.h"
#include "../Sound/SoundManager.h"

// sceneMain�̃O���[�o���ϐ�����
extern int RippleCount;

//*****************************************************************************************************************************
//
//				�p�[�g�S��
//
//*****************************************************************************************************************************
/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
sceneMainGlobalState* sceneMainGlobalState::GetInstance()
{
	// �����ɕϐ������
	static sceneMainGlobalState instance;
	return &instance;
}

// �����
void sceneMainGlobalState::Enter(sceneMain *pMain)
{

}

// ���s��
void sceneMainGlobalState::Execute(sceneMain *pMain)
{

}

// �o��
void sceneMainGlobalState::Exit(sceneMain *pMain)
{

}

void sceneMainGlobalState::Render(sceneMain * pMain)
{
}

//
bool sceneMainGlobalState::OnMessage(sceneMain *pMain, const Message &msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	////case MESSAGE_TYPE::
	//
	//	break;
	//default:
	//
	//	break;
	//}//end switch

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g�Ȃ̂Ő��^�����̏I��
	return false;
}


//*****************************************************************************************************************************
//			���C��
//*****************************************************************************************************************************
sceneMainGame* sceneMainGame::GetInstance()
{
	// �����ɕϐ������
	static sceneMainGame instance;
	return &instance;
}

// �����
void sceneMainGame::Enter(sceneMain *pMain)
{
	// �X�e�[�W�̔ԍ��ɉ����Đl�Ԃ�ǂݍ���
	RippleCount = Stage::LoadPerson();
}

// ���s��
void sceneMainGame::Execute(sceneMain *pMain)
{
	if (KeyBoard('Z') == 1)
	{
		//	rip->Action();
		float ramX = (float)(rand() % 100) - 50;
		float ramZ = (float)(rand() % 100) - 50;

		GossipRippleMgr.AddRipple(RIPPLE_TYPE::RED, Vector3(ramX, 0, ramZ));
	}
	if (KeyBoard('X') == 1)
	{	
		// �Z�b�g
		PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	if (tdnMouse::GetLeft() == 3 && RippleOK)
	{
		FOR(PersonMgr.GetPersonSize())
		{
			if (!PersonMgr.GetPerson(i)->IsShed() && (Math::WorldToScreen(PersonMgr.GetPerson(i)->GetPos()) - tdnMouse::GetPos()).Length() < 50)
			{
				RippleCount--;

				// �\�A�����~�܂�܂Ŏg���Ȃ�����
				RippleOK = false;

				// �Z�b�g
				PersonMgr.GetPerson(i)->ActionGossipRipple();

				// UI�̐����o��
				UIMgr.PushHukidashi(PersonMgr.GetPerson(i)->GetPos(), HUKIDASHI_TYPE::ORIGIN);

				// ���̃g�[���߂�
				se->SetTone("�g��o��", 0);

				break;
			}
		}
		//PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	// �\�A�����~�܂�����
	if (PersonMgr.isJudgeMoment())
	{
		// �N���b�N�ŗ�����悤�ɂȂ�
		RippleOK = true;

		if (PersonMgr.isAllShed())
		{
			pMain->SetMode(sceneMain::MODE::GAMECLEAR);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());
		}
		else if (RippleCount == 0)
		{
			pMain->SetMode(sceneMain::MODE::GAMEOVER);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetFSM()->ChangeState(sceneMainGameOver::GetInstance());
		}
	}

	//rip->Update();
	GossipRippleMgr.Update();

	PersonMgr.Update();
}

// �o��
void sceneMainGame::Exit(sceneMain *pMain)
{

}

void sceneMainGame::Render(sceneMain * pMain)
{
}

bool sceneMainGame::OnMessage(sceneMain *pMain, const Message &msg)
{
	// �o�Ă����I
	return false;
}


//*****************************************************************************************************************************
//			�Q�[���N���A��
//*****************************************************************************************************************************
sceneMainGameClear* sceneMainGameClear::GetInstance()
{
	// �����ɕϐ������
	static sceneMainGameClear instance;
	return &instance;
}

// �����
void sceneMainGameClear::Enter(sceneMain *pMain)
{

}

// ���s��
void sceneMainGameClear::Execute(sceneMain *pMain)
{

}

// �o��
void sceneMainGameClear::Exit(sceneMain *pMain)
{

}

void sceneMainGameClear::Render(sceneMain * pMain)
{
}

bool sceneMainGameClear::OnMessage(sceneMain *pMain, const Message &msg)
{
	// �o�Ă����I
	return false;
}



//*****************************************************************************************************************************
//			�Q�[���I�[�o�[
//*****************************************************************************************************************************
sceneMainGameOver* sceneMainGameOver::GetInstance()
{
	// �����ɕϐ������
	static sceneMainGameOver instance;
	return &instance;
}

// �����
void sceneMainGameOver::Enter(sceneMain *pMain)
{

}

// ���s��
void sceneMainGameOver::Execute(sceneMain *pMain)
{

}

// �o��
void sceneMainGameOver::Exit(sceneMain *pMain)
{

}

void sceneMainGameOver::Render(sceneMain * pMain)
{
}

bool sceneMainGameOver::OnMessage(sceneMain *pMain, const Message &msg)
{
	// �o�Ă����I
	return false;
}