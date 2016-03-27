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
	// �S��������
	for (int i = 0; i < (int)m_RippleData.size(); i++)
	{
		SAFE_DELETE(m_RippleData[i]);
	}

	//�@�f�[�^�����
	m_RippleData.clear();
}

// �X�V
void GossipRippleManager::Update()
{

	if (m_RippleData.empty())return;// �󂾂����烊�^�[��

									
	for (int i = 0; i < (int)m_RippleData.size(); i++)
	{
		m_RippleData[i]->Update();

		// (TODO)�������ŏ�������
		if (m_RippleData[i]->IsEnd()==true)
		{
		
		}
	}

}

// �`��
void GossipRippleManager::Render()
{
	if (m_RippleData.empty())return;// �󂾂����烊�^�[��
									
	for (int i = 0; i < (int)m_RippleData.size(); i++)
	{
		m_RippleData[i]->Render();
	}

}

void GossipRippleManager::AddRipple(RIPPLE_TYPE type, Vector3 pos)
{
	GossipRipple* data;
	
	data = new GossipRipple(type, pos);
	// +Action (TODO) ���͂����Ŕg����L����
	data->Action();

	// �v�f�ǉ�
	m_RippleData.push_back(data);

}


