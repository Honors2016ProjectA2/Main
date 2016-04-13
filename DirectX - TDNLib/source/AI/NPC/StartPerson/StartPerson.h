#pragma once
#include "TDNLIB.h"

#include "AI\NPC\BasePerson\BasePerson.h"

/*
	始めの人の種類
*/

class StartPerson:public BasePerson
{
public:
	StartPerson(ENTITY_ID id);
	virtual ~StartPerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	virtual void  Render();
	virtual void  UIRender();


	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg);
	
	void ResetState();


	// アクセサ
	// ★ステートマシンのアクセサを作る
	StateMachine<StartPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ★ステートマシンを新たにベースパーソンから継承先に
	 StateMachine<StartPerson>*  m_pStateMachine;


	 // スタート
	 tdn2DObj* m_startPic;

};
