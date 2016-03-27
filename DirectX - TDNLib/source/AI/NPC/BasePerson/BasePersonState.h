#pragma once
#include "AI\State\StateMachine.h"
//#include "AI\Message\Message.h"
//#include "AI\Message\MessageDispatcher.h"

class BasePerson;

/*
	人のステートマシンのカートリッジ

*/


//-------------------グローバスステート
class PersonGlobalState :public State<BasePerson>
{
public:

	//this is a シングルトン
	static PersonGlobalState* GetInstance();

	// 入る
	virtual void Enter(BasePerson* pPerson);

	// 実行します
	virtual void Execute(BasePerson* pPerson);

	// 帰る
	virtual void Exit(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonGlobalState() {};
	~PersonGlobalState() {};

	PersonGlobalState(const PersonGlobalState&);
	PersonGlobalState& operator=(const PersonGlobalState&);
};



//--------------------待機
class PersonWait:public State<BasePerson>
{
public:

	//this is a シングルトン
	static PersonWait* GetInstance();

	// 入る
	virtual void Enter(BasePerson* pPerson);

	// 実行します
	virtual void Execute(BasePerson* pPerson);

	// 帰る
	virtual void Exit(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonWait() {};
	~PersonWait() {};

	PersonWait(const PersonWait&);
	PersonWait& operator=(const PersonWait&);
};

//--------------------流した後の待機
class PersonShedWait :public State<BasePerson>
{
public:

	//this is a シングルトン
	static PersonShedWait* GetInstance();

	// 入る
	virtual void Enter(BasePerson* pPerson);

	// 実行します
	virtual void Execute(BasePerson* pPerson);

	// 帰る
	virtual void Exit(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonShedWait() {};
	~PersonShedWait() {};

	PersonShedWait(const PersonShedWait&);
	PersonShedWait& operator=(const PersonShedWait&);
};

