#pragma once
#include <map>
#include <cassert>
#include <string>
#include "BaseGameEntity.h"

/*
	エージェントのポインタのマネージャー
	



*/

class BaseGameEntity;

class EntityManager
{
public:
	static EntityManager* Instance();
	~EntityManager() {};

	//この関数はエンティティのIDで示されたインデックスの位置にある
	void            RegisterEntity(BaseGameEntity* NewEntity);

	//　パラメータとして与えられたIDを持つエンティティのポインタを返す
	BaseGameEntity* GetEntityFromID(int id)const;

	// このメソッドはリストからエンティティを削除する
	void            RemoveEntity(BaseGameEntity* pEntity);

private:

	// 入力を楽にするために
	typedef std::map<int, BaseGameEntity*> EntityMap;

	// エンティティがすぐに調べられる用に
	// このなかではエンティティへのポインタはそのIDで相互参照される
	EntityMap m_EntityMap;

	// 封印
	EntityManager() {};
	
	// ctorをコピーし割り当てはPrivateに
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);



};

// インスタンス化
#define EntityMgr EntityManager::Instance()
