#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"
#include "pie_graph\pie_graph.h"

#include "Effect\BaseEffect\BaseEffect.h"
#include "UI\SendPower\SendPower.h"


// UI

class UIManager
{
public:

	// いつもの
	static UIManager& GetInstance();
	static void Release();
	virtual ~UIManager();

	void Init();

	// 更新・描画7
	void Update();
	void Render();
		
	// 上昇
	void AddScore(int score);
	void ConboCount();

	// 時間上昇
	void AddTimer(int timer);

	// 動く光
	void AddSendPower(char* filename, Vector3 startPos, Vector3 centerPos,
		Vector3 center2Pos, Vector3 endPos, int endFlame, int power);

	// グラフの設定
	void SetGraph(float graph) {
		m_percent = graph;
	}

	// タイマーのゲッター
	int GetTimer() { return m_timer; }
	void SetTimer(int timer) { m_timer = timer; }


	// 
	int GetWorfHappyCount() { return m_worfHappyCount; }
	void AddWorfHappyCount(int add = 1) { m_worfHappyCount += add; }

	int GetSheepKillCount() { return m_sheepKillCount; }
	void AddSheepKillCount(int add = 1) { m_sheepKillCount = add; }

	int GetScore() { return m_score; }
	int GetCombo() { return m_combo; }

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
	int m_timeColR, m_timeColG, m_timeColB;
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


	/****************/
	// ベジェの光
	/****************/
	//SendPower;
	// 数字のデータ
	std::list<SendPower*> m_SendPowerData;
	std::list<SendPower*>::iterator m_SendPowerIt;

	void SendPowerUpdate();
	void SendPowerRender();

};


// インスタンス化
#define UIMNG (UIManager::GetInstance())

