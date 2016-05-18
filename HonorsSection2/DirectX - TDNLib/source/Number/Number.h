#pragma once
#include "TDNLIB.h"

/***********************************/
//	数字を描画する
/***********************************/

class Number
{
private:
	// 数字の色のタイプ
	enum class NUM_KIND
	{
		NORMAL, HEAL
	};

public:

	// 実体
	//static Number& GetInstance();
	//static void Release();

	// (TODO)後で演出を変える分岐をする
	 Number();
	virtual ~Number();

	// 更新・描画
	void Update();
	void Render();
	void Render(int x, int y, int num, NUM_KIND kind = NUM_KIND::NORMAL);

	// アクション
	void Action(int derayTime = 0);

	// イラストのアクセサー
	tdn2DAnim* GetAnim() { return m_pic; }

private:
	// 実体
	//static Number* m_pInstance;

	// 数字のイラスト
	tdn2DAnim* m_pic;
	// 数字のサイズ
	int m_picSize;
	int m_picScale;

	// 封印
	//Number();
	//Number(const Number&);
	//Number& operator=(const Number&);




};


/***********************************/
//	↑に上がってくる数字を演出するマネージャー
/***********************************/



// ↓で使う数字の
struct NumberData
{
	enum class STATE
	{
		START,
		ARRIVAL,
		END
	};
	STATE state;

	Number* number;
	int x, y;
	int score;

	int flame;
	int endFlame;
	int alpha;

	// 終了フラグ
	bool isEnd;

};

class Number_Effect
{
private:


public:

	// 実体
	static Number_Effect& GetInstance();
	static void Release();
	//Number_Effect();
	virtual ~Number_Effect();

	// 更新・描画
	void Update();
	void Render();

	// 数字追加
	void AddNumber(int x,int y,int score);

	//// NumberData
	//NumberData  

private:
	// 実体
	static Number_Effect* m_pInstance;

	// 数字のデータ
	std::list<NumberData*> m_NumberData;
	std::list<NumberData*>::iterator m_it;

	// 封印
	Number_Effect();
	Number_Effect(const Number_Effect&);
	Number_Effect& operator=(const Number_Effect&);

};

// インスタンス化
#define NumberEffect	(Number_Effect::GetInstance())
