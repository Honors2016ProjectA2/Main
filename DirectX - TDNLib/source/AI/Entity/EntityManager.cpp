#include "EntityManager.h"






EntityManager* EntityManager::Instance()
{
	static EntityManager instance;

	return &instance;
}

BaseGameEntity* EntityManager::GetEntityFromID(int id)const
{
	// エンティティを見つけます
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	// エンティティがマップのメンバーかどうか
	assert((ent != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

	// BaseEntityのポインタを返す
	return ent->second;
}

void EntityManager::RemoveEntity(BaseGameEntity* pEntity)
{
	// 要素を消す
	m_EntityMap.erase(m_EntityMap.find(pEntity->GetID()));
}

// 要素追加
void EntityManager::RegisterEntity(BaseGameEntity* NewEntity)
{
	// 要素を追加する
	m_EntityMap.insert(std::make_pair(NewEntity->GetID(), NewEntity));
}
