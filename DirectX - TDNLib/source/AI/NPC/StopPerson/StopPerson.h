#pragma once
#include "TDNLIB.h"
#include "AI\NPC\BasePerson\BasePerson.h"

/*
	�X�g�b�v�p�[�\��
*/


class StopPerson :public BasePerson
{
public:
	StopPerson(ENTITY_ID id);
	virtual ~StopPerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	virtual void  Render();
	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();


	// �A�N�Z�T
	// ���X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<StopPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ���X�e�[�g�}�V����V���Ƀx�[�X�p�[�\������p�����
	StateMachine<StopPerson>*  m_pStateMachine;
};
