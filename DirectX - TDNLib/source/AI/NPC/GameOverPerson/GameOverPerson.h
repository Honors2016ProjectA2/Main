#pragma once
#include "TDNLIB.h"
#include "AI\NPC\BasePerson\BasePerson.h"

/*
	�Q�[���I�[�o�[�p�[�\��
*/


class GameOverPerson :public BasePerson
{
public:
	GameOverPerson(ENTITY_ID id);
	virtual ~GameOverPerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	virtual void  Render();
	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();


	// �A�N�Z�T
	// ���X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<GameOverPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ���X�e�[�g�}�V����V���Ƀx�[�X�p�[�\������p�����
	StateMachine<GameOverPerson>*  m_pStateMachine;
};
