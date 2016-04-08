#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
#include "AI\Entity\EntityManager.h"
#include "AI\Message\MessageDispatcher.h"
#include "GossipRipple\GossipRipple.h"


// ���u�e���v���[�g<�N���XENTITY_TYPE>�v�Ŏ��O�ɐ錾���܂�
//template <class entity_type> class State;
template <class entity_type> class StateMachine;

/*
	�l�̎��
*/

enum class PERSON_TYPE
{
	WAIT,
	START,
	GOAL,
	STOP,
	STRONG,
	GAMEOVER
};


/*
	��{�̐l�̌`
*/
class BasePerson :public BaseGameEntity
{
public:
	BasePerson(ENTITY_ID id);
	virtual ~BasePerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	
	// 
	virtual void  Render();
	
	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg) = 0;

	// �\�𗬂��g��
	void ActionGossipRipple();

	// �A�N�Z�T
	// �����̌`�ɂ��邩��X�e�[�g�}�V���̃A�N�Z�T�����
	//StateMachine<BasePerson>* GetFSM()const { return m_pStateMachine; }

	Vector3 GetPos() { return m_pos; }
	void SetPos(Vector3 pos) { m_pos = pos; }

	Vector3 GetMove() { return m_move; }
	void SetMove(Vector3 move) { m_move = move; }

	float GetAngle() { return m_angle; }
	void SetAngle(float angle) { m_angle = angle; }

	GossipRipple* GetRipple() { return m_Ripple; }
	
	bool IsShed() { return m_isShed; };
	void SetIsShed(bool f) { m_isShed = f; }

	bool IsStay() { return m_isStay; };
	void SetIsStay(bool f) { m_isStay = f; }

	PERSON_TYPE GetPersonType() { return m_PersonType;	}
	
	iex3DObj* GetObj() { return m_obj; }

	// �g��𗬂��Ă�Œ�
	//bool IsShedNow();

	// ���x�[�X�ɃX�e�[�g�}�V�����Ȃ��G��Ȃ���
	// virture�𗘗p���Ċe�X�̃X�e�[�g�}�V���ɌĂт����鎖�Ƀg���C�I
	virtual void ResetState() = 0; // �����̃X�e�[�g�ɖ߂�


	// ���͒���
	Vector3 m_orgPos;
	Vector3 m_trunPos;
	float m_len;
	bool m_trunFlag;

protected:

	PERSON_TYPE m_PersonType;

	iex3DObj* m_obj;
	Vector3 m_pos;
	Vector3 m_move;

	float m_angle;

	bool m_isShed;	// ~�͉\�𗬂�����

	bool m_isStay;  // �ŏ����炢�邩


	// ���X�e�[�g�}�V��
	// StateMachine<BasePerson>*  m_pStateMachine;

	// ��l�Ɉ�g��
	GossipRipple* m_Ripple;

};


