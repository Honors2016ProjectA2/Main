#pragma once


/*
噂の波紋マネージャー.h

*/

class BasePerson;

class PersonManager
{
public:
	static PersonManager& GetInstance();
	static void Release();
	virtual ~PersonManager();

private:
	static PersonManager* pInstance;

	// 封印
	PersonManager();
	PersonManager(const PersonManager&);
	PersonManager& operator=(const PersonManager&);


};


// インスタンス化
#define PersonMgr	(PersonManager::GetInstance())


