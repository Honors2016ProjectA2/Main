#include	"MousePointer.h"
#include	"Stage/StageMNG.h"

namespace{
	const int FLOOR_SIZE = 720/3;
	const int MAX_FLOOR = 3;
}

MousePointer::MousePointer()
{
	pointerObj = new tdn2DObj("DATA/icon.png");
	ShowCursor(false);
}

MousePointer::~MousePointer()
{
	SAFE_DELETE(pointerObj);
}

void MousePointer::Update()
{
	Vector2 rcv(tdnMouse::GetPos());
	posX = (int)rcv.x;
	posY = (int)rcv.y;

	CheckFloor();
}

void MousePointer::CheckFloor()
{
	if( posY <= FLOOR_SIZE ){
		nowFloor = 2;
	}else if( posY <= FLOOR_SIZE*2 ){
		nowFloor = 1;
	}else{
		nowFloor = 0;
	}

	//�o���t���A���Ɉʒu�����߂�
	if( nowFloor > appearedFloor ){
		nowFloor = appearedFloor;
	}
}

MousePointer::WHEEL_STATE MousePointer::GetWheelState()
{
	if( tdnMouse::GetWheelFlag() == WHEEL_FLAG::UP ){
		return WHEEL_STATE::UP;
	}
	if( tdnMouse::GetWheelFlag() == WHEEL_FLAG::DOWN ){
		return WHEEL_STATE::DOWN;
	}

	return WHEEL_STATE::NOT_MOVE;
}

MousePointer::CLICK MousePointer::GetClickState()
{
	if (tdnMouse::GetLeft() == 1){
		return CLICK::LEFT;
	}
	if (tdnMouse::GetRight() == 1){
		return CLICK::RIGHT;
	}

	return CLICK::NONE;
}

void MousePointer::Render()
{
	//�����|�C���^�̉摜�������ւ��鎖������΁A������
	pointerObj->Render(posX, posY, 47, 64, 0,0,47,64);
}

void MousePointer::DataReceive(StageManager* stage)
{
	appearedFloor = 2;
}