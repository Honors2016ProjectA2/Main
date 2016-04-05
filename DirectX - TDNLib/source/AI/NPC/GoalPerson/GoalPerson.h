#pragma once
#include "TDNLIB.h"
#include  "AI\NPC\BasePerson\BasePerson.h"


/*
	ゴールの人

*/

class GoalPerson:public BasePerson
{
public:
	GoalPerson(ENTITY_ID id);
	virtual ~GoalPerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	virtual void  Render();
	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();

	// アクセサ
	// ★ステートマシンのアクセサを作る
	StateMachine<GoalPerson>* GetFSM()const { return m_pStateMachine; }
private:

	// ★ステートマシンを新たにベースパーソンから継承先に
	StateMachine<GoalPerson>*  m_pStateMachine;
};
