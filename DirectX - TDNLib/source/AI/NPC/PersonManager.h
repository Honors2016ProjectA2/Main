#pragma once
#include "TDNLIB.h"
#include "AI\NPC\BasePerson\BasePerson.h"


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

	// Entityのカウント
	static int m_IDcount;

	// 封印
	PersonManager();
	PersonManager(const PersonManager&);
	PersonManager& operator=(const PersonManager&);

	// 人データ
	std::vector<BasePerson*> m_PersonData;


public:
	// 大量生産
	/*
		中で勝手にＩＤを振る
	*/
	void AddPerson(PERSON_TYPE type, Vector3 pos);

	//	リセット
	void Reset();

	// 更新・描画
	void Update();
	void Render();

	// 波紋vs人
	void RippleVSPerson(int ripPersonNo);

	// アクセサ
	BasePerson* GetPerson(int no);
	int GetPersonSize() { return (int)m_PersonData.size(); }
};


// インスタンス化
#define PersonMgr	(PersonManager::GetInstance())


