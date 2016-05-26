#include "EffectManager.h"

/*****************************************************************/
//
//				�G�t�F�N�g�}�l�[h�W���[
//
/*****************************************************************/

/***********************************************/
// �C���X�^���X��
/***********************************************/
EffectManager* EffectManager::m_pInstance = nullptr;

EffectManager& EffectManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new EffectManager();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *m_pInstance;
}

void EffectManager::Release()
{
	SAFE_DELETE(m_pInstance);
}


/***********************************************/
// �������E���
/***********************************************/
EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{
	// �S��������
	for (auto it : m_EffectData)
	{
		SAFE_DELETE(it);
	}

	//�@�f�[�^�����
	m_EffectData.clear();
}


/***********************************************/
// �X�V
/***********************************************/
void EffectManager::Update()
{
	// List
	for (auto it = m_EffectData.begin(); it != m_EffectData.end();)
	{

		(*it)->Update();

		//if ((*it)->isEnd == true)
		//{
		//	// ��ɏ���
		//	SAFE_DELETE((*it));
		//	// ����ɍX�V�����
		//	it = m_EffectData.erase(it);
		//}
		//else
		{
			// �����ōX�V
			it++;
		}
	}
}


/***********************************************/
// �`��
/***********************************************/
void EffectManager::Render()
{
	// List
	for (auto it : m_EffectData)
	{
		//it->number->Render(it->x, it->y, it->score);
		it->Render();
	}

}

/************************************************/
//	�����ǉ�
/************************************************/
void EffectManager::AddEffect(int x, int y, EFFECT_TYPE type)
{
	BaseEffect* data;

	switch (type)
	{
	case EFFECT_TYPE::PLUS:
		data = new PlusEffect();
		break;
	case EFFECT_TYPE::HIT:
		data = new HitEffect();
		break;
	case EFFECT_TYPE::PUT:
		data = new PutEffect();
		break;
	case EFFECT_TYPE::NOTICE:
		data = new NoticeEffect();
		break;
	default:
		break;
	}

	data->Action(x, y);

	// �v�f�ǉ�
	m_EffectData.push_back(data);


}
