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

	/*�@�z�C�[���Ɋւ��ā@*/
	enum WHEEL_STATE{
		NOT_MOVE, UP, DOWN
	};
	WHEEL_STATE GetWheelState();	//���݂̃z�C�[���̏�Ԃ�����Ă���

	/*�@�N���b�N�Ɋւ��ā@*/
	enum CLICK{
		NONE, RIGHT, LEFT
	};
	CLICK GetClickState();	//���݂̃N���b�N�̏�Ԃ�����Ă���

private:
	tdn2DObj* pointerObj = nullptr;
	int posX, posY;
	int nowFloor = 0;
	int appearedFloor = 0;

	void CheckFloor();

public:
	//���݃}�E�X�|�C���^�����K�ɂ��邩�i������O�`�Q
	int GetNowFloor(){
		return nowFloor;
	}

};