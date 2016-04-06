#pragma once
#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class StopPerson;


/*
	噂をかき消す人のステートマシンのカートリッジ
*/


//-------------------グローバスステート
class StopPersonGlobalState :public State<StopPerson>
{
public:

	//this is a シングルトン
	static StopPersonGlobalState* GetInstance();

	// 入る
	virtual void Enter(StopPerson* pPerson);

	// 実行します
	virtual void Execute(StopPerson* pPerson);

	// 帰る
	virtual void Exit(StopPerson* pPerson);

	// 描画
	virtual void Render(StopPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	StopPersonGlobalState() {};
	~StopPersonGlobalState() {};

	StopPersonGlobalState(const StopPersonGlobalState&);
	StopPersonGlobalState& operator=(const StopPersonGlobalState&);
};



//--------------------最初

class StartStop :public State<StopPerson>
{
public:

	//this is a シングルトン
	static StartStop* GetInstance();

	// 入る
	virtual void Enter(StopPerson* pPerson);

	// 実行します
	virtual void Execute(StopPerson* pPerson);

	// 帰る
	virtual void Exit(StopPerson* pPerson);

	// 描画
	virtual void Render(StopPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	StartStop() {};
	~StartStop() {};

	StartStop(const StartStop&);
	StartStop& operator=(const StartStop&);
};


//--------------------流してる時の待機
class ShedStop :public State<StopPerson>
{
public:

	//this is a シングルトン
	static ShedStop* GetInstance();

	// 入る
	virtual void Enter(StopPerson* pPerson);

	// 実行します
	virtual void Execute(StopPerson* pPerson);

	// 帰る
	virtual void Exit(StopPerson* pPerson);

	// 描画
	virtual void Render(StopPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	ShedStop() {};
	~ShedStop() {};

	ShedStop(const ShedStop&);
	ShedStop& operator=(const ShedStop&);
};


//--------------------終わった時の待機
class EndStop :public State<StopPerson>
{
public:

	//this is a シングルトン
	static EndStop* GetInstance();

	// 入る
	virtual void Enter(StopPerson* pPerson);

	// 実行します
	virtual void Execute(StopPerson* pPerson);

	// 帰る
	virtual void Exit(StopPerson* pPerson);

	// 描画
	virtual void Render(StopPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	EndStop() {};
	~EndStop() {};

	EndStop(const EndStop&);
	EndStop& operator=(const EndStop&);
};

