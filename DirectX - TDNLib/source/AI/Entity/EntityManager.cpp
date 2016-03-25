#include "EntityManager.h"






EntityManager* EntityManager::Instance()
{
	static EntityManager instance;

	return &instance;
}

BaseGameEntity* EntityManager::GetEntityFromID(int id)const
{
	// �G���e�B�e�B�������܂�
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	// �G���e�B�e�B���}�b�v�̃����o�[���ǂ���
	assert((ent != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

	// BaseEntity�̃|�C���^��Ԃ�
	return ent->second;
}

void EntityManager::RemoveEntity(BaseGameEntity* pEntity)
{
	// �v�f������
	m_EntityMap.erase(m_EntityMap.find(pEntity->GetID()));
}

// �v�f�ǉ�
void EntityManager::RegisterEntity(BaseGameEntity* NewEntity)
{
	// �v�f��ǉ�����
	m_EntityMap.insert(std::make_pair(NewEntity->GetID(), NewEntity));
}
