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
class EnemyManager;

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

	//------- method --------
public:
	Stage();
	~Stage();
	void Init(Vector2 pos, StageState startState);
	void Update();
	void Render();
	int GetWidth();
};

//-------- StageManager class ---------
class StageManager
{
	//---- field ------
public:
	//int floor;         // 出現中の階層数-1
private:
	// ステージの画像系
	enum StageImage
	{
		BACK,			// ステージの基本背景
		HOUSE_BACK,		// ステージの家の後ろの部分

		// ～ここにゲームキャラ達～

		HOUSE_FRONT,	// ステージの家の前の部分
		DOOR1_OPEN,		// 左上ドア空いてる
		DOOR1_CLOSE,	// 左上ドア閉まってる
		DOOR2_OPEN,		// 真ん中ドア空いてる
		DOOR2_CLOSE,	// 真ん中ドア閉まってる
		DOOR3_OPEN,		// 左下ドア空いてる
		DOOR3_CLOSE,	// 左下ドア閉まってる

		SAKU,			// 柵
		KUSA,			// 草

		IKENIE,			// いけにえ棒
		FIRE,			// 炎

		MAX
	};


	tdn2DObj *m_pStageImages[StageImage::MAX];
	int m_FireAnimFrame, m_FireAnimPanel;

	tdn2DObj *m_pDogImage, *m_pFireImage;

	enum ImageSrc { STAGE, SHUTTER };

	int APPEND_STAGE_BORDER[STAGE_MAX]; // stage追加タイミングのスコア
	int m_AddScore[STAGE_MAX];			// レーンゴールしたときに加算されるスコア
	Stage* stage[STAGE_MAX];

	std::vector<CurvePoint::Dog*> m_Doglists[STAGE_MAX];
	//std::vector<CurvePoint::Fire*> m_Firelists[STAGE_MAX];

	int m_DogStock;		// 犬のストック

	//------ method --------
public:
	StageManager();
	~StageManager();
	void Init();
	void Reset();
	void Update();

	// 描画
	void Render();
	void RenderBack();	// 背景とか描画
	void RenderFront();	// 草とか柵とか描画

	void CurvePointLock(int floorIdx);

	///<summary>各種データ取り込み</summary>
	void Reflection(DataManager* data, MousePointer* mouse);
	///<summary>シャッターが完全に開いているか</summary>
	///<param name="floorIdx">判定するフロアの添字</param>
	bool IsOpen(int floorIdx);

	std::vector<CurvePoint::Dog*> *GetDogList(int floor){ return &m_Doglists[floor]; }
	//std::vector<CurvePoint::Fire*> *GetFireList(int floor){ return &m_Firelists[floor]; }
	CurvePoint::Dog *GetDog(int floor, UINT id){ for (UINT i = 0; i < m_Doglists[floor].size(); i++){ if (m_Doglists[floor][i]->GetID() == id)return m_Doglists[floor][i]; } return nullptr; }
	//CurvePoint::Fire *GetFire(int floor, UINT id){ for (UINT i = 0; i < m_Firelists[floor].size(); i++){ if (m_Firelists[floor][i]->GetID() == id)return m_Firelists[floor][i]; } return nullptr; }

	Vector2 GetArrowPos(int floorIdx);
	Vector2 GetWarningPos(int floorIdx, bool IsRight);
	Vector2 GetPopupPos(int floorIdx, bool IsRight);
	Vector2 GetBalloonPos(int floorIdx);

	bool m_FireSelect;	// 炎設置かどうか
	int m_FireStock;	// 炎のストック
};

// 最短フロア検索関数
int FindFloor(float posY);

extern int STAGE_POS_Y[3];
extern int LANE_WIDTH;
extern Vector2 YAKINIKU_AREA;
extern bool g_bDogSetFrame;