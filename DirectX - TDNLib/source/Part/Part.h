#pragma once

//*****************************************************************************************************************************
//
//		パート管理クラス
//
//*****************************************************************************************************************************

class sceneMain;

class sceneMainGlobalState :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainGlobalState* GetInstance();

	// 入る
	virtual void Enter(sceneMain* pMain);

	// 実行します
	virtual void Execute(sceneMain* pMain);

	// 帰る
	virtual void Exit(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGlobalState() {};
	~sceneMainGlobalState() {};

	sceneMainGlobalState(const sceneMainGlobalState&);
	sceneMainGlobalState& operator=(const sceneMainGlobalState&);
};


//--------------------メイン
class sceneMainGame :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainGame* GetInstance();

	// 入る
	virtual void Enter(sceneMain* pMain);

	// 実行します
	virtual void Execute(sceneMain* pMain);

	// 帰る
	virtual void Exit(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGame() {};
	~sceneMainGame() {};

	sceneMainGame(const sceneMainGame&);
	sceneMainGame& operator=(const sceneMainGame&);

	// 波紋の回数
	int m_RippleCount;

};

//--------------------ゲームオーバー
class sceneMainGameOver :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainGameOver* GetInstance();

	// 入る
	virtual void Enter(sceneMain* pMain);

	// 実行します
	virtual void Execute(sceneMain* pMain);

	// 帰る
	virtual void Exit(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameOver() {};
	~sceneMainGameOver() {};

	sceneMainGameOver(const sceneMainGameOver&);
	sceneMainGameOver& operator=(const sceneMainGameOver&);
};

//--------------------ゲームクリアー
class sceneMainGameClear :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainGameClear* GetInstance();

	// 入る
	virtual void Enter(sceneMain* pMain);

	// 実行します
	virtual void Execute(sceneMain* pMain);

	// 帰る
	virtual void Exit(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameClear() {};
	~sceneMainGameClear() {};

	sceneMainGameClear(const sceneMainGameOver&);
	sceneMainGameClear& operator=(const sceneMainGameOver&);
};