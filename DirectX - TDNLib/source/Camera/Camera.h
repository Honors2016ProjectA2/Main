#pragma once


//*****************************************************************************************************************************
//
//		�J��������N���X
//
//*****************************************************************************************************************************

// �O���錾�ł̓_���������̂ŁA�C���N���[�h��
#include "../AI/State/StateMachine.h"
#include "../AI/Entity/BaseGameEntity.h"

class Camera : BaseGameEntity
{
public:

	// �������E���
	void Initialize();
	~Camera();

	// �X�V(�������z�I�[�o�[���C�h)
	void Update();

	void Activate();

	// ���b�Z�[�W����舵��(�������z�I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg);


	// �X�e�[�g�}�V���̃A�N�Z�T
	StateMachine<Camera>* GetFSM()const { return m_pStateMachine; }

	// �Z�b�^�[�Q�b�^�[
	Vector3 &GetPos(){ return m_pos; }
	void SetPos(const Vector3 &pos){ m_pos = pos; }
	Vector3 &GetTarget(){ return m_target; }
	void SetTarget(const Vector3 &target){ m_target = target; }
	Vector3 &GetOrgPos(){ return m_OrgPos; }
	float GetOrgAngle(){ return m_OrgAngle; }

private:

	// ���W�A�����_
	Vector3 m_pos;
	Vector3 m_target;

	// ���o�J�����̌��_���W�E�A���O��
	Vector3 m_OrgPos;
	float m_OrgAngle;

	// ���X�e�[�g�}�V��
	StateMachine<Camera> *m_pStateMachine;
};