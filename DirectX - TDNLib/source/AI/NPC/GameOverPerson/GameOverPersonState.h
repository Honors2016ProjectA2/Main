#pragma once
#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class GameOverPerson;


/*
	噂をかき消す人のステートマシンのカートリッジ
*/


//-------------------グローバスステート
class GameOverPersonGlobalState :public State<GameOverPerson>
{
public:

	//this is a シングルトン
	static GameOverPersonGlobalState* GetInstance();

	// 入る
	virtual void Enter(GameOverPerson* pPerson);

	// 実行します
	virtual void Execute(GameOverPerson* pPerson);

	// 帰る
	virtual void Exit(GameOverPerson* pPerson);

	// 描画
	virtual void Render(GameOverPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	GameOverPersonGlobalState() {};
	~GameOverPersonGlobalState() {};

	GameOverPersonGlobalState(const GameOverPersonGlobalState&);
	GameOverPersonGlobalState& operator=(const GameOverPersonGlobalState&);
};



//--------------------最初

class StartGameOver :public State<GameOverPerson>
{
public:

	//this is a シングルトン
	static StartGameOver* GetInstance();

	// 入る
	virtual void Enter(GameOverPerson* pPerson);

	// 実行します
	virtual void Execute(GameOverPerson* pPerson);

	// 帰る
	virtual void Exit(GameOverPerson* pPerson);

	// 描画
	virtual void Render(GameOverPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	StartGameOver() {};
	~StartGameOver() {};

	StartGameOver(const StartGameOver&);
	StartGameOver& operator=(const StartGameOver&);
};


//--------------------流してる時の待機
class ShedGameOver :public State<GameOverPerson>
{
public:

	//this is a シングルトン
	static ShedGameOver* GetInstance();

	// 入る
	virtual void Enter(GameOverPerson* pPerson);

	// 実行します
	virtual void Execute(GameOverPerson* pPerson);

	// 帰る
	virtual void Exit(GameOverPerson* pPerson);

	// 描画
	virtual void Render(GameOverPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	ShedGameOver() {};
	~ShedGameOver() {};

	ShedGameOver(const ShedGameOver&);
	ShedGameOver& operator=(const ShedGameOver&);
};


//--------------------終わった時の待機
class EndGameOver :public State<GameOverPerson>
{
public:

	//this is a シングルトン
	static EndGameOver* GetInstance();

	// 入る
	virtual void Enter(GameOverPerson* pPerson);

	// 実行します
	virtual void Execute(GameOverPerson* pPerson);

	// 帰る
	virtual void Exit(GameOverPerson* pPerson);

	// 描画
	virtual void Render(GameOverPerson* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	EndGameOver() {};
	~EndGameOver() {};

	EndGameOver(const EndGameOver&);
	EndGameOver& operator=(const EndGameOver&);
};

