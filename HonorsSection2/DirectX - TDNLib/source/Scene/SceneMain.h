#pragma once

#include	"system/BaseScene.h"
#include	"IEX_Expansion.h"
class Ready;
class End;
class MousePointer;
class StageManager;
class DataManager;
class SheepManager;
class Result;
class Result2;

class sceneMain	: public BaseScene
{
public:

	//‰Šú‰»E‰ğ•ú
	bool Initialize();
	~sceneMain();
	//ˆ—
	bool Update();
	//•`‰æ
	void Render();
private:
	tdn2DObj* back = nullptr;
	Ready* ready = nullptr;
	End* end = nullptr;
	MousePointer* pointer = nullptr;
	StageManager* stage = nullptr;
	DataManager* dataMNG = nullptr;
	SheepManager* m_pSheepMgr = nullptr;
	Result2 * result = nullptr;

	tdn2DObj* byunAlpha = nullptr;

	Surface* backUp;
	tdn2DObj* renderTarget = nullptr;

	int state = 0;

	bool isResultFlag;

	//INIT, TITLE, EXPLAIN, READY, MAIN, END, RESULT
	void Init();
	//XVˆ—
	void ReadyEvent();
	void MainUpdate();
	void EndEvent();
	void ResultUpdate();

	void DataDelivery();	//î•ñó‚¯“n‚µ

	//•`‰æˆ—
	void ReadyRender();
	void MainRender();
	void EndRender();
	void ResultRender();

	void DebugText();
};