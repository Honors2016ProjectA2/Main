#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
#include "AI\Entity\EntityManager.h"
#include "AI\Message\MessageDispatcher.h"
#include "GossipRipple\GossipRipple.h"


// ★「テンプレート<クラスENTITY_TYPE>」で事前に宣言します
//template <class entity_type> class State;
template <class entity_type> class StateMachine;

/*
	人の種類
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
	基本の人の形
*/
class BasePerson :public BaseGameEntity
{
public:
	BasePerson(ENTITY_ID id);
	virtual ~BasePerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	
	// 
	virtual void  Render();
	
	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg) = 0;

	// 噂を流す波紋
	void ActionGossipRipple();

	// アクセサ
	// ★この形にするからステートマシンのアクセサを作る
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

	// 波紋を流してる最中
	//bool IsShedNow();

	// ★ベースにステートマシンがなく触れないが
	// virtureを利用して各々のステートマシンに呼びかける事にトライ！
	virtual void ResetState() = 0; // 初期のステートに戻す


	// 今は直に
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

	bool m_isShed;	// ~は噂を流したか

	bool m_isStay;  // 最初からいるか


	// ★ステートマシン
	// StateMachine<BasePerson>*  m_pStateMachine;

	// 一人に一つ波紋
	GossipRipple* m_Ripple;

};


