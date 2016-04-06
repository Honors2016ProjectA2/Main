#pragma once
#include "TDNLIB.h"
#include "AI\NPC\BasePerson\BasePerson.h"

/*
	ストップパーソン
*/


class StopPerson :public BasePerson
{
public:
	StopPerson(ENTITY_ID id);
	virtual ~StopPerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	virtual void  Render();
	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();


	// アクセサ
	// ★ステートマシンのアクセサを作る
	StateMachine<StopPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ★ステートマシンを新たにベースパーソンから継承先に
	StateMachine<StopPerson>*  m_pStateMachine;
};
