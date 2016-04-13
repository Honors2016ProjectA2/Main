#include "TDNLIB.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "../AI/Message/Message.h"
#include "../Stage/Stage.h"
#include "sceneMainState.h"
#include "../IconButton/IconButton.h"
#include "../UI/UI.h"
#include "../Sound/SoundManager.h"
#include "../Fade/Fade.h"
#include "../JudgeManager/JudgeManager.h"
#include "Tutorial.h"
#include "../Camera/Camera.h"

// sceneMain�̃O���[�o���ϐ�����
extern int RippleCount;

// ���Q�[���̃X�e�[�g���O���[�o���Ŏ��I
// UI���̗l�X�ȏ��ŕK�v�ɂȂ邱�Ƃ������̂����A�����Ń����o�ϐ��Ƃ��Ď��ƁAsceneMain(MainFrame)�ɃA�N�Z�X����K�v�������ĕs�\�Ȃ̂ł����ɒu�����Ƃɂ���
GAME_STATE g_GameState;

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
//			�Q�[�����C���̃C���g��
//*****************************************************************************************************************************
sceneMainIntro* sceneMainIntro::GetInstance()
{
	// �����ɕϐ������
	static sceneMainIntro instance;
	return &instance;
}

// �����
void sceneMainIntro::Enter(sceneMain *pMain)
{
	// �^�C�}�[������
	m_timer = 0;

	// �X�e�b�v������
	m_step = 0;

	// �摜�̈ʒu������
	m_ImagePos = Vector2(-1024, 320);

	// (TODO)������tdn2DObj�̓V���O���g���f�X�g���N�^�ŏ������ƃG���[���o��̂ŃX�e�[�g����Exit�ŏ����Ă��邪�A
	// ���_Exit�ɍs���܂łɃQ�[�����I������Ƃ���Ȃ����������[�N����������
	if (JudgeMgr.GetClearFlag() == CLEAR_FLAG::GOAL_PERSON)
		m_pImage = new tdn2DObj("DATA/UI/goal_personIntro.png");
	else if (JudgeMgr.GetClearFlag() == CLEAR_FLAG::ALL_SHED)
		m_pImage = new tdn2DObj("DATA/UI/all_shedIntro.png");
	else assert(0);

	// �{�^���S��OFF
	for (UINT i = 0; i < BUTTON_ID::MAX; i++)
	{
		pMain->GetButtonMgr()->SetEnDis(i, EN_DIS_TYPE::DISABLE_VANISH);
	}
}

// ���s��
void sceneMainIntro::Execute(sceneMain *pMain)
{
	GossipRippleMgr.Update();

	PersonMgr.Update();

	// ���N���b�N�ŃC���g���X�L�b�v
	if (tdnMouse::GetLeft() == 3)
	{
		// �Q�[���Ɉڍs
		pMain->GetFSM()->ChangeState(sceneMainSetPart::GetInstance());
	}

	// �J�������񂪃C���g�����Ȃ̂ł܂������Ȃ�(�����̃C���g���p�[�g�̓J�������o���I����Ă���̃p�[�g)
	if (CameraMgr.isIntro()) return;

	switch (m_step)
	{
	case 0:
		/* �t�F�[�h�����邾���̎d�� */

		// �u���b�N�t�F�[�hON
		Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);

		// ���̃X�e�b�v��
		NextStep();
		break;

	case 1:
		/* �N���A�����̉摜��������E�ֈړ������邾���̎d�� */

	{
			  // �ڕW���W(���������I�Ȍv�Z�ŏo��)
			  const int GoalX = tdnSystem::GetScreenSize().right / 2 - m_pImage->GetWidth() / 2;

			  // ���}A��Ԃ�GOGO�I�I  �^�񒆂ɓ���������
			  if ((m_ImagePos.x += 12) > GoalX)
			  {
				  m_ImagePos.x = (float)GoalX;
				  // ���̃X�e�b�v��
				  NextStep();
			  }
	}
		break;

	case 2:
		/* �N���A������3�b���߂邾���̎d�� */

		if (++m_timer > 180)NextStep();
		break;

	case 3:
		/* �N���A�����̉摜������Ȃ�[ */

		// ���}A��Ԃ�GOGO�I�I
		if ((m_ImagePos.x += 12) > tdnSystem::GetScreenSize().right + 320)
		{
			// �t�F�[�h����
			Fade::Set(Fade::FLAG::FADE_IN, 4, 0x00000000, 128, 0);

			// ���̃X�e�b�v��
			NextStep();
		}
		break;

	case 4:
		/* �t�F�[�h������܂ő҂����̎d�� */

		// �t�F�[�h���I�������
		if (Fade::isFadeStop())
		{
			// �Q�[���Ɉڍs
			pMain->GetFSM()->ChangeState(sceneMainSetPart::GetInstance());
		}
		break;
	}
}

// �o��
void sceneMainIntro::Exit(sceneMain *pMain)
{
	// (TODO)�� Enter�Q��
	delete m_pImage;
}

void sceneMainIntro::Render(sceneMain * pMain)
{

}

void sceneMainIntro::Render2D(sceneMain *pMain)
{
	Fade::Render();

	// �t�F�[�h�̏�ɉ摜��u��
	m_pImage->Render((int)m_ImagePos.x, (int)m_ImagePos.y);

	tdnText::Draw(640, 620, 0x88ffffff, "�C���g���Đ���: ���N���b�N�ŃX�L�b�v");
}

bool sceneMainIntro::OnMessage(sceneMain *pMain, const Message &msg)
{
	// �o�Ă����I
	return false;
}

//*****************************************************************************************************************************
//			�l�Ԃ�ݒu���ăX�^�[�g�������p�[�g
//*****************************************************************************************************************************
sceneMainSetPart* sceneMainSetPart::GetInstance()
{
	// �����ɕϐ������
	static sceneMainSetPart instance;
	return &instance;
}

sceneMainSetPart::~sceneMainSetPart()
{
	SAFE_DELETE(m_pTutorial);
};

// �����
void sceneMainSetPart::Enter(sceneMain *pMain)
{
	// �`���[�g���A���|�C���^����ɂ���(��������Ȃ��Ɖ��̂����������[�N���Ă��܂�)
	SAFE_DELETE(m_pTutorial);

	// ���`���[�g���A���̃X�e�[�W�Ȃ�`���[�g���A�����s�|�C���^��new
	int StageNo(StageMgr.GetStageNo());
	if (StageNo < 1)
	{
		m_pTutorial = new TutorialManager;
		m_pTutorial->Initialize(pMain, StageNo);
	}
	else
	{
		// �{�^���L����
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RETRY, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::START, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_WHITE);	// ���{�^����I����Ԃ�
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::BLUE;

		// �R���e�B�j���[�p�̃{�^�����͔�\��(������)����
		pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_YES, EN_DIS_TYPE::DISABLE_VANISH);
		pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_NO, EN_DIS_TYPE::DISABLE_VANISH);
	}

	// �ݒu����Ƃ��ɕ\�����郁�b�V���̏�����
	m_HoldMesh = nullptr;


	// �����c����ց@�����Őݒu����l�Ԃ̃^�C�v��ݒ肵�Ă�̂ŁA���������炱����WAIT�����ꂼ��̃^�C�v�ɐݒ肵�Ă�������

	ButtonPersonMap[SELECT_BUTTON_COLOR::BLUE] = PERSON_TYPE::WAIT;		// ��ԍ��̃{�^��(����WAIT�ő��v)
	ButtonPersonMap[SELECT_BUTTON_COLOR::RED] = PERSON_TYPE::STOP;		// �^�񒆂̃{�^��(�\���L����̂��~�߂���)
	ButtonPersonMap[SELECT_BUTTON_COLOR::GREEN] = PERSON_TYPE::STRONG;	// �E�̃{�^��(1.5�{�͈̔͂������)
}

// ���s��
void sceneMainSetPart::Execute(sceneMain *pMain)
{
	if (m_pTutorial)
	{
		// �`���[�g���A���̐����I����Č�̓Q�[���i�s���Ď�
		if (!m_pTutorial->Update(pMain))
		{
			delete m_pTutorial, m_pTutorial = nullptr;
		}
		return;
	}

	// �L�[�{�[�h�ɂ��ݒu�l�ԕύX
	if (KeyBoardTRG(KB_1))
	{
		ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::BLUE);
	}
	else if (KeyBoardTRG(KB_2))
	{
		ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::RED);
	}
	else if (KeyBoardTRG(KB_3))
	{
		ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::GREEN);
	}

	// �z�C�[���ɂ��ݒu�l�ԕύX
	else if (tdnMouse::GetWheelFlag() == WHEEL_FLAG::UP)
	{
		if (m_SelectButtonColor + 1 >= (int)SELECT_BUTTON_COLOR::MAX)
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)0);
		}
		else
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)(m_SelectButtonColor + 1));
		}
	}
	else if (tdnMouse::GetWheelFlag() == WHEEL_FLAG::DOWN)
	{
		if (m_SelectButtonColor == 0)
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)2);
		}
		else
		{
			ChangeSelectButton(pMain, (SELECT_BUTTON_COLOR)(m_SelectButtonColor - 1));
		}
	}


	// �{�^���͈͓��łȂ���΁A�ݒu����I�u�W�F�N�g���Q��(�`�悷��)
	if (pMain->GetButtonMgr()->GetInButtonNo() == IconButtonManager::NOT_IN_BUTTON && RippleCount > 0)
	{
		// (TODO)���͐l�Ԃ̉�������̃I�u�W�F�������Ă���A�{���͑I�������{�^���ɉ����Đl��ς���
		m_HoldMesh = PersonMgr.GetPerson(0)->GetObj();
		m_HoldMeshPos = Math::ScreenToWorldPlate(tdnMouse::GetPos());
	}
	else
	{
		m_HoldMesh = nullptr;
	}

	// ���N���b�N
	if (tdnMouse::GetLeft() == 3)
	{
		// ���{�^������
		switch (pMain->GetButtonMgr()->GetInButtonNo())
		{
			/* �{�^���N���b�N */
		case BUTTON_ID::BLUE:

			// �ݒu�l�ԕύX
			ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::BLUE);
			//if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::DISABLE_WHITE);
			//}
			//else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
			//}
			//pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::DISABLE_WHITE);
			//m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::BLUE;
			break;

			/* �ԃ{�^���N���b�N */
		case BUTTON_ID::RED:

			// �ݒu�l�ԕύX
			ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::RED);
			//if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
			//}
			//else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
			//}
			//pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::DISABLE_WHITE);
			//m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::RED;
			break;

			/* �΃{�^���N���b�N */
		case BUTTON_ID::GREEN:

			// �ݒu�l�ԕύX
			ChangeSelectButton(pMain, SELECT_BUTTON_COLOR::GREEN);
			//if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
			//}
			//else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
			//{
			//	pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
			//}
			//pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::DISABLE_WHITE);
			//m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::GREEN;
			break;

			/* �X�^�[�g�{�^���N���b�N */
		case BUTTON_ID::START:
			// ���K�������ɂ���
			se->SetTone("�g��o��", 0);

			// ���g�䗬���X�^�[�g�I
			PersonMgr.StartGossip();

			// �{�^�������ɂ���I
			//pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(3, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(4, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(5, EN_DIS_TYPE::DISABLE_BLACK);
			pMain->GetButtonMgr()->SetEnDis(6, EN_DIS_TYPE::DISABLE_BLACK);

			// �X�e�[�g��g�䗬��Ă�̂����邾���̃X�e�[�g�ɂ���
			pMain->GetFSM()->ChangeState(sceneMainGossip::GetInstance());
			break;

			/* �����{�^���ɓ����Ă��Ȃ��Ƃ� */
		case IconButtonManager::NOT_IN_BUTTON:

			// �ݒu�񐔂��c���Ă���Ȃ�
			if (RippleCount > 0)
			{
				RippleCount--;

				// �I�񂾃{�^���̎�ނɉ����Đl�Ԃ�ݒu����
				PersonMgr.AddPerson(ButtonPersonMap[(SELECT_BUTTON_COLOR)m_SelectButtonColor], m_HoldMeshPos);
			}
			break;
		}
	}
	//rip->Update();
	GossipRippleMgr.Update();

	PersonMgr.Update();
}

// �o��
void sceneMainSetPart::Exit(sceneMain *pMain)
{

}

void sceneMainSetPart::Render(sceneMain * pMain)
{
	if (m_pTutorial)
	{
		m_pTutorial->Render(pMain);
		return;
	}

	if (m_HoldMesh)
	{
		m_HoldMesh->SetPos(m_HoldMeshPos);
		m_HoldMesh->Update();
		m_HoldMesh->Render(RS::COPY, .5f);
	}
}

void sceneMainSetPart::Render2D(sceneMain *pMain)
{
	// UI�`��
	UIMgr.Render();

	// �`���[�g���A���ł̕`��
	if (m_pTutorial)
	{
		m_pTutorial->Render2D(pMain);
	}
}

bool sceneMainSetPart::OnMessage(sceneMain *pMain, const Message &msg)
{
	// �o�Ă����I
	return false;
}

void sceneMainSetPart::ChangeSelectButton(sceneMain *pMain, SELECT_BUTTON_COLOR next)
{
	// �I��ł�F�Ɠ���������o�Ă����I
	if ((int)next == m_SelectButtonColor) return;

	switch (next)
	{
		/* �{�^�� */
	case SELECT_BUTTON_COLOR::BLUE:
		if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
		}
		else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
		}
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, (BYTE)((BYTE)EN_DIS_TYPE::DISABLE_WHITE | (BYTE)EN_DIS_TYPE::BORDERING));
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::BLUE;
		break;

		/* �ԃ{�^�� */
	case SELECT_BUTTON_COLOR::RED:
		if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
		}
		else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::GREEN)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, EN_DIS_TYPE::ENABLE);
		}
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, (BYTE)((BYTE)EN_DIS_TYPE::DISABLE_WHITE | (BYTE)EN_DIS_TYPE::BORDERING));
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::RED;
		break;

		/* �΃{�^�� */
	case SELECT_BUTTON_COLOR::GREEN:
		if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::BLUE)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::BLUE, EN_DIS_TYPE::ENABLE);
		}
		else if (m_SelectButtonColor == (int)SELECT_BUTTON_COLOR::RED)
		{
			pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::RED, EN_DIS_TYPE::ENABLE);
		}
		pMain->GetButtonMgr()->SetEnDis((UINT)BUTTON_ID::GREEN, (BYTE)((BYTE)EN_DIS_TYPE::DISABLE_WHITE | (BYTE)EN_DIS_TYPE::BORDERING));
		m_SelectButtonColor = (int)SELECT_BUTTON_COLOR::GREEN;
		break;
	}
}


//*****************************************************************************************************************************
//			���킳������Ă�p�[�g
//*****************************************************************************************************************************
sceneMainGossip* sceneMainGossip::GetInstance()
{
	// �����ɕϐ������
	static sceneMainGossip instance;
	return &instance;
}

// �����
void sceneMainGossip::Enter(sceneMain *pMain)
{

}

// ���s��
void sceneMainGossip::Execute(sceneMain *pMain)
{
	if (KeyBoard('Z') == 1)
	{
		//	rip->Action();
		float ramX = (float)(rand() % 100) - 50;
		float ramZ = (float)(rand() % 100) - 50;

		GossipRippleMgr.AddRipple(RIPPLE_TYPE::RED, Vector3(ramX, 0, ramZ));
	}
	
	// ���N���b�N
	if (tdnMouse::GetLeft() == 3)
	{
		// �{�^������
		switch (pMain->GetButtonMgr()->GetInButtonNo())
		{
		case BUTTON_ID::RETRY:
			break;
		}

		//FOR(PersonMgr.GetPersonSize())
		//{
		//	if (!PersonMgr.GetPerson(i)->IsShed() && (Math::WorldToScreen(PersonMgr.GetPerson(i)->GetPos()) - tdnMouse::GetPos()).Length() < 50)
		//	{
		//		RippleCount--;
		//
		//		// �\�A�����~�܂�܂Ŏg���Ȃ�����
		//		RippleOK = false;
		//
		//		// �Z�b�g
		//		PersonMgr.GetPerson(i)->ActionGossipRipple();
		//
		//		// UI�̐����o��
		//		UIMgr.PushHukidashi(PersonMgr.GetPerson(i)->GetPos(), HUKIDASHI_TYPE::ORIGIN);
		//
		//		// ���̃g�[���߂�
		//		se->SetTone("�g��o��", 0);
		//
		//		break;
		//	}
		//}
		//PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	// �\�A�����~�܂�����
	//if (PersonMgr.isJudgeMoment())
	//{
		// �S����������
		//if (PersonMgr.isAllShed())
		//{
		//	// ���̃Z�b�g���[�h�̓V�[�����C���̕`��ŕ��򂳂��Ă邩��
		//	g_GameState = GAME_STATE::GAME_CLEAR;
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
		//	pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
		//
		//	// �X�e�[�g�`�F���W
		//	pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());
		//
		//	// �u���b�N�t�F�[�hON
		//	Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		//}
		//
		//// �g�p�񐔂�0�ɂȂ�����
		//else if (RippleCount == 0)
		//{
		//	g_GameState = GAME_STATE::GAME_OVER;
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
		//	//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
		//	pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);
		//
		//	// �X�e�[�g�`�F���W
		//	pMain->GetFSM()->ChangeState(sceneMainGameOver::GetInstance());
		//
		//	// �u���b�N�t�F�[�hON
		//	Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		//}
	//}

	// ���W���b�W���񂪔�����o���Ă���A���[�h�ؑ�
	if (JudgeMgr.isGameOver())
	{
		if (Fade::alpha == 0)
		{
			// �{�^���L���E�����ݒ�
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);

			// �u���b�N�t�F�[�hON
			Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		}
		else if (Fade::alpha == 128)
		{
			// �O���[�o���̃Q�[���X�e�[�g���`�F���W
			g_GameState = GAME_STATE::GAME_OVER;

			// �X�e�[�g�`�F���W(�Q�[���X�e�[�g���Q�[���I�[�o�[�X�e�[�g��)
			pMain->GetFSM()->ChangeState(sceneMainGameOver::GetInstance());
		}
	}
	else if (JudgeMgr.isGameClear())
	{
		if (Fade::alpha == 0)
		{
			// �{�^���L���E�����ݒ�
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::YES, EN_DIS_TYPE::ENABLE);
			//pMain->GetButtonMgr()->SetEnDis((UINT)sceneMain::BUTTON_ID::NO, EN_DIS_TYPE::ENABLE);
			pMain->GetButtonMgr()->SetEnDis(2, EN_DIS_TYPE::DISABLE_BLACK);

			// �u���b�N�t�F�[�hON
			Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00000000, 1, 128);
		}
		else if (Fade::alpha == 128)
		{
			// �O���[�o���̃Q�[���X�e�[�g���`�F���W
			g_GameState = GAME_STATE::GAME_CLEAR;

			// �X�e�[�g�`�F���W(�Q�[���X�e�[�g���N���A�X�e�[�g��)
			pMain->GetFSM()->ChangeState(sceneMainGameClear::GetInstance());
		}
	}


	//rip->Update();
	GossipRippleMgr.Update();

	PersonMgr.Update();
}

// �o��
void sceneMainGossip::Exit(sceneMain *pMain)
{

}

void sceneMainGossip::Render(sceneMain * pMain)
{
}

void sceneMainGossip::Render2D(sceneMain *pMain)
{
	// UI�`��
	UIMgr.Render();
}

bool sceneMainGossip::OnMessage(sceneMain *pMain, const Message &msg)
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

void sceneMainGameClear::Render2D(sceneMain *pMain)
{
	// UI�`��
	UIMgr.Render();
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
	// ���R���e�B�j���[�p�̃{�^���L����
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_YES, EN_DIS_TYPE::ENABLE);
	pMain->GetButtonMgr()->SetEnDis(BUTTON_ID::CONTINUE_NO, EN_DIS_TYPE::ENABLE);
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

void sceneMainGameOver::Render2D(sceneMain *pMain)
{
	// UI�`��
	UIMgr.Render();
}

bool sceneMainGameOver::OnMessage(sceneMain *pMain, const Message &msg)
{
	// �o�Ă����I
	return false;
}