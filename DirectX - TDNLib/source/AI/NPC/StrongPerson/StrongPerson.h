#pragma once
#include "TDNLIB.h"

#include "AI\NPC\BasePerson\BasePerson.h"

/*
�����Ȃ��l�̎��
*/

class StrongPerson :public BasePerson
{
public:
	StrongPerson(ENTITY_ID id);
	virtual ~StrongPerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	virtual void  Render();
	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();


	// �A�N�Z�T
	// ���X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<StrongPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ���X�e�[�g�}�V����V���Ƀx�[�X�p�[�\������p�����
	StateMachine<StrongPerson>*  m_pStateMachine;
};
