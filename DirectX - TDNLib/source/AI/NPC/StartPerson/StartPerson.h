#pragma once
#include "TDNLIB.h"

#include "AI\NPC\BasePerson\BasePerson.h"

/*
	�n�߂̐l�̎��
*/

class StartPerson:public BasePerson
{
public:
	StartPerson(ENTITY_ID id);
	virtual ~StartPerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	virtual void  Render();
	virtual void  UIRender();


	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg);
	
	void ResetState();


	// �A�N�Z�T
	// ���X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<StartPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ���X�e�[�g�}�V����V���Ƀx�[�X�p�[�\������p�����
	 StateMachine<StartPerson>*  m_pStateMachine;


	 // �X�^�[�g
	 tdn2DObj* m_startPic;

};
