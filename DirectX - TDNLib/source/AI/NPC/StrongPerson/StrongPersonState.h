#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class StrongPerson;


/*
泊まっている人ののステートマシンのカートリッジ
*/


//-------------------グローバスステート
class StrongPersonGlobalState :public State<StrongPerson>
{
public:

	//this is a シングルトン
	static StrongPersonGlobalState* GetInstance();

	// 入る
	virtual void Enter(StrongPerson* pPerson);

	// 実行します
	virtual void Execute(StrongPerson* pPerson);

	// 帰る
	virtual void Exit(StrongPerson* pPerson);

	// 描画
	virtual void Render(StrongPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	StrongPersonGlobalState() {};
	~StrongPersonGlobalState() {};

	StrongPersonGlobalState(const StrongPersonGlobalState&);
	StrongPersonGlobalState& operator=(const StrongPersonGlobalState&);
};



//--------------------最初

class StartStrong :public State<StrongPerson>
{
public:

	//this is a シングルトン
	static StartStrong* GetInstance();

	// 入る
	virtual void Enter(StrongPerson* pPerson);

	// 実行します
	virtual void Execute(StrongPerson* pPerson);

	// 帰る
	virtual void Exit(StrongPerson* pPerson);

	// 描画
	virtual void Render(StrongPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	StartStrong() {};
	~StartStrong() {};

	StartStrong(const StartStrong&);
	StartStrong& operator=(const StartStrong&);
};


//--------------------流してる時の待機
class ShedStrong :public State<StrongPerson>
{
public:

	//this is a シングルトン
	static ShedStrong* GetInstance();

	// 入る
	virtual void Enter(StrongPerson* pPerson);

	// 実行します
	virtual void Execute(StrongPerson* pPerson);

	// 帰る
	virtual void Exit(StrongPerson* pPerson);

	// 描画
	virtual void Render(StrongPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	ShedStrong() {};
	~ShedStrong() {};

	ShedStrong(const ShedStrong&);
	ShedStrong& operator=(const ShedStrong&);
};


//--------------------終わった時の待機
class EndStrong :public State<StrongPerson>
{
public:

	//this is a シングルトン
	static EndStrong* GetInstance();

	// 入る
	virtual void Enter(StrongPerson* pPerson);

	// 実行します
	virtual void Execute(StrongPerson* pPerson);

	// 帰る
	virtual void Exit(StrongPerson* pPerson);

	// 描画
	virtual void Render(StrongPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	EndStrong() {};
	~EndStrong() {};

	EndStrong(const EndStrong&);
	EndStrong& operator=(const EndStrong&);
};

