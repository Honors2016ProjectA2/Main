#include "JudgeManager.h"
#include "../AI/NPC/PersonManager.h"

//**************************************************************************************************
//
//		���X�e�[�g���Ƃ̃��b�Z�[�W��M����
//
//**************************************************************************************************

//===============================================================================
//		�O���[�o���X�e�[�g
bool Judge::GlobalState::OnMessage(JudgeManager *pJudge, const Message &msg)
{
	// �o�Ă����I
	return false;
}

//===============================================================================
//		�S���ɗ���
void Judge::AllShed::Enter(JudgeManager *pJudge)
{
	m_ShedCount = 0;

	// �S���Ȃ̂ŁA���X�g�̃T�C�Y
	// ��TODO:���킳�𗬂��Ă͂����Ȃ��l�Ԃ̓J�E���g���Ȃ��悤�ɂ�����
	m_GoalCount = PersonMgr.GetMaxStayPerson();
}
bool Judge::AllShed::OnMessage(JudgeManager *pJudge, const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SHED_GOSSIP:
		// �S���������I
		if (++m_ShedCount >= m_GoalCount)
		{
			pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_CLEAR);
		}
		break;

	case MESSAGE_TYPE::GAMEOVER:
		// �����Ă͂����Ȃ����c�ɗ������Ƃ�
		pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_OVER);
		break;

	case MESSAGE_TYPE::GOAL_GOSSIP:
	{
		MyAssert(0, "�N���A�����͑S���ɗ����Ɛݒ肳��Ă��܂����A�S�[���ƂȂ�l�Ԃ����܂�");
		break;
	}
	default:
		MyAssert(0, "����Ȗ��߂͎󂯕t���ĂȂ��ł�");
		break;
	}


	return false;// [��肭���b�Z�[�W���͂��Ȃ�����]
}

//===============================================================================
//		����l���ɗ���
bool Judge::GoalPerson::OnMessage(JudgeManager *pJudge, const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SHED_GOSSIP:
		break;

	case MESSAGE_TYPE::GAMEOVER:
		// �����Ă͂����Ȃ����c�ɗ������Ƃ�
		pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_OVER);
		break;

	case MESSAGE_TYPE::GOAL_GOSSIP:
	{
		/*�����ɗ�����Q�[���N���A�I���Ƃ̓N���A�������݂܂�*/
		// ���̃N���X�̗���JudgeMgr�ł�

		pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_CLEAR);

		return true;// [��肭���b�Z�[�W���͂���!]
		break;
	}
	default:
		MyAssert(0, "����Ȗ��߂͎󂯕t���ĂȂ��ł�");
		break;
	}
	
	
	return false;// [��肭���b�Z�[�W���͂��Ȃ�����]
}



// �錾
JudgeManager* JudgeManager::pInstance = nullptr;

/***********/
//	���̎擾
/***********/
JudgeManager & JudgeManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new JudgeManager();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *pInstance;
}

/***********/
//	���
/***********/
void JudgeManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// ������
JudgeManager::JudgeManager() :
	BaseGameEntity(ENTITY_ID::JUDGE_MNG),// ���W���b�W�}�l�[�W���[�p�̂h�c�ԍ���n��
	m_ClearFlag(CLEAR_FLAG::GOAL_PERSON)
{
	// �����ԏ�����
	m_JudgeMode = JUDGEMODE::NONE;

	// ���X�e�[�g�}�V���̏�����
	m_pStateMachine = new StateMachine<JudgeManager>(this);
	m_pStateMachine->SetGlobalState(Judge::GlobalState::GetInstance());// �O���[�o��
}


// ���
JudgeManager::~JudgeManager()
{
	SAFE_DELETE(m_pStateMachine);
}

void JudgeManager::Update()
{
	m_pStateMachine->Update();
}


//**************************************************************************************************
//		���b�Z�[�W��M
//**************************************************************************************************
bool JudgeManager::HandleMessage(const Message & msg)
{
	// �܂�����MGR�ɃX�e�[�g�}�V���͂������ɂ������ȁE�E�E
	// �Ƃ������Ƃł����Œ��Ƀ��b�Z�[�W�ɑ΂��鏈����

	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::GOAL_GOSSIP:
	//{
	//	int a = 0;
	//	a++;
	//	/*�����ɗ�����Q�[���N���A�I���Ƃ̓N���A�������݂܂�*/
	//	// ���̃N���X�̗���JudgeMgr�ł�
	//
	//	return true;// [��肭���b�Z�[�W���͂���!]
	//	break;
	//}
	//default:
	//	MyAssert(0, "����Ȗ��߂͎󂯕t���ĂȂ��ł�");
	//	break;
	//}
	//
	//
	//return false;// [��肭���b�Z�[�W���͂��Ȃ�����]

	// �ꉞ�A�����̃N���A�����ɂ��Ή�����悤�ɃX�e�[�g�}�V���ŕ����Ă݂܂��B���̏����̓X�e�[�g�̒��ŏ����Ă��܂�
	return m_pStateMachine->HandleMessage(msg);
}

//**************************************************************************************************
//		�N���A�����̐ݒ�
//**************************************************************************************************
void JudgeManager::SetClearFlag(CLEAR_FLAG flag)
{
	SetClearFlag((BYTE)flag);	// ���̊֐����Ăяo��

	// �����ɉ����āA�X�e�[�g��ύX
	//switch (flag)
	//{
	//case CLEAR_FLAG::ALL_SHED:
	//	m_pStateMachine->SetCurrentState(Judge::AllShed::GetInstance());
	//	break;
	//
	//case CLEAR_FLAG::GOAL_PERSON:
	//	m_pStateMachine->SetCurrentState(Judge::GoalPerson::GetInstance());
	//	break;
	//default:
	//	MyAssert(0, "��O: �Ӑ}���Ȃ��N���A�t���O���ݒ肳��܂���");
	//	break;
	//}
}

void JudgeManager::SetClearFlag(BYTE flag)
{
	// �����ɉ����āA�X�e�[�g��ύX
	if (flag & (BYTE)CLEAR_FLAG::ALL_SHED)
	{
		m_ClearFlag = CLEAR_FLAG::ALL_SHED;
		m_pStateMachine->SetCurrentState(Judge::AllShed::GetInstance());
	}
	else if (flag & (BYTE)CLEAR_FLAG::GOAL_PERSON)
	{
		m_ClearFlag = CLEAR_FLAG::GOAL_PERSON;
		m_pStateMachine->SetCurrentState(Judge::GoalPerson::GetInstance());
	}
	else MyAssert(0, "��O: �Ӑ}���Ȃ��N���A�t���O���ݒ肳��܂���");

	// ����l���ɗ����ȃt���O�̐ݒ�
	m_bDontShed = !(!(flag & (BYTE)CLEAR_FLAG::DONT_SHED_PERSON));
}