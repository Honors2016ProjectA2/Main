#pragma once

//*****************************************************************************************************************************
//
//		パート管理クラス
//
//*****************************************************************************************************************************

#include "../Scene/SceneMain.h"

// 定数の前方宣言
enum class PERSON_TYPE;

// クラスの前方宣言
class TutorialManager;

// ボタンのID（シーンメインからここに移植しました）
enum BUTTON_ID
{
	CONTINUE_YES = 0,
	CONTINUE_NO = 1,
	RETRY = 2,
	BLUE = 3,
	RED = 4,
	GREEN = 5,
	START = 6,
	MAX
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
	void Enter(sceneMain* pMain);

	// 実行します
	void Execute(sceneMain* pMain);

	// 帰る
	void Exit(sceneMain* pMain);

	// 描画
	void Render(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(sceneMain* pMain, const Message& msg);

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
	void Enter(sceneMain* pMain);

	// 実行します
	void Execute(sceneMain* pMain);

	// 帰る
	void Exit(sceneMain* pMain);

	// 描画
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainIntro():m_timer(0) {};
	~sceneMainIntro() {};

	sceneMainIntro(const sceneMainIntro&);
	sceneMainIntro& operator=(const sceneMainIntro&);

	// イントロのタイマー
	int m_timer;

	// ステップ(順番に)
	int m_step;

	// クリア条件の画像
	tdn2DObj *m_pImage;
	Vector2 m_ImagePos;

	// ステップを進めるプチ関数
	void NextStep()
	{
		m_step++;
		m_timer = 0;
	}
};

//--------------------設置パート
class sceneMainSetPart :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainSetPart* GetInstance();

	// 入る
	void Enter(sceneMain* pMain);

	// 実行します
	void Execute(sceneMain* pMain);

	// 帰る
	void Exit(sceneMain* pMain);

	// 描画
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainSetPart() :m_HoldMesh(nullptr),m_pTutorial(nullptr){};
	~sceneMainSetPart();

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

	// チュートリアルを実行するポインタ
	TutorialManager *m_pTutorial;
};

//--------------------うわさが流れてるパート
class sceneMainGossip :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainGossip* GetInstance();

	// 入る
	void Enter(sceneMain* pMain);

	// 実行します
	void Execute(sceneMain* pMain);

	// 帰る
	void Exit(sceneMain* pMain);

	// 描画
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGossip() {};
	~sceneMainGossip() {};

	sceneMainGossip(const sceneMainGossip&);
	sceneMainGossip& operator=(const sceneMainGossip&);
};

//--------------------ゲームオーバー
class sceneMainGameOver :public State<sceneMain>
{
public:

	//this is a シングルトン
	static sceneMainGameOver* GetInstance();

	// 入る
	void Enter(sceneMain* pMain);

	// 実行します
	void Execute(sceneMain* pMain);

	// 帰る
	void Exit(sceneMain* pMain);

	// 描画
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(sceneMain* pMain, const Message& msg);

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
	void Enter(sceneMain* pMain);

	// 実行します
	void Execute(sceneMain* pMain);

	// 帰る
	void Exit(sceneMain* pMain);

	// 描画
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameClear() {};
	~sceneMainGameClear() {};

	sceneMainGameClear(const sceneMainGameOver&);
	sceneMainGameClear& operator=(const sceneMainGameOver&);
};