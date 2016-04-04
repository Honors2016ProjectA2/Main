#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class WaitPerson;


/*
	泊まっている人ののステートマシンのカートリッジ
*/


//-------------------グローバスステート
class WaitPersonGlobalState :public State<WaitPerson>
{
public:

	//this is a シングルトン
	static WaitPersonGlobalState* GetInstance();

	// 入る
	virtual void Enter(WaitPerson* pPerson);

	// 実行します
	virtual void Execute(WaitPerson* pPerson);

	// 帰る
	virtual void Exit(WaitPerson* pPerson);

	// 描画
	virtual void Render(WaitPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	WaitPersonGlobalState() {};
	~WaitPersonGlobalState() {};

	WaitPersonGlobalState(const WaitPersonGlobalState&);
	WaitPersonGlobalState& operator=(const WaitPersonGlobalState&);
};



//--------------------最初

class StartWait :public State<WaitPerson>
{
public:

	//this is a シングルトン
	static StartWait* GetInstance();

	// 入る
	virtual void Enter(WaitPerson* pPerson);

	// 実行します
	virtual void Execute(WaitPerson* pPerson);

	// 帰る
	virtual void Exit(WaitPerson* pPerson);

	// 描画
	virtual void Render(WaitPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	StartWait() {};
	~StartWait() {};

	StartWait(const StartWait&);
	StartWait& operator=(const StartWait&);
};


//--------------------流してる時の待機
class ShedWait :public State<WaitPerson>
{
public:

	//this is a シングルトン
	static ShedWait* GetInstance();

	// 入る
	virtual void Enter(WaitPerson* pPerson);

	// 実行します
	virtual void Execute(WaitPerson* pPerson);

	// 帰る
	virtual void Exit(WaitPerson* pPerson);

	// 描画
	virtual void Render(WaitPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	ShedWait() {};
	~ShedWait() {};

	ShedWait(const ShedWait&);
	ShedWait& operator=(const ShedWait&);
};


//--------------------終わった時の待機
class EndWait :public State<WaitPerson>
{
public:

	//this is a シングルトン
	static EndWait* GetInstance();

	// 入る
	virtual void Enter(WaitPerson* pPerson);

	// 実行します
	virtual void Execute(WaitPerson* pPerson);

	// 帰る
	virtual void Exit(WaitPerson* pPerson);

	// 描画
	virtual void Render(WaitPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	EndWait() {};
	~EndWait() {};

	EndWait(const EndWait&);
	EndWait& operator=(const EndWait&);
};

