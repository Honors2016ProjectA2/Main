#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class GoalPerson;


/*
最初の人のステートマシンのカートリッジ
*/


//-------------------グローバスステート
class GoalPersonGlobalState :public State<GoalPerson>
{
public:

	//this is a シングルトン
	static GoalPersonGlobalState* GetInstance();

	// 入る
	virtual void Enter(GoalPerson* pPerson);

	// 実行します
	virtual void Execute(GoalPerson* pPerson);

	// 帰る
	virtual void Exit(GoalPerson* pPerson);

	// 描画
	virtual void Render(GoalPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalPersonGlobalState() {};
	~GoalPersonGlobalState() {};

	GoalPersonGlobalState(const GoalPersonGlobalState&);
	GoalPersonGlobalState& operator=(const GoalPersonGlobalState&);
};



//--------------------最初

class GoalStart :public State<GoalPerson>
{
public:

	//this is a シングルトン
	static GoalStart* GetInstance();

	// 入る
	virtual void Enter(GoalPerson* pPerson);

	// 実行します
	virtual void Execute(GoalPerson* pPerson);

	// 帰る
	virtual void Exit(GoalPerson* pPerson);

	// 描画
	virtual void Render(GoalPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalStart() {};
	~GoalStart() {};

	GoalStart(const GoalStart&);
	GoalStart& operator=(const GoalStart&);
};


//--------------------流してる時の待機
class GoalShed :public State<GoalPerson>
{
public:

	//this is a シングルトン
	static GoalShed* GetInstance();

	// 入る
	virtual void Enter(GoalPerson* pPerson);

	// 実行します
	virtual void Execute(GoalPerson* pPerson);

	// 帰る
	virtual void Exit(GoalPerson* pPerson);

	// 描画
	virtual void Render(GoalPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalShed() {};
	~GoalShed() {};

	GoalShed(const GoalShed&);
	GoalShed& operator=(const GoalShed&);
};


//--------------------終わった時の待機
class GoalEnd :public State<GoalPerson>
{
public:

	//this is a シングルトン
	static GoalEnd* GetInstance();

	// 入る
	virtual void Enter(GoalPerson* pPerson);

	// 実行します
	virtual void Execute(GoalPerson* pPerson);

	// 帰る
	virtual void Exit(GoalPerson* pPerson);

	// 描画
	virtual void Render(GoalPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalEnd() {};
	~GoalEnd() {};

	GoalEnd(const GoalEnd&);
	GoalEnd& operator=(const GoalEnd&);
};

