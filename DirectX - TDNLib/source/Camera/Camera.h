#pragma once


//*****************************************************************************************************************************
//
//		カメラ制御クラス
//
//*****************************************************************************************************************************

// 前方宣言ではダメだったので、インクルードに
#include "../AI/State/StateMachine.h"
#include "../AI/Entity/BaseGameEntity.h"

class Camera : BaseGameEntity
{
public:

	// 初期化・解放
	void Initialize();
	~Camera();

	// 更新(純粋仮想オーバーライド)
	void Update();

	void Activate();

	// メッセージを取り扱う(純粋仮想オーバーライド)
	bool  HandleMessage(const Message& msg);


	// ステートマシンのアクセサ
	StateMachine<Camera>* GetFSM()const { return m_pStateMachine; }

	// セッターゲッター
	Vector3 &GetPos(){ return m_pos; }
	void SetPos(const Vector3 &pos){ m_pos = pos; }
	Vector3 &GetTarget(){ return m_target; }
	void SetTarget(const Vector3 &target){ m_target = target; }
	Vector3 &GetOrgPos(){ return m_OrgPos; }
	float GetOrgAngle(){ return m_OrgAngle; }

private:

	// 座標、注視点
	Vector3 m_pos;
	Vector3 m_target;

	// 演出カメラの原点座標・アングル
	Vector3 m_OrgPos;
	float m_OrgAngle;

	// ★ステートマシン
	StateMachine<Camera> *m_pStateMachine;
};