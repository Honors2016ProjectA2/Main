#include "JudgeManager.h"




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
	BaseGameEntity(ENTITY_ID::JUDGE_MNG)// ���W���b�W�}�l�[�W���[�p�̂h�c�ԍ���n��
{


}


// ���
JudgeManager::~JudgeManager()
{

}

void JudgeManager::Update()
{


}

bool JudgeManager::HandleMessage(const Message & msg)
{
	// �܂�����MGR�ɃX�e�[�g�}�V���͂������ɂ������ȁE�E�E
	// �Ƃ������Ƃł����Œ��Ƀ��b�Z�[�W�ɑ΂��鏈����

	switch (msg.Msg)
	{
	case MESSAGE_TYPE::GOAL_GOSSIP:
	{
		int a = 0;
		a++;
		/*�����ɗ�����Q�[���N���A�I���Ƃ̓N���A�������݂܂�*/
		// ���̃N���X�̗���JudgeMgr�ł�

		return true;// [��肭���b�Z�[�W���͂���!]
		break;
	}
	default:
		MyAssert(0, "����Ȗ��߂͎󂯕t���ĂȂ��ł�");
		break;
	}


	return false;// [��肭���b�Z�[�W���͂��Ȃ�����]
}
