#pragma once
#include "TDNLIB.h"
#include  "AI\NPC\BasePerson\BasePerson.h"


/*
	�S�[���̐l

*/

class GoalPerson:public BasePerson
{
public:
	GoalPerson(ENTITY_ID id);
	virtual ~GoalPerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	virtual void  Render();
	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();

	// �A�N�Z�T
	// ���X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<GoalPerson>* GetFSM()const { return m_pStateMachine; }
private:

	// ���X�e�[�g�}�V����V���Ƀx�[�X�p�[�\������p�����
	StateMachine<GoalPerson>*  m_pStateMachine;
};
