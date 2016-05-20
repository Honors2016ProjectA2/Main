#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include	"system/BaseScene.h"
#include	"IEX_Expansion.h"
class Ready;
class End;
class MousePointer;
class StageManager;
class LightCtrl;
class DataManager;
class SheepManager;
class Watchman_mng;
class Result;


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
	LightCtrl* light = nullptr;
	DataManager* dataMNG = nullptr;
	SheepManager* m_pSheepMgr = nullptr;
	Watchman_mng* watchman = nullptr;
	Result * result = nullptr;

	tdn2DObj* byunAlpha = nullptr;

	Surface* backUp;
	tdn2DObj* renderTarget = nullptr;

	int state = 0;

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

	//—h‚êŠÖ˜A
	struct Shake{
		void Init();
		void Update();
		void SetFloor(int floor);
		Vector2 move;
		int timer;
		int waitTimer;
		int beforeFloor;
	}shake;
};

#endif // !_SCENEMAIN_H_