#pragma once
#include	"IEX_Expansion.h"
class StageManager;


class MousePointer
{
public:
	MousePointer();
	~MousePointer();
	void Update();
	void Render();
	void DataReceive(StageManager* stage);

	/*　ホイールに関して　*/
	enum WHEEL_STATE{
		NOT_MOVE, UP, DOWN
	};
	WHEEL_STATE GetWheelState();	//現在のホイールの状態を取ってくる

	/*　クリックに関して　*/
	enum CLICK{
		NONE, RIGHT, LEFT
	};
	CLICK GetClickState();	//現在のクリックの状態を取ってくる

private:
	tdn2DObj* pointerObj = nullptr;
	int posX, posY;
	int nowFloor = 0;
	int appearedFloor = 0;

	void CheckFloor();

public:
	//現在マウスポインタが何階にいるか（下から０～２
	int GetNowFloor(){
		return nowFloor;
	}

};