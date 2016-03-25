#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
#include "AI\Entity\EntityManager.h"
#include "AI\Message\MessageDispatcher.h"


// ★「テンプレート<クラスENTITY_TYPE>」で事前に宣言します
//template <class entity_type> class State;
template <class entity_type> class StateMachine;

/*
	基本の人の形
*/
class BasePerson :public BaseGameEntity
{
public:
	BasePerson(int id);
	virtual ~BasePerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	// メッセージ更新
	virtual bool  HandleMessage(const Message& msg);


	// ★この形にするからステートマシンのアクセサを作る
	StateMachine<BasePerson>* GetFSM()const { return m_pStateMachine; }


private:

	iex3DObj* m_obj;
	Vector3 m_pos;


	// ★ステートマシン
	StateMachine<BasePerson>*  m_pStateMachine;



};


