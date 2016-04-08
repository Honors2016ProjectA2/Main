#include "TDNLIB.h"

#include "Camera.h"
#include "CameraState.h"
#include "../AI/Message/Message.h"
#include "../AI/Entity/EntityManager.h"
#include "../AI/State/state.h"

CameraManager *CameraManager::pInstance = nullptr;

//*****************************************************************************************************************************
//			���̎擾
//*****************************************************************************************************************************
CameraManager &CameraManager::GetInstance()
{
	if (!pInstance) pInstance = new CameraManager(ENTITY_ID::CAMERA_MNG);
	return *pInstance;
}


//*****************************************************************************************************************************
//			�J�����������E���
//*****************************************************************************************************************************
void CameraManager::Initialize(bool bIntro)
{
	tdnView::Init();

	// ���W�ƒ����_������
	m_pos = Vector3(.0f, 65.0f, -60.0f), m_target = Vector3(.0f, .0f, .0f);

	// �X�e�[�g�}�V��������
	m_pStateMachine = new StateMachine<CameraManager>(this);
	m_pStateMachine->SetGlobalState(CameraGlobalState::GetInstance());// �O���[�o��

	// �ŏ��̓X�e�[�g���C���g���J������
	if(bIntro)m_pStateMachine->SetCurrentState(IntroCameraState::GetInstance());
	else      m_pStateMachine->SetCurrentState(FixCameraState::GetInstance());
}

CameraManager::~CameraManager()
{
	SAFE_DELETE(m_pStateMachine);
}

void CameraManager::Release()
{
	// 1�����Ȃ����̂̍폜(���̃f�X�g���N�^���Ăяo��)
	SAFE_DELETE(pInstance);
}

//*****************************************************************************************************************************
//			�J�����X�V
//*****************************************************************************************************************************
void CameraManager::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �r���[���Z�b�g
	tdnView::Set(m_pos, m_target);
}

//*****************************************************************************************************************************
//			�J�����A�N�e�B�x�[�g
//*****************************************************************************************************************************
void CameraManager::Activate()
{
	tdnView::Activate();
	tdnView::Clear(0x00000000);
}

//*****************************************************************************************************************************
//			�X�e�[�g�}�V���̃��b�Z�[�W
//*****************************************************************************************************************************
bool CameraManager::HandleMessage(const Message & msg)
{
	// �C���g���J�������肢���܂����b�Z�[�W
	//if (msg.Msg == MESSAGE_TYPE::START_INTRO_CAMERA)
	//{
	//	// �X�e�[�g���C���g���J������
	//	m_pStateMachine->SetCurrentState(IntroCameraState::GetInstance());
	//	return true;
	//}

	return m_pStateMachine->HandleMessage(msg);
}


bool CameraManager::isIntro()
{
	// �C���g�����Ȃ�true
	return m_pStateMachine->isInState(*IntroCameraState::GetInstance());
}