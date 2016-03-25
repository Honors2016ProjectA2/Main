#include "AI\NPC\PersonManager.h"

//�@�ЂƂ̎��
#include "AI\NPC\BasePerson\BasePerson.h"



// �錾
PersonManager* PersonManager::pInstance = nullptr;


/***********/
//	���̎擾
/***********/
PersonManager & PersonManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new PersonManager();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *pInstance;
}

/***********/
//	���
/***********/
void PersonManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// ������
PersonManager::PersonManager()
{

}

// ���
PersonManager::~PersonManager()
{

}
