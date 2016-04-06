#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
噂の波紋マネージャー.h

*/

class BasePerson;


// クリア条件定数
enum class CLEAR_FLAG
{
	ALL_SHED = 0,		// 全員に流したら
	GOAL_PERSON = 1		// 特定キャラに噂を流したら
};

class JudgeManager :public BaseGameEntity
{
public:
	static JudgeManager& GetInstance();
	static void Release();
	virtual ~JudgeManager();

	// 純粋仮想関数により継承したもの
	void Update();
	bool HandleMessage(const Message& msg);

	// クリア条件設定
	void SetClearFlag(CLEAR_FLAG flag);

	// 現在の状態定数
	enum class JUDGEMODE
	{
		NONE,			// ゲーム進行中
		GAME_CLEAR,		// ゲームクリア
		GAME_OVER		// ゲームオーバー
	}m_JudgeMode;

	// ★クリアしたか、ゲームオーバーになったかのゲッター
	bool isGameClear(){ return (m_JudgeMode == JUDGEMODE::GAME_CLEAR); }
	bool isGameOver(){ return (m_JudgeMode == JUDGEMODE::GAME_OVER); }

	// クリアしたか、ゲームオーバーになったかのセッター
	// クリアしたよーメッセージを受信するのはマネージャーではなく、持っているステートマシンが受信するので、ステートマシンからのアクセスが必要になる)
	void SetJudgeMode(JUDGEMODE mode){ m_JudgeMode = mode; }

private:
	static JudgeManager* pInstance;

	// ステートマシン(HandleMessageで使うので)
	StateMachine<JudgeManager> *m_pStateMachine;



	// 封印
	JudgeManager();
	JudgeManager(const JudgeManager&);
	JudgeManager& operator=(const JudgeManager&);


};

#define JudgeMgr (JudgeManager::GetInstance())





//*****************************************************************************************************************************
//
//		クリア判定のステート(面倒なのでここに書く)
//
//*****************************************************************************************************************************

// 名前が被る可能性もあるので、namespaceで区切る
namespace Judge
{

//--------------------グローバルステート
	class GlobalState :public State<JudgeManager>
	{
	public:

		//this is a シングルトン
		static GlobalState* GetInstance()
		{
			static GlobalState inst;
			return &inst;
		}

		// 入る
		void Enter(JudgeManager* pJudge){}

		// 実行します
		void Execute(JudgeManager* pJudge){}

		// 帰る
		void Exit(JudgeManager* pJudge){}

		// 描画
		void Render(JudgeManager* pJudge){}

		// 使うのはメッセージぐらい
		bool OnMessage(JudgeManager* pJudge, const Message& msg);

	private:
		GlobalState() {};
		~GlobalState() {};
		GlobalState(const GlobalState&){}
		GlobalState& operator=(const GlobalState&){}
	};

	//--------------------全員に流す
	class AllShed :public State<JudgeManager>
	{
	public:

		//this is a シングルトン
		static AllShed* GetInstance()
		{
			static AllShed inst;
			return &inst;
		}

		// 入る
		void Enter(JudgeManager* pJudge){}

		// 実行します
		void Execute(JudgeManager* pJudge){}

		// 帰る
		void Exit(JudgeManager* pJudge){}

		// 描画
		void Render(JudgeManager* pJudge){}

		// 使うのはメッセージぐらい
		virtual bool OnMessage(JudgeManager* pJudge, const Message& msg);

	private:
		AllShed() {};
		~AllShed() {};
		AllShed(const AllShed&){}
		AllShed& operator=(const AllShed&){}
	};

	//--------------------ゴール人物に噂を流す
	class GoalPerson :public State<JudgeManager>
	{
	public:

		//this is a シングルトン
		static GoalPerson* GetInstance()
		{
			static GoalPerson inst;
			return &inst;
		}

		// 入る
		virtual void Enter(JudgeManager* pJudge){}

		// 実行します
		virtual void Execute(JudgeManager* pJudge){}

		// 帰る
		virtual void Exit(JudgeManager* pJudge){}

		// 描画
		virtual void Render(JudgeManager* pJudge){}

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(JudgeManager* pJudge, const Message& msg);

	private:
		GoalPerson() {};
		~GoalPerson() {};
		GoalPerson(const GoalPerson&){}
		GoalPerson& operator=(const GoalPerson&){}
	};
}