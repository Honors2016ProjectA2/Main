#pragma once
#include "TDNLIB.h"

#include "AI\NPC\BasePerson\BasePerson.h"

/*
	動かない人の種類
*/

class WaitPerson:public BasePerson
{
public:
	WaitPerson(ENTITY_ID id);
	virtual ~WaitPerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	virtual void  Render();
	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg);

	// アクセサ
	// ★ステートマシンのアクセサを作る
	StateMachine<WaitPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ★ステートマシンを新たにベースパーソンから継承先に
	 StateMachine<WaitPerson>*  m_pStateMachine;
};
