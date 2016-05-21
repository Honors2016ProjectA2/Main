#pragma once

//-------- include ---------
#include "IEX_Expansion.h"
#include "../CurvePoint/CurvePoint.h"

#if(0)// shutter type (true: アニメーション, false: 一枚絵)
#define _SPIC2015_SHUTTER_TYPE_ANIMATION_
#endif

//--------- class definition ----------
class tdn2DObj;
class DataManager;
class MousePointer;
class Watchman_mng;
class CurvePoint;

// 犬のラジオボタン
//struct CurvePointRadio
//{
//	CurvePoint *p[2];	// 実体
//	bool bCheck[2];		// チェックしたか
//
//	CurvePointRadio(){ FOR(2){ p[i] = nullptr; bCheck[i] = false; } }
//	~CurvePointRadio(){ FOR(2)SAFE_DELETE(p[i]); }
//	void Update(){ FOR(2)p[i]->Update(); }
//	void Render(){ FOR(2)p[i]->Render(); }
//};

//-------- Stage class ------
class Stage
{
	//------ field ---------
private:
	tdn2DObj* image;
	Vector2 pos;
	const int W;
	const int H;
	const int START_Y;
	const int SPEED_Y;
	const int SHUTTER_X;
	const int SHUTTER_Y;
	int offsetY;
public:
	enum StageState { NONE, FALL };
	StageState state;
	std::vector<CurvePoint*> *m_CPlist;

	//------- method --------
public:
	Stage();
	~Stage();
	void Init(Vector2 pos, StageState startState, std::vector<CurvePoint*> *CPlist);
	void Update();
	void Render();
	int GetWidth();
};

//-------- StageManager class ---------
class StageManager
{
	//---- field ------
public:
	int floor;         // 出現中の階層数-1
private:
	tdn2DObj *m_pDogImage;
	enum ImageSrc { STAGE, SHUTTER };

	int APPEND_STAGE_BORDER[STAGE_MAX]; // stage追加タイミングのスコア
	int m_AddScore[STAGE_MAX];			// レーンゴールしたときに加算されるスコア

	Stage* stage[STAGE_MAX];

	std::vector<CurvePoint*> m_CPlists[STAGE_MAX];	// 犬設置リスト(ステージに参照させるのは、テキスト読み込み一括でやりたかったため)
	int m_CPStock;									// 犬のストック

	//------ method --------
public:
	StageManager();
	~StageManager();
	void Init();
	void Reset();
	void Update();
	void Render();
	void CurvePointLock(int floorIdx);

	///<summary>各種データ取り込み</summary>
	void Reflection(DataManager* data, MousePointer* mouse);
	///<summary>シャッターが完全に開いているか</summary>
	///<param name="floorIdx">判定するフロアの添字</param>
	bool IsOpen(int floorIdx);

	std::vector<CurvePoint*> *GetCurvePointList(int floor){ return &m_CPlists[floor]; }

	Vector2 GetArrowPos(int floorIdx);
	Vector2 GetWarningPos(int floorIdx, bool IsRight);
	Vector2 GetPopupPos(int floorIdx, bool IsRight);
	Vector2 GetBalloonPos(int floorIdx);
};

extern int STAGE_POS_Y[3];
extern int LANE_WIDTH;