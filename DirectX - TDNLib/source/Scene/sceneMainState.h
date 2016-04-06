#pragma once

//*****************************************************************************************************************************
//
//		パート管理クラス
//
//*****************************************************************************************************************************

#include "../Scene/SceneMain.h"

// 定数の前方宣言
enum class PERSON_TYPE;

// ボタンのID（シーンメインからここに移植しました）
enum BUTTON_ID
{
	YES = 0,	// コンティニュー等の
	NO = 1,
	RETRY = 2,
	BLUE = 3,
	RED = 4,
	GREEN = 5,
	START = 6
};

// ゲーム状態定数
enum class GAME_STATE
{
	INTRO, MAIN, GAME_OVER, GAME_CLEAR
};
// ゲーム状態をグローバルで持つ
extern GAME_STATE g_GameState;

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

	// 描画
	virtual void Render(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGlobalState() {};
	~sceneMainGlobalState() {};

	sceneMainGlobalState(const sceneMainGlobalState&);
	sceneMainGlobalState& operator=(const sceneMainGlobalState&);
};

//--------------------ゲーム開始時のイントロ
class sceneMainIntro :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainIntro* GetInstance();

	// 入る
	virtual void Enter(sceneMain* pMain);

	// 実行します
	virtual void Execute(sceneMain* pMain);

	// 帰る
	virtual void Exit(sceneMain* pMain);

	// 描画
	virtual void Render(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainIntro():m_timer(0) {};
	~sceneMainIntro() {};

	sceneMainIntro(const sceneMainIntro&);
	sceneMainIntro& operator=(const sceneMainIntro&);

	// イントロのタイマー
	int m_timer;
};

//--------------------設置パート
class sceneMainSetPart :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainSetPart* GetInstance();

	// 入る
	virtual void Enter(sceneMain* pMain);

	// 実行します
	virtual void Execute(sceneMain* pMain);

	// 帰る
	virtual void Exit(sceneMain* pMain);

	// 描画
	virtual void Render(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainSetPart(){};
	~sceneMainSetPart() {};

	sceneMainSetPart(const sceneMainSetPart&);
	sceneMainSetPart& operator=(const sceneMainSetPart&);

	// 配置として描画するメッシュ
	iex3DObj *m_HoldMesh;
	Vector3 m_HoldMeshPos;

	// 選んだ左下のボタンの色
	enum class SELECT_BUTTON_COLOR
	{
		BLUE,
		RED,
		GREEN,
		MAX
	};
	int m_SelectButtonColor;	// プレイヤーが選択したボタン(++とかするのでint)

	// 左下のボタンの色と設置する人間の紐づけ
	std::map<SELECT_BUTTON_COLOR, PERSON_TYPE> ButtonPersonMap;

	// 選択ボタン色切り替え
	void ChangeSelectButton(sceneMain *pMain, SELECT_BUTTON_COLOR next);
};

//--------------------メイン(うわさが流れてるパート)
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

	// 描画
	virtual void Render(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGame() {};
	~sceneMainGame() {};

	sceneMainGame(const sceneMainGame&);
	sceneMainGame& operator=(const sceneMainGame&);
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

	// 描画
	virtual void Render(sceneMain* pMain);

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

	// 描画
	virtual void Render(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameClear() {};
	~sceneMainGameClear() {};

	sceneMainGameClear(const sceneMainGameOver&);
	sceneMainGameClear& operator=(const sceneMainGameOver&);
};