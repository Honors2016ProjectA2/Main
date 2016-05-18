#pragma once

//-------- include ----------
#include "IEX_Expansion.h"

//------- protorype ---------
class UIManager;

//-------- DataManager class --------
class DataManager
{
	//------- field ---------
private:
	const static int LEVEL_MAX = 4;
	const static int TIME_FRAME[LEVEL_MAX];
	const static int LEVEL_UP_SCORE[LEVEL_MAX];
	int frame;
	int time;
	int diffTime;
	Vector2 pos;
	int score;
	int diffScore;
	int level;
	bool popFlg;

	// スコア加算する変数(ステージマネージャのやつをさんしょうして時間で入れ替えたりする)
	int *m_pAddScores[STAGE_MAX];

	//------- method --------
public:
	DataManager();
	~DataManager();
	void Init();
	void Update();
	void Reflection(UIManager* uiMNG);
	Vector2 GetPos();
	bool PopTrg();
	// time
	int GetTime()const;
	int GetDiffTime()const;
	void AddTime(int floorIdx, Vector2& pos);
	void SubTime_Kill(int floorIdx, Vector2& pos);
	void SubTime_Invade(int floorIdx, Vector2& pos);
	// score
	int GetScore()const;
	int GetDiffScore()const;
	void AddScore(int floorIdx);
	void SetpAddScore(int *pScore1, int *pScore2, int *pScore3){ m_pAddScores[0] = pScore1; m_pAddScores[1] = pScore2; m_pAddScores[2] = pScore3; }

};