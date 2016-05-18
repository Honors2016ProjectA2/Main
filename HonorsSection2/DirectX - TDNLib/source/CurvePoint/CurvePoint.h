#pragma once

//--------- CurvePointLock class -----------
//--------- CurvePoint class ---------
class CurvePoint
{
public:
	enum CurvePointState { OPEN, CLOSE };
	//--------- field ---------
private:
	tdn2DObj* image;
	const int W;
	const int H;
	const int SX;
	const int SY;
	const int SW;
	const int SH;
	Vector2 pos;
	CurvePointState state;
	int timer;
	int speed;
	DIR m_dir;	// 羊を曲げる方向
#ifdef _SPIC2015_SHUTTER_TYPE_ANIMATION_
	const int FRAME_MAX = 0;
	int frame;
#else
	const int WAIT_TIME;
	int waitTimer;
	const int SPEED_Y;
	int height;
#endif
public:
	const int LOCK_TIME;
	int lockTimer;

	//------- method ---------
public:
	CurvePoint(tdn2DObj* dogImage, Vector2 pos, DIR dir);
	~CurvePoint();
	void Update();
	void Render();
	void SetPos(int y);
	void Open();
	void Close();
	void Stop();
	void Change(){ state = IsOpening() ? CurvePointState::CLOSE : CurvePointState::OPEN; }		// ON_OFFの切り替え
	bool IsOpening();
	int GetWidth();
	DIR GetDir(){ return m_dir; }		// カーブの方向取得
	Vector2 &GetPos(){ return pos; }
	void Get_pos2(Vector2 &out){ out = pos; }

	bool m_bCursorIn;	// マウスカーソルが範囲内に入ってるかどうか
};