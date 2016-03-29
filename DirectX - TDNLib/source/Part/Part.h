#pragma once

//*****************************************************************************************************************************
//
//		パート管理クラス
//
//*****************************************************************************************************************************

class  PartManager
{
public:
	PartManager();
	~PartManager();
	void Update();
	void Render();

	enum class PART
	{
		MAIN,			// ゲームのメイン
		GAME_CLEAR,		// ゲームオーバー時の処理
		GAME_OVER,		// ゲームクリア時の処理
		MAX
	};

	// パート切り替え
	void ChangePart(PART p);

private:

	class Part
	{
	public:
		class Base
		{
		protected:
			PartManager *mgr;		// 管理してる人の実体(管理してる人のステップを進めるのに使う)
			int process;		// step担当してるけどさらに中のステップ
			int timer;			// 汎用的なタイマー
			enum class CURSOR{ LEFT, RIGHT, NONE }cursor;
		public:
			Base(PartManager *mgr) :mgr(mgr), process(0), timer(0){}
			virtual ~Base(){}
			virtual void Initialize(){}
			virtual void Update() = 0;
			virtual void Render() = 0;
		};

		class Main : public Base
		{
		public:
			Main(PartManager *mgr) :Base(mgr){}
			~Main(){}
			void Initialize();
			void Update();
			void Render();
		};

		class GameClear : public Base
		{
		public:
			GameClear(PartManager *mgr) :Base(mgr){}
			~GameClear(){}
			void Initialize();
			void Update();
			void Render();
		};

		class GameOver : public Base
		{
		public:
			GameOver(PartManager *mgr) :Base(mgr){}
			~GameOver(){}
			void Initialize();
			void Update();
			void Render();
		};
	};

	Part::Base *part_worker;

};