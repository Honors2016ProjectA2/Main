#pragma once


//*****************************************************************************************************************************
//
//		�J��������N���X
//
//*****************************************************************************************************************************

// �O���錾�ł̓_���������̂ŁA�C���N���[�h��
#include "../AI/State/StateMachine.h"
#include "../AI/Entity/BaseGameEntity.h"

class CameraManager : BaseGameEntity
{
public:
	// ���̎擾
	static CameraManager &GetInstance();

	// �������E���
	void Initialize(bool bIntro = true);
	void Release();
	~CameraManager();

	// �X�V(�������z�I�[�o�[���C�h)
	void Update();

	void Activate();

	// ���b�Z�[�W����舵��(�������z�I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg);


	// �X�e�[�g�}�V���̃A�N�Z�T
	StateMachine<CameraManager>* GetFSM()const { return m_pStateMachine; }

	// �Z�b�^�[�Q�b�^�[
	Vector3 &GetPos(){ return m_pos; }
	void SetPos(const Vector3 &pos){ m_pos = pos; }
	Vector3 &GetTarget(){ return m_target; }
	void SetTarget(const Vector3 &target){ m_target = target; }
	Vector3 &GetOrgPos(){ return m_OrgPos; }
	float GetOrgAngle(){ return m_OrgAngle; }

	// �C���g�������ۂ�
	bool isIntro();

private:
	// 1�����Ȃ������̎���
	static CameraManager *pInstance;

	// ���W�A�����_
	Vector3 m_pos;
	Vector3 m_target;

	// ���o�J�����̌��_���W�E�A���O��
	Vector3 m_OrgPos;
	float m_OrgAngle;

	// ���X�e�[�g�}�V��
	StateMachine<CameraManager> *m_pStateMachine;

	// ����
	CameraManager(ENTITY_ID id):BaseGameEntity(id){}
	//CameraManager(const CameraManager&){}
	CameraManager &operator=(const CameraManager&){}
};

// �C���X�^���X�擾�ȈՉ�
#define CameraMgr (CameraManager::GetInstance())