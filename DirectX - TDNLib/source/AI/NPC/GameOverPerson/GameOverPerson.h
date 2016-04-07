#pragma once
#include "TDNLIB.h"
#include "AI\NPC\BasePerson\BasePerson.h"

/*
	ゲームオーバーパーソン
*/


class GameOverPerson :public BasePerson
{
public:
	GameOverPerson(ENTITY_ID id);
	virtual ~GameOverPerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	virtual void  Render();
	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg);

	void ResetState();


	// アクセサ
	// ★ステートマシンのアクセサを作る
	StateMachine<GameOverPerson>* GetFSM()const { return m_pStateMachine; }

private:

	// ★ステートマシンを新たにベースパーソンから継承先に
	StateMachine<GameOverPerson>*  m_pStateMachine;
};
