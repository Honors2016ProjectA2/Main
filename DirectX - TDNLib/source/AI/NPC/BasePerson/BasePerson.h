#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
#include "AI\Entity\EntityManager.h"
#include "AI\Message\MessageDispatcher.h"


// ���u�e���v���[�g<�N���XENTITY_TYPE>�v�Ŏ��O�ɐ錾���܂�
//template <class entity_type> class State;
template <class entity_type> class StateMachine;

/*
	��{�̐l�̌`
*/
class BasePerson :public BaseGameEntity
{
public:
	BasePerson(int id);
	virtual ~BasePerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	// ���b�Z�[�W�X�V
	virtual bool  HandleMessage(const Message& msg);


	// �����̌`�ɂ��邩��X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<BasePerson>* GetFSM()const { return m_pStateMachine; }


private:

	iex3DObj* m_obj;
	Vector3 m_pos;


	// ���X�e�[�g�}�V��
	StateMachine<BasePerson>*  m_pStateMachine;



};


