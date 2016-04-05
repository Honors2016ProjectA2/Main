#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class StartPerson;


/*
	最初の人のステートマシンのカートリッジ
*/


//-------------------グローバスステート
class StartPersonGlobalState :public State<StartPerson>
{
public:

	//this is a シングルトン
	static StartPersonGlobalState* GetInstance();

	// 入る
	virtual void Enter(StartPerson* pPerson);

	// 実行します
	virtual void Execute(StartPerson* pPerson);

	// 帰る
	virtual void Exit(StartPerson* pPerson);

	// 描画
	virtual void Render(StartPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	StartPersonGlobalState() {};
	~StartPersonGlobalState() {};

	StartPersonGlobalState(const StartPersonGlobalState&);
	StartPersonGlobalState& operator=(const StartPersonGlobalState&);
};



//--------------------最初

class FirstStart :public State<StartPerson>
{
public:

	//this is a シングルトン
	static FirstStart* GetInstance();

	// 入る
	virtual void Enter(StartPerson* pPerson);

	// 実行します
	virtual void Execute(StartPerson* pPerson);

	// 帰る
	virtual void Exit(StartPerson* pPerson);

	// 描画
	virtual void Render(StartPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	FirstStart() {};
	~FirstStart() {};

	FirstStart(const FirstStart&);
	FirstStart& operator=(const FirstStart&);
};


//--------------------流してる時の待機
class FirstShed :public State<StartPerson>
{
public:

	//this is a シングルトン
	static FirstShed* GetInstance();

	// 入る
	virtual void Enter(StartPerson* pPerson);

	// 実行します
	virtual void Execute(StartPerson* pPerson);

	// 帰る
	virtual void Exit(StartPerson* pPerson);

	// 描画
	virtual void Render(StartPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	FirstShed() {};
	~FirstShed() {};

	FirstShed(const FirstShed&);
	FirstShed& operator=(const FirstShed&);
};


//--------------------終わった時の待機
class FirstEnd :public State<StartPerson>
{
public:

	//this is a シングルトン
	static FirstEnd* GetInstance();

	// 入る
	virtual void Enter(StartPerson* pPerson);

	// 実行します
	virtual void Execute(StartPerson* pPerson);

	// 帰る
	virtual void Exit(StartPerson* pPerson);

	// 描画
	virtual void Render(StartPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	FirstEnd() {};
	~FirstEnd() {};

	FirstEnd(const FirstEnd&);
	FirstEnd& operator=(const FirstEnd&);
};

