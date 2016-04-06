#pragma once
#include "TDNLIB.h"

#include "AI\NPC\BasePerson\BasePerson.h"

/*
動かない人の種類
*/

class StrongPerson :public BasePerson
{
public:
	StrongPerson(ENTITY_ID id);
	virtual ~StrongPerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	virtual void  Render();
	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();


	// アクセサ
	// ★ステートマシンのアクセサを作る
	StateMachine<StrongPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ★ステートマシンを新たにベースパーソンから継承先に
	StateMachine<StrongPerson>*  m_pStateMachine;
};
