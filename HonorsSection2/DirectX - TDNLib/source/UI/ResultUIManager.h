#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"
#include "pie_graph\pie_graph.h"

#include "Effect\BaseEffect\BaseEffect.h"
#include "Number\Number.h"
#include "Sheep\Sheep.h"

// ResultUI
class ResultUIManager
{
public:
	// ステップ(動きの流れ)
	enum class STEP
	{
		START,SCORE,SONOTA,EXE,SUM_CALC,RANK,RANKING_START, RANKING,END
	};
	STEP m_step;


	// いつもの
	static ResultUIManager& GetInstance();
	static void Release();
	virtual ~ResultUIManager();

	void Init();

	// 更新・描画
	bool Update();
	void Render();
	void ResultRender();
	void CircleRender();

	// ブラインド
	void MaskRender();
	tdn2DObj* m_blindPic;
	int m_blindSplit;
	float m_blindAngle;
	tdn2DObj* m_maskScreen;
	float m_blindRete;

	// 始動・終了
	void Action();
	void Stop();
	void StopUpdate();
	int m_screenAlpha;
	bool isStopFlag;

	void Debug();

	// ゲッター
	bool IsResultScreen() { return isResultScreen; }


	// サークル構造体
	struct Circle
	{
		float scale;
		tdn2DObj* pic;
		int x, y;
		int flame;

		int startX, startY;
		int endX, endY;
		int endFlame;
	};
	Circle m_invCircle;
	Circle m_circle;
	bool CircleUpdate();

	// フォント達
	enum FONT
	{
		RESULT, SCORE, COMBO, WORF, FIRE, SUM, END
	};

	struct Font
	{
		tdn2DAnim* pic;
		int x, y;
		bool isSE;

	};
	//Font m_font[END];

	// その他
	tdn2DAnim* m_senPic;// 線
	tdn2DAnim* m_helpPic;// ヘルプ

	//　数字達
	Number* m_number[FONT::END];
	int m_numX[FONT::END], m_numY[FONT::END];
	int m_numNum[FONT::END];
	int m_MaxSumNum;
	
	int m_WaitFlame;// やばい

	// ランク処理
	enum RANK_TYPE
	{
		SS,S,A,B,C
	};
	int m_rankType;
	tdn2DAnim* m_RankPic;
	tdn2DAnim* m_RankRipPic;// 波紋エフェクト
	int m_RankPicX, m_RankPicY;

	tdn2DAnim* m_RankINPic;
	tdn2DAnim* m_RankFrame;

	void RankUpdate();

	//
	tdn2DObj* m_resultBG;

	ResultSheepManager* m_SheepMGN;

	Font  m_sumFont;

	// ボード達
	//tdn2DAnim* m_boadScore;
	//tdn2DAnim* m_boadEx;

	//int m_boadScoreX, m_boadScoreY;
	//int m_boadExX, m_boadExY;
private:
	static ResultUIManager* inst;

	// 封印
	ResultUIManager();
	ResultUIManager(const ResultUIManager&) {}
	ResultUIManager& operator=(const ResultUIManager&) {}
private: //変数

	// アルファ考慮のレンダーターゲット
	tdn2DObj* m_resultScreen;
	tdn2DObj* m_circleScreen;
	tdn2DObj* m_screen;
	Surface* pBackBuffer;

	bool isResultScreen;

	// ランキング板
	tdn2DObj* m_rankingScreen;
	tdn2DObj* m_rankingBG;
	tdn2DObj* m_rankingFont;

	void RankingRender();

	void Load();
	void Save();
	void Sort();


	static const int RANKING_MAX = 4;
	int m_RankingNum[RANKING_MAX];

	Number* m_rankingNumber[RANKING_MAX];
	tdn2DObj* m_RankingPic;

};


// インスタンス化
#define RESULT_UIMNG (ResultUIManager::GetInstance())

