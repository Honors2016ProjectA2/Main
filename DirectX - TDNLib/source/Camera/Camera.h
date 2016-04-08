#pragma once


//*****************************************************************************************************************************
//
//		カメラ制御クラス
//
//*****************************************************************************************************************************

// 前方宣言ではダメだったので、インクルードに
#include "../AI/State/StateMachine.h"
#include "../AI/Entity/BaseGameEntity.h"

class CameraManager : BaseGameEntity
{
public:
	// 実体取得
	static CameraManager &GetInstance();

	// 初期化・解放
	void Initialize(bool bIntro = true);
	void Release();
	~CameraManager();

	// 更新(純粋仮想オーバーライド)
	void Update();

	void Activate();

	// メッセージを取り扱う(純粋仮想オーバーライド)
	bool  HandleMessage(const Message& msg);


	// ステートマシンのアクセサ
	StateMachine<CameraManager>* GetFSM()const { return m_pStateMachine; }

	// セッターゲッター
	Vector3 &GetPos(){ return m_pos; }
	void SetPos(const Vector3 &pos){ m_pos = pos; }
	Vector3 &GetTarget(){ return m_target; }
	void SetTarget(const Vector3 &target){ m_target = target; }
	Vector3 &GetOrgPos(){ return m_OrgPos; }
	float GetOrgAngle(){ return m_OrgAngle; }

	// イントロ中か否か
	bool isIntro();

private:
	// 1つしかない自分の実体
	static CameraManager *pInstance;

	// 座標、注視点
	Vector3 m_pos;
	Vector3 m_target;

	// 演出カメラの原点座標・アングル
	Vector3 m_OrgPos;
	float m_OrgAngle;

	// ★ステートマシン
	StateMachine<CameraManager> *m_pStateMachine;

	// 封印
	CameraManager(ENTITY_ID id):BaseGameEntity(id){}
	//CameraManager(const CameraManager&){}
	CameraManager &operator=(const CameraManager&){}
};

// インスタンス取得簡易化
#define CameraMgr (CameraManager::GetInstance())