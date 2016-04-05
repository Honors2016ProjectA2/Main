#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
噂の波紋マネージャー.h

*/

class BasePerson;

class JudgeManager :public BaseGameEntity
{
public:
	static JudgeManager& GetInstance();
	static void Release();
	virtual ~JudgeManager();

	// 純粋仮想関数により継承したもの
	void Update();

	bool  HandleMessage(const Message& msg);

private:
	static JudgeManager* pInstance;

	// 封印
	JudgeManager();
	JudgeManager(const JudgeManager&);
	JudgeManager& operator=(const JudgeManager&);


};

#define JudgeMgr (JudgeManager::GetInstance())
