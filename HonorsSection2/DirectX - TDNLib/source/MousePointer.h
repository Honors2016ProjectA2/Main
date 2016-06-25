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

	/*�@�z�C�[���Ɋւ��ā@*/
	enum WHEEL_STATE{
		NOT_MOVE, UP, DOWN
	};
	WHEEL_STATE GetWheelState();	//���݂̃z�C�[���̏�Ԃ�����Ă���

	/*�@�|�C���^�[�̏�ԂɊւ��ā@*/
	enum class STATE
	{
		NOMAL,	// ���ʂ̏��
		CATCH	// �r�̏�ɗ����Ƃ��ƁA���̏�ɗ����Ƃ�
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
	//���݃}�E�X�|�C���^�����K�ɂ��邩�i������O�`�Q
	int GetNowFloor(){
		return nowFloor;
	}

};