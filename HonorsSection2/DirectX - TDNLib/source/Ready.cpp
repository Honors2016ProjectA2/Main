#include	"Ready.h"
#include	"Sound/SoundManager.h"

namespace{
	namespace STATE{
		enum{
			READY_RUN, GO_BIGGER, RET_TRUE
		};
	}

	namespace READY{
		const int START_X = 1280;
		const int Y = 360;


		namespace SINN{
			const int DEFAULT_SIZE = 120;
			const int SIZE_HALF = (int)(DEFAULT_SIZE/2*0.8);
			const int ANIM_MAX = 5;
		}

		namespace TEXT{
			const int W_HALF = 457/2;
			const int H = 128;
		}

		const int MOVE_SPEED = 10;

		const int END_X = -(SINN::SIZE_HALF*2 + TEXT::W_HALF*2 + SINN::SIZE_HALF*2);
	}

	namespace GO{
		namespace SINN{
			const int DEFAULT_SIZE = 240;
			const int SIZE_HALF = (int)(DEFAULT_SIZE/2*0.5);
			const int ADJUSTMENT = 4;
			const int ANIM_MAX = 4;
		}
		namespace TEXT{
			const int W_HALF = 194/2;
			const int H = 75;
			const int U = 512;
			const int V = 198;
		}

		const float SCALE_MAX = 10.0f;
		const int SCALE_TIME = 60;
		const float SCALE_SPEED = SCALE_MAX/SCALE_TIME;
	}
}

Ready::Ready()
{
	sinnRun = new tdn2DObjEx("DATA/CHR/sheep_animation.png");
	sinnJump = new tdn2DObjEx("DATA/CHR/fat_sheep.png");
	ready = new tdn2DObjEx("DATA/Ready/Ready.png");
	go = new tdn2DObjEx("DATA/Ready/GO.png");
}

Ready::~Ready()
{
	SAFE_DELETE(sinnRun);
	SAFE_DELETE(sinnJump);
	SAFE_DELETE(ready);
	SAFE_DELETE(go);
}

void Ready::Init()
{
	x = READY::START_X + READY::SINN::SIZE_HALF;
	scale = .0f;
	state = STATE::READY_RUN;

	//bgm->Play("READY");
}

bool Ready::Update()
{
	switch(state){
	case STATE::READY_RUN: ReadyRun();	break;
	case STATE::GO_BIGGER: GoBigger();	break;
	case STATE::RET_TRUE: return true;
	}
	return false;
}

void Ready::ReadyRun()
{
	RunAnimation();

	x -= READY::MOVE_SPEED;
	if( x <= READY::END_X ){
		state = STATE::GO_BIGGER;
		bgm->Stop("READY");
	}
}

void Ready::RunAnimation()
{
	animNum++;
	if (animNum >= READY::SINN::ANIM_MAX)
	{
		animNum = 0;
	}
}

void Ready::GoBigger()
{
	bgm->Play("GO");
	GoAnimation();

	scale += GO::SCALE_SPEED;
	if( scale >= GO::SCALE_MAX ){
		state = STATE::RET_TRUE;
		bgm->Stop("GO");
	}
}

void Ready::GoAnimation()
{
	animNum++;
	if( animNum >= GO::SINN::ANIM_MAX ){
		animNum = 0;
	}
}

void Ready::Render()
{
	switch(state){
	case STATE::READY_RUN: ReadyRender();	break;
	case STATE::GO_BIGGER: GoRender();	break;
	}
}

void Ready::ReadyRender()
{
	//前走る人
	int xPos = x;
	sinnRun->RenderUC(xPos, READY::Y, READY::SINN::SIZE_HALF*2, READY::SINN::SIZE_HALF*2, animNum%4*READY::SINN::DEFAULT_SIZE, animNum/4*READY::SINN::DEFAULT_SIZE,READY::SINN::DEFAULT_SIZE, READY::SINN::DEFAULT_SIZE, .0f);
	//Ready
	xPos += READY::SINN::SIZE_HALF+READY::TEXT::W_HALF;
	ready->RenderUC(xPos, READY::Y, READY::TEXT::W_HALF*2, READY::TEXT::H, 0,0,READY::TEXT::W_HALF*2, READY::TEXT::H, .0f);
	//後ろ走る人
	xPos += READY::SINN::SIZE_HALF+READY::TEXT::W_HALF;
	sinnRun->RenderUC(xPos, READY::Y, READY::SINN::SIZE_HALF*2, READY::SINN::SIZE_HALF*2, animNum%4*READY::SINN::DEFAULT_SIZE, animNum/4*READY::SINN::DEFAULT_SIZE,READY::SINN::DEFAULT_SIZE, READY::SINN::DEFAULT_SIZE, .0f);
}

void Ready::GoRender()
{
	//後ろの人
	sinnJump->RenderCC(1280/2,720/2+(int)(GO::SINN::ADJUSTMENT*scale), (int)(GO::SINN::SIZE_HALF*2*scale), (int)(GO::SINN::SIZE_HALF*2*scale), animNum%4*GO::SINN::DEFAULT_SIZE, animNum/4*GO::SINN::DEFAULT_SIZE, GO::SINN::DEFAULT_SIZE, GO::SINN::DEFAULT_SIZE, .0f);
	//手前の文字
	go->RenderCC(1280/2,720/2, (int)(GO::TEXT::W_HALF*2*scale), (int)(GO::TEXT::H*scale), 0,0, GO::TEXT::U, GO::TEXT::V, .0f);
}