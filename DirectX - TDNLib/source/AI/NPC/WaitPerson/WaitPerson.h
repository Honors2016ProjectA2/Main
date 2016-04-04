#pragma once
#include "TDNLIB.h"

#include "AI\NPC\BasePerson\BasePerson.h"

/*
	�����Ȃ��l�̎��
*/

class WaitPerson:public BasePerson
{
public:
	WaitPerson(ENTITY_ID id);
	virtual ~WaitPerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	virtual void  Render();
	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg);

	// �A�N�Z�T
	// ���X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<WaitPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ���X�e�[�g�}�V����V���Ƀx�[�X�p�[�\������p�����
	 StateMachine<WaitPerson>*  m_pStateMachine;
};
