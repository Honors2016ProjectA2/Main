#include	"MousePointer.h"
#include	"Stage/StageMNG.h"
#include	"Sheep/Sheep.h"
#include	"Niku/Niku.h"

namespace{
	const int FLOOR_SIZE = 720/3;
	const int MAX_FLOOR = 3;
}

MousePointer::MousePointer()
{
	pointerObj = new tdn2DObj("DATA/icon.png");
	m_Effect = new tdn2DAnim("DATA/ring.png");
	m_Effect->OrderRipple(8, 1.0f, 0.1f);
	m_state = STATE::NOMAL;

	ShowCursor(false);
}

MousePointer::~MousePointer()
{
	SAFE_DELETE(m_Effect);
	SAFE_DELETE(pointerObj);
}

void MousePointer::Update()
{
	Vector2 rcv(tdnMouse::GetPos());
	posX = (int)rcv.x;
	posY = (int)rcv.y;

	CheckFloor();
	
	if (tdnMouse::GetLeft()==3)
	{
		m_Effect->Action();
	}

	m_Effect->Update();

	// ���������Ń}�E�X�J�[�\���̕ύX�X�V���s���I
	if(g_pSheepMgr)m_state = (g_pSheepMgr->isSheepPointerIn() || NikuMgr->isNikuPointerIn()) ? STATE::CATCH : STATE::NOMAL;
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

void MousePointer::SetState(STATE s)
{
	m_state = s;
}

void MousePointer::Render()
{

	// �G�t�F�N�g
	m_Effect->Render(posX - 32, posY - 32);

	//�����|�C���^�̉摜�������ւ��鎖������΁A������
	pointerObj->Render(posX, posY, 64, 64, (tdnMouse::GetLeft() == 1) ? 64 : 0, (int)m_state * 64, 64, 64);

	

}

void MousePointer::DataReceive(StageManager* stage)
{
	appearedFloor = 2;
}