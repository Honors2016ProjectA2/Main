#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
噂の波紋マネージャー.h

*/

class BasePerson;

class PersonManager:public BaseGameEntity
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

	// うわさが流れた人の総計
	int m_NumShedPerson;

	// 噂流しの判定した瞬間
	bool m_bJudgeMoment;

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

	// メッセージ更新
	bool  HandleMessage(const Message& msg);

	// 波紋vs人
	void RippleVSPerson(RIPPLE_INFO* pRipData);

	// アクセサ
	BasePerson* GetPerson(int no);
	int GetPersonSize() { return (int)m_PersonData.size(); }

	// クリア判定: 全員に噂を流したか
	bool isAllShed(){ return (m_NumShedPerson == m_PersonData.size()); }
	bool isJudgeMoment(){ return m_bJudgeMoment; }
};


// インスタンス化
#define PersonMgr	(PersonManager::GetInstance())


