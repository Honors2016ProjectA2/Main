#pragma once
#include "TDNLIB.h"
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

	// 描画
	virtual void Render(BasePerson* pPerson);

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

	// 描画
	virtual void Render(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonWait() {};
	~PersonWait() {};

	PersonWait(const PersonWait&);
	PersonWait& operator=(const PersonWait&);
};


//--------------------流してる時の待機
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

	// 描画
	virtual void Render(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonShedWait() {};
	~PersonShedWait() {};

	PersonShedWait(const PersonShedWait&);
	PersonShedWait& operator=(const PersonShedWait&);
};


//--------------------終わった時の待機
class PersonEndWait :public State<BasePerson>
{
public:

	//this is a シングルトン
	static PersonEndWait* GetInstance();

	// 入る
	virtual void Enter(BasePerson* pPerson);

	// 実行します
	virtual void Execute(BasePerson* pPerson);

	// 帰る
	virtual void Exit(BasePerson* pPerson);

	// 描画
	virtual void Render(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonEndWait() {};
	~PersonEndWait() {};

	PersonEndWait(const PersonEndWait&);
	PersonEndWait& operator=(const PersonEndWait&);
};





//--------------------走る
class PersonRun :public State<BasePerson>
{
public:

	//this is a シングルトン
	static PersonRun* GetInstance();

	// 入る
	virtual void Enter(BasePerson* pPerson);

	// 実行します
	virtual void Execute(BasePerson* pPerson);

	// 帰る
	virtual void Exit(BasePerson* pPerson);

	// 描画
	virtual void Render(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonRun() {};
	~PersonRun() {};

	PersonRun(const PersonRun&);
	PersonRun& operator=(const PersonRun&);

	
	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};


//--------------------流してる時の待機
class PersonShedRun :public State<BasePerson>
{
public:

	//this is a シングルトン
	static PersonShedRun* GetInstance();

	// 入る
	virtual void Enter(BasePerson* pPerson);

	// 実行します
	virtual void Execute(BasePerson* pPerson);

	// 帰る
	virtual void Exit(BasePerson* pPerson);

	// 描画
	virtual void Render(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonShedRun() {};
	~PersonShedRun() {};

	PersonShedRun(const PersonShedRun&);
	PersonShedRun& operator=(const PersonShedRun&);
};



//--------------------終わった時の走り
class PersonEndRun :public State<BasePerson>
{
public:

	//this is a シングルトン
	static PersonEndRun* GetInstance();

	// 入る
	virtual void Enter(BasePerson* pPerson);

	// 実行します
	virtual void Execute(BasePerson* pPerson);

	// 帰る
	virtual void Exit(BasePerson* pPerson);

	// 描画
	virtual void Render(BasePerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonEndRun() {};
	~PersonEndRun() {};

	PersonEndRun(const PersonEndRun&);
	PersonEndRun& operator=(const PersonEndRun&);
};

