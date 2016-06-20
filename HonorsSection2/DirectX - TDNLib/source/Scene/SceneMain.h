#pragma once

#include	"system/BaseScene.h"
#include	"IEX_Expansion.h"
class Ready;
class End;
class MousePointer;
class StageManager;
class DataManager;
class Result;
class Result2;
class Explain;
class Tips;

class sceneMain	: public BaseScene
{
public:

	//初期化・解放
	bool Initialize();
	~sceneMain();
	//処理
	bool Update();
	//描画
	void Render();
private:
	tdn2DObj* back = nullptr;
	Explain* explain = nullptr;
	Ready* ready = nullptr;
	End* end = nullptr;
	Tips* tips = nullptr;

	MousePointer* pointer = nullptr;
	StageManager* stage = nullptr;
	DataManager* dataMNG = nullptr;
	//Result2 * result = nullptr;
	tdn2DObj* byunAlpha = nullptr;

	Surface* backUp;
	tdn2DObj* renderTarget = nullptr;

	int state = 0;

	bool isResultFlag;



	//INIT, TITLE, EXPLAIN, READY, MAIN, END, RESULT
	void Init();
	//更新処理
	void ExplainUpdate();
	void ReadyEvent();
	void MainUpdate();
	void EndEvent();
	void ResultUpdate();
	void TipsUpdate();

	void DataDelivery();	//情報受け渡し

	//描画処理
	void ExplainRender();
	void ReadyRender();
	void MainRender();
	void EndRender();
	void ResultRender();
	void TipsRender();

	void DebugText();

public:
	// ポーズメニュー
	struct POSE
	{
		tdn2DAnim* pic;
		int x, y;
		bool isAction;

		tdn2DAnim* rip;
	};
	POSE m_stop;
	POSE m_again;

	struct POSE_TIMER 
	{
		tdn2DAnim* one;
		tdn2DAnim* two;
		tdn2DAnim* three;
	};
	POSE_TIMER m_poseTimer;

	enum class POSE_STATE
	{
		START, EXE, COUNT, TITLE
	};
	POSE_STATE m_poseState;

	struct PoseIcon
	{
		tdn2DObj* pic;
		int x, y;

	};
	PoseIcon m_poseIcon;

private:

	void PoseUpdate();
	void PoseRender();
};