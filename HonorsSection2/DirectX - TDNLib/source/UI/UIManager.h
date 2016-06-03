#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"
#include "pie_graph\pie_graph.h"

#include "Effect\BaseEffect\BaseEffect.h"

// UI

class UIManager
{
public:

	// いつもの
	static UIManager& GetInstance();
	static void Release();
	virtual ~UIManager();

	// 更新・描画7
	void Update();
	void Render();
		
	// 上昇
	void AddScore(int score);
	void ConboCount();

	// 時間上昇
	void AddTimer(int timer);

	// グラフの設定
	void SetGraph(float graph) {
		m_percent = graph;
	}

	// タイマーのゲッター
	int GetTimer() { return m_timer; }
	void SetTimer(int timer) { m_timer = timer; }

	int GetWorfHappyCount() { return m_worfHappyCount; }
	void AddWorfHappyCount(int add = 1) { m_worfHappyCount += add; }


	int GetSheepKillCount() { return m_sheepKillCount; }
	void AddSheepKillCount(int add = 1) { m_sheepKillCount = add; }


	void GraphAction();
private:
	static UIManager* inst;

	void ScoreUpdate();
	void ScoreRender();
	void ConboUpdate();
	void ConboRender();
	void TimerUpdate();
	void TimerRender();
	void PointUpdate();
	void PointRender();
	void GraphUpdate();
	void GraphRender();


	void Debug();


	// 封印
	UIManager();
	UIManager(const UIManager&) {}
	UIManager& operator=(const UIManager&) {}
private: //変数

	int m_score;
	int m_scorePoint;

	int m_combo;

	// スコア
	Number* m_scoreNum;
	tdn2DObj* m_scorePic;

	// コンボ
	Number* m_comboNum;
	tdn2DObj* m_conboBG;
	tdn2DObj* m_conboGage;
	tdn2DObj* m_conboGageEnergy;
	float m_energyRate;

	// タイマー
	int m_timer;
	int m_flame;
	Number* m_timerNum;
	tdn2DObj* m_timerPic;
	//float timer;

	// 草ゲージ
	int m_graphX, m_graphY;
	tdn2DObj* m_graphBack;
	tdn2DObj* m_graphFlont;
	Pie_graph* m_graph;
	tdn2DAnim* m_graphRip;
	float m_percent;

	// 
	int m_worfHappyCount;
	int m_sheepKillCount;

};


// インスタンス化
#define UIMNG (UIManager::GetInstance())

