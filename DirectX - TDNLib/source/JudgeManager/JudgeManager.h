#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
�\�̔g��}�l�[�W���[.h

*/

class BasePerson;

class JudgeManager :public BaseGameEntity
{
public:
	static JudgeManager& GetInstance();
	static void Release();
	virtual ~JudgeManager();

	// �������z�֐��ɂ��p����������
	void Update();

	bool  HandleMessage(const Message& msg);

private:
	static JudgeManager* pInstance;

	// ����
	JudgeManager();
	JudgeManager(const JudgeManager&);
	JudgeManager& operator=(const JudgeManager&);


};

#define JudgeMgr (JudgeManager::GetInstance())
