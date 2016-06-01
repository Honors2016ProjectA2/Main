#include	"End.h"
#include	"../Stage/StageMNG.h"

namespace{
	namespace STATE{
		enum{
			TEXT_POP, EXPOINT_DASH, RET_TRUE
		};
	}
	const int SCALE_TIME = 30;
	const float SCALE_MAX = 1.8f;

	const int MAX_EX_POINT = 11*3*2;
	const int FLOOR_Y[3] = { 550, 310, 80 };
	const int FLOOR_X[11] = { 11, 34, 67, 46, 104, 93, 127, 166, 143, 189, 155 };
	const int BASE_X_L = 1280;//-50
	const int BASE_X_R = 1280+320;
	const int WAIT_TIME = 180;

	const int MAX_FLOOR = 3;
}

End::End()
{
	timeUp = new tdn2DObjEx("DATA/TimeUp.png");
	m_sheepPic = new tdn2DObjEx("DATA/CHR/sheep_animation.png");
	m_wolfPic = new tdn2DObjEx("DATA/CHR/「！」左移動.png");
	data = new ExPointData[MAX_EX_POINT];
}

End::~End()
{
	SAFE_DELETE(timeUp);
	SAFE_DELETE(m_sheepPic);
	SAFE_DELETE(m_wolfPic);
	SAFE_DELETE_ARRAY(data);
}

void End::Init()
{
	state = STATE::TEXT_POP;
	timeUpScale = .0f;
	waitTimer = 0;

	for(int i=0; i<MAX_EX_POINT; i++){
		Vector2 pos;
		int moveAngle;
		tdn2DObj* pointer;

		//高さは下から順に
		int floorNum = i%(MAX_EX_POINT/2);
		floorNum /= MAX_EX_POINT/2/3;
		pos.y = (float)FLOOR_Y[floorNum];
		int floor = floorNum;

		//横座標
		floorNum = i%(MAX_EX_POINT/2);
		floorNum %= MAX_EX_POINT/2/3;
		int x = FLOOR_X[floorNum];

		//前半は右側に、後半は左側に設置（よって移動方向などは逆）
		if( i<MAX_EX_POINT/2 ){
			moveAngle = ExPointData::LEFT;
			pointer = m_sheepPic;          // 羊の絵のポインターへ
			pos.x = +(float)(x + BASE_X_R);// 羊も←へ
		}else{
			moveAngle = ExPointData::RIGHT;
			pointer = m_wolfPic;			// 羊の絵のポインターへ
			pos.x = +(float)(x + BASE_X_L);
		}
		data[i].Set(pos, moveAngle, pointer,floor);
	}
}

bool End::Update()
{
	switch(state){
	case STATE::TEXT_POP:		TextPop();		break;
	case STATE::EXPOINT_DASH:	ExPointDash();	break;
	case STATE::RET_TRUE:		return true;
		
	}
	return false;
}

void End::TextPop()
{
	timeUpScale += SCALE_MAX/SCALE_TIME;
	if( timeUpScale > SCALE_MAX ){
		timeUpScale = SCALE_MAX;
		state = STATE::EXPOINT_DASH;
	}
}

void End::ExPointDash()
{
	for(int i=0; i<MAX_EX_POINT; i++){
		data[i].Update();
	}

	if( waitTimer++ >= WAIT_TIME ){
		state = STATE::RET_TRUE;
	}
}

void End::Render()
{
	for(int i=0; i<MAX_EX_POINT; i++){
		data[i].Render(floorNum);
	}
	timeUp->RenderCC(640, 360, (int)(512*timeUpScale),(int)(128*timeUpScale), 0,0,512,128, 0.1f);
}


//**********************************
//	ビックリマーク処理
//**********************************

namespace{
	const int MOVE_SPEED = 15;
	const int ANIM_FRAME = 2;
	const int ANIM_MAX = 5;
	const int RENDER_SIZE = 120;
	const int SPLIT_NUM = 4;
}

void End::ExPointData::Set(const Vector2 pos, const int moveAngle, tdn2DObj* obj, const int myFloor)
{
	this->obj = obj;
	this->pos = pos;
	this->moveAngle = moveAngle;
	this->myFloor = myFloor;
	animNum = rand()%ANIM_MAX;
	animFrame = rand()%ANIM_FRAME;
}

void End::ExPointData::Update()
{
	//座標移動 今は同じ方向に進む
	if( moveAngle == RIGHT ){
		pos.x += -MOVE_SPEED; //オオカミ 
	}else{
		pos.x += -MOVE_SPEED; // 羊
	}

	//アニメーション
	if( animFrame++ >= ANIM_FRAME ){
		animFrame = 0;
		if( ++animNum >= ANIM_MAX ){
			animNum = 0;
		}
	}
}

void End::ExPointData::Render(const int floor)
{
	if( floor < myFloor ) return;
	//obj->RenderUC((int)pos.x, (int)pos.y, RENDER_SIZE, RENDER_SIZE, animNum%SPLIT_NUM*RENDER_SIZE,animNum/SPLIT_NUM*RENDER_SIZE, RENDER_SIZE, RENDER_SIZE, .0f);
	obj->Render((int)pos.x, (int)pos.y, RENDER_SIZE, RENDER_SIZE, animNum%SPLIT_NUM*RENDER_SIZE, animNum / SPLIT_NUM*RENDER_SIZE, RENDER_SIZE, RENDER_SIZE);


}

void End::DataReceive(StageManager* stage)
{
	floorNum = 2;
}