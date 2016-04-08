#include "TDNLIB.h"

#include "Camera.h"
#include "CameraState.h"
#include "../AI/Message/Message.h"
#include "../AI/Entity/EntityManager.h"
#include "../AI/State/state.h"

CameraManager *CameraManager::pInstance = nullptr;

//*****************************************************************************************************************************
//			実体取得
//*****************************************************************************************************************************
CameraManager &CameraManager::GetInstance()
{
	if (!pInstance) pInstance = new CameraManager(ENTITY_ID::CAMERA_MNG);
	return *pInstance;
}


//*****************************************************************************************************************************
//			カメラ初期化・解放
//*****************************************************************************************************************************
void CameraManager::Initialize(bool bIntro)
{
	tdnView::Init();

	// 座標と注視点初期化
	m_pos = Vector3(.0f, 65.0f, -60.0f), m_target = Vector3(.0f, .0f, .0f);

	// ステートマシン初期化
	m_pStateMachine = new StateMachine<CameraManager>(this);
	m_pStateMachine->SetGlobalState(CameraGlobalState::GetInstance());// グローバル

	// 最初はステートをイントロカメラに
	if(bIntro)m_pStateMachine->SetCurrentState(IntroCameraState::GetInstance());
	else      m_pStateMachine->SetCurrentState(FixCameraState::GetInstance());
}

CameraManager::~CameraManager()
{
	SAFE_DELETE(m_pStateMachine);
}

void CameraManager::Release()
{
	// 1つしかない実体の削除(↑のデストラクタを呼び出す)
	SAFE_DELETE(pInstance);
}

//*****************************************************************************************************************************
//			カメラ更新
//*****************************************************************************************************************************
void CameraManager::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// ビュー情報セット
	tdnView::Set(m_pos, m_target);
}

//*****************************************************************************************************************************
//			カメラアクティベート
//*****************************************************************************************************************************
void CameraManager::Activate()
{
	tdnView::Activate();
	tdnView::Clear(0x00000000);
}

//*****************************************************************************************************************************
//			ステートマシンのメッセージ
//*****************************************************************************************************************************
bool CameraManager::HandleMessage(const Message & msg)
{
	// イントロカメラお願いしますメッセージ
	//if (msg.Msg == MESSAGE_TYPE::START_INTRO_CAMERA)
	//{
	//	// ステートをイントロカメラに
	//	m_pStateMachine->SetCurrentState(IntroCameraState::GetInstance());
	//	return true;
	//}

	return m_pStateMachine->HandleMessage(msg);
}


bool CameraManager::isIntro()
{
	// イントロ中ならtrue
	return m_pStateMachine->isInState(*IntroCameraState::GetInstance());
}