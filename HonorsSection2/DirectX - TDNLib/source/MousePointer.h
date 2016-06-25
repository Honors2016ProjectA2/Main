#pragma once
#include "TDNLIB.h"
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

	/*　ポインターの状態に関して　*/
	enum class STATE
	{
		NOMAL,	// 普通の状態
		CATCH	// 羊の上に来たときと、肉の上に来たとき
	};

	void SetState(STATE s);

private:
	STATE m_state;
	tdn2DObj* pointerObj = nullptr;
	tdn2DAnim* m_Effect;
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