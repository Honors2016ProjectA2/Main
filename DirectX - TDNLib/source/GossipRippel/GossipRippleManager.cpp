#include "GossipRippleManager.h"


// �錾
GossipRippleManager* GossipRippleManager::pInstance = nullptr;


/***********/
//	���̎擾
/***********/
GossipRippleManager & GossipRippleManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new GossipRippleManager();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *pInstance;
}

/***********/
//	���
/***********/
void GossipRippleManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// ������
GossipRippleManager::GossipRippleManager()
{

}

// ���
GossipRippleManager::~GossipRippleManager()
{

}


