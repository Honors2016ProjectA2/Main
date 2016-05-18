#ifndef _SPIC2015_UIMNG_H_
#define _SPIC2015_UIMNG_H_

//------ include ---------
#include <vector>
#include "IEX_Expansion.h"

//------ class definition ---------
class tdn2DObjEx;
class StageManager;
class DataManager;
class SheepManager;
class Watchman_mng;

//--------- Number class ----------
class NumberUI
{
	//------- field ----------
protected:
	// number param
	const int NUM_W;
	const int NUM_H;
	const int NUM_SX;
	const int NUM_SY;
	const int NUM_SW;
	const int NUM_SH;
	const int NUM_SR;
	const int NUM_SC;
	// unit param
	const int UNIT_W;
	const int UNIT_H;
	const int UNIT_SX;
	const int UNIT_SY;
	const int UNIT_SW;
	const int UNIT_SH;
	int num;
public:
	int endPosX;

	//------- method ----------
public:
	NumberUI(int sw,int sh);
	virtual ~NumberUI();
	virtual void Init();
	void Render_Left(tdn2DObj* number, tdn2DObj* unit, Vector2 pos);
	void Render_Right(tdn2DObj* number, tdn2DObj* unit, Vector2 pos);
	virtual void SetNum(int value);
};

//---------- BaseUI class ------------
class BaseUI
{
	//----- field -------
protected:
	tdn2DObjEx* image;
	const int W;
	const int H;
	const int SX;
	const int SY;
	const int SW;
	const int SH;
	const int SR;
	const int SC;
	Vector2 pos;
	int frame;
	int animeFrameMax;
	int animeRoopTime;
	int offsetBlockR;
	int offsetBlockC;
	int timer;
	int state;
public:
	bool IsFinish;

	//----- method -------
public:
	BaseUI(tdn2DObjEx* image, Vector2& pos, int w, int h, int sx, int sy, int sw, int sh,
		   int sr, int sc, int frameMax, int roopTime);
	BaseUI(const BaseUI& r);
	BaseUI& operator =(const BaseUI& r);
	virtual ~BaseUI();
	virtual void Update();
	virtual void Render();
	void SetImage(tdn2DObjEx* image);
};

//---------- UI_Time class -----------
class UI_Time :public NumberUI
{
	//----- field ------
private:
	const int LABEL_W;
	const int LABEL_H;
	const int LABEL_SX;
	const int LABEL_SY;
	const int LABEL_SW;
	const int LABEL_SH;
	int trueNum; // 実際の値
	int addNum;  // 1フレーム当たりの加算値

	//---- method ------
public:
	UI_Time();
	~UI_Time();
	void Init();
	void Update();
	void Render(tdn2DObj* label, tdn2DObj* number, tdn2DObj* unit, Vector2 pos);
	void SetNum(int value)override;
};

//---------- UI_Score class -------------
class UI_Score :public NumberUI
{
	//----- field ------
private:
	int trueNum; // 実際の値
	int addNum;  // 1フレーム当たりの加算値

	//---- method ------
public:
	UI_Score();
	~UI_Score();
	void Init();
	void Update();
	void Render(tdn2DObj* number, tdn2DObj* unit, Vector2 pos);
	void SetNum(int value)override;
};

//------- UI_LockCount class -----------
class UI_LockCount :public NumberUI
{
	//----- field -------
private:
	const int BAL_W;
	const int BAL_H;
	const int BAL_SX;
	const int BAL_SY;
	const int BAL_SW;
	const int BAL_SH;
	const float SCALE_SPEED;
	Vector2 pos;
	float scale;
	enum BalloonState { NONE, FADE_IN, FADE_OUT, WAIT, COUNT };
	int timer;
	BalloonState state;
public:
	bool IsFinish;

	//----- method --------
public:
	UI_LockCount(Vector2& pos);
	~UI_LockCount();
	void FadeIn();
	void FadeOut();
	void Update();
	void Render(tdn2DObj* number, tdn2DObjEx* balloon);
};

//---------- UI_DifPop class -------------
class UI_DifPop :public BaseUI
{
	//----- field -------
private:
	int alpha;
	int moveY;
public:
	enum PopType { PLUS = 0, MINUS_1 = 1, MINUS_2 = 2 };
	enum PopNum { NUM_0 = 0, NUM_1 = 1, NUM_2 = 2 };

	//---- method ------
public:
	UI_DifPop(tdn2DObjEx* image, Vector2& pos, PopType type, PopNum num);
	UI_DifPop(const UI_DifPop& r);
	UI_DifPop& operator =(const UI_DifPop& r);
	void Update()override;
	void Render()override;
};

//---------- UI_Arrow class -------------
class UI_Arrow :public BaseUI
{
	//------ field -------
public:
	int alpha;
	enum AlphaState { NONE, FADE_IN, FADE_OUT };
	const int FADE_SPEED;

	//---- method ------
public:
	UI_Arrow(tdn2DObjEx* image, Vector2& pos);
	UI_Arrow(const UI_Arrow& r);
	UI_Arrow& operator =(const UI_Arrow& r);

	void Update()override;
	void Render()override;
	void FadeIn();
	void FadeOut();
};

//--------- UI_Warning class -------------
class UI_Warning :public BaseUI
{
	//--------- field ---------
public:
	bool renderFlg;
	const int FLASH_FRAME;

	//---- method ------
public:
	UI_Warning(tdn2DObjEx* image, Vector2& pos, bool IsRedColor, bool IsRight);
	UI_Warning(const UI_Warning& r);
	UI_Warning& operator =(const UI_Warning& r);
	void Update()override;
	void Render()override;
};

//-------- UI_SpeedUp class -----------
class UI_SpeedUp :public BaseUI
{
	//------- field ----------
public:
	tdn2DObjEx* imageArrow;
	const int A_W;
	const int A_H;
	const int A_SX;
	const int A_SY;
	const int A_SW;
	const int A_SH;
	bool renderFlg;
	const int FLASH_FRAME;

	//-------- method ----------
public:
	UI_SpeedUp(tdn2DObjEx* labelImage, tdn2DObjEx*arrowImage);
	UI_SpeedUp(const UI_SpeedUp& r);
	UI_SpeedUp& operator =(const UI_SpeedUp& r);
	void Update()override;
	void Render(Vector2& pos);
};

//------- UI_Mouse class ----------
class UI_Mouse : public BaseUI
{
	//------- field -------
public:
	bool renderFlg;
	const int FLASH_FRAME;
	const int ALPHA_SPEED;
	int alpha;

	//------ method --------
public:
	UI_Mouse(tdn2DObjEx* image, Vector2& pos);
	UI_Mouse(const UI_Mouse& r);
	UI_Mouse& operator =(const UI_Mouse& r);
	void Update()override;
	void Render()override;
};

//------- UIManager class ---------
class UIManager
{
	//------ field ----------
private:
	const static int SOURCE_MAX = 11;
	tdn2DObjEx* src[SOURCE_MAX];
	enum ImageSrc { TIME_TITLE, MOUSE, SHINNYO, NUMBER, DIF_POP, ARROW_RIGHT, ARROW_LEFT, WARNING, SPEED_UP, SPEED_ARROW, BALLOON };
	const static int ARROW_MAX = 3;
	const static int DIFPOP_MAX = 16;
	const static int WARNING_MAX = 6;
	const static int LOCK_MAX = 3;
	UI_Arrow* arrow[ARROW_MAX];
	UI_DifPop* difPop[DIFPOP_MAX];
	UI_Warning* warning[WARNING_MAX];
	UI_LockCount* lockCount[LOCK_MAX];
	UI_Time* time;
	UI_Score* score;
	UI_SpeedUp* speedUp;
	UI_Mouse* mouse;
public:
	bool sppedUpFlg;
	enum PopupType
	{
		PLUS_5, PLUS_10, PLUS_15,
		MINUS_5, MINUS_10, MINUS_15
	};

	//------- method ----------
private:
	void SetArrow(StageManager* stage, SheepManager* shinnyo);
	void SetWarning(Watchman_mng* watchman, StageManager* stage);

public:
	UIManager();
	~UIManager();
	void Init();
	void Update();
	void Render_Back();
	void Render_Front();
	void Render_End();
	void SetPopup(int num, Vector2& pos);
	void SetSpeedUp();
	void Reflection(StageManager* stage, SheepManager* shinnyo, Watchman_mng* watchman, DataManager* data);
};

#endif