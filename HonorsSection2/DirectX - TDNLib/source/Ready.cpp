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

		const int MOVE_SPEED = 15;

		const int END_X = -900;
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

		const float SCALE_MAX = 4.0f;
		const int SCALE_TIME = 60;
		const float SCALE_SPEED = SCALE_MAX/SCALE_TIME;
	}
}

Ready::Ready()
{
	//sinnRun = new tdn2DObjEx("DATA/CHR/sheep_animation.png");
	//sinnJump = new tdn2DObjEx("DATA/CHR/fat_sheep.png");
	//ready = new tdn2DObjEx("DATA/Ready/Ready.png");
	//go = new tdn2DObjEx("DATA/Ready/GO.png");
	
	m_animeGO = new tdn2DAnim("DATA/Ready/GO.png");
	m_animeGO->OrderRipple(12, 2.0f, 0.1f);

	sinnRun = new tdn2DObj("DATA/CHR/sheep_animation.png");
	sinnJump = new tdn2DObj("DATA/CHR/fat_sheep.png");
	ready = new tdn2DObj("DATA/Ready/Ready.png");
	go = new tdn2DObj("DATA/Ready/GO.png");

	animFlame = 0;
	scale = 0.0f;

	sinnJump->SetScale(scale);
	go->SetScale(scale);
}

Ready::~Ready()
{
	SAFE_DELETE(sinnRun);
	SAFE_DELETE(sinnJump);
	SAFE_DELETE(ready);
	SAFE_DELETE(go);
	SAFE_DELETE(m_animeGO);
}

void Ready::Init()
{
	x = READY::START_X + READY::SINN::SIZE_HALF;
	scale = .0f;
	state = STATE::READY_RUN;
}

bool Ready::Update()
{
	// ステートによって関数を分ける
	switch(state){
	case STATE::READY_RUN: ReadyRun();	break;
	case STATE::GO_BIGGER: GoBigger();	break;
	case STATE::RET_TRUE: return true;
	}
	return false;
}

// Ready
void Ready::ReadyRun()
{
	RunAnimation();

	x -= READY::MOVE_SPEED;
	if( x <= READY::END_X ){
		state = STATE::GO_BIGGER;
		se->Stop("ドン", 0);
		m_animeGO->Action();
	}
}

void Ready::RunAnimation()
{
	animFlame++;
	if (animFlame >= 2)
	{
		animFlame = 0;

		animNum++;
		if (animNum >= READY::SINN::ANIM_MAX)
		{
			animNum = 0;
		}
	}

	}


// GO!
void Ready::GoBigger()
{
	se->Play("GO");
	GoAnimation();

	scale += GO::SCALE_SPEED;
	sinnJump->SetScale(scale);
	go->SetScale(scale);

	m_animeGO->Update();

	if( scale >= GO::SCALE_MAX ){
		state = STATE::RET_TRUE;
		//bgm->Stop("GO");
	}
}

void Ready::GoAnimation()
{
	animFlame++;
	if (animFlame >= 2)
	{
		animFlame = 0;

		animNum++;
		if (animNum >= GO::SINN::ANIM_MAX) {
			animNum = 0;
		}
	}
}

// 描画
void Ready::Render()
{
	switch(state){
	case STATE::READY_RUN: ReadyRender();	break;
	case STATE::GO_BIGGER: GoRender();	break;
	}
}



void Ready::ReadyRender()
{
	// スケール
	sinnRun->SetScale(0.8f);

	//前走る人
	int xPos = x;
	sinnRun->Render(xPos, READY::Y, 120, 120, (animNum % 4) * 120, (animNum / 4) * 120, 120, 120);
	sinnRun->Render(xPos + 120, READY::Y + 120 , 120, 120, (animNum % 4) * 120, (animNum / 4) * 120, 120, 120);
	sinnRun->Render(xPos + 120, READY::Y - 120 , 120, 120, (animNum % 4) * 120, (animNum / 4) * 120, 120, 120);

	//Ready
	xPos += 150;
	ready->Render(xPos, READY::Y);
	//後ろ走る人
	xPos += 450;
	sinnRun->Render(xPos, READY::Y, 120, 120, (animNum % 4) * 120, (animNum / 4) * 120, 120, 120);
	sinnRun->Render(xPos - 120, READY::Y + 120 , 120, 120, (animNum % 4) * 120, (animNum / 4) * 120, 120, 120);
	sinnRun->Render(xPos + 60, READY::Y + 120, 120, 120, (animNum % 4) * 120, (animNum / 4) * 120, 120, 120);



}

void Ready::GoRender()
{
	//後ろの人
	//sinnJump->Render(1280/2,720/2+(int)(GO::SINN::ADJUSTMENT*scale), (int)(GO::SINN::SIZE_HALF*2*scale), (int)(GO::SINN::SIZE_HALF*2*scale), animNum%4*GO::SINN::DEFAULT_SIZE, animNum/4*GO::SINN::DEFAULT_SIZE, GO::SINN::DEFAULT_SIZE, GO::SINN::DEFAULT_SIZE, .0f);
	//sinnJump->Render(500, 300, 240, 240, (animNum % 4) * 240, 0, 240, 240);

	//手前の文字
	//go->Render(1280/2,720/2, (int)(GO::TEXT::W_HALF*2*scale), (int)(GO::TEXT::H*scale), 0,0, GO::TEXT::U, GO::TEXT::V, .0f);
	//go->Render(400, 400);

	go->SetScale(2.0f);
	go->Render(400, 400);
	m_animeGO->Render(400, 400,RS::COPY);

}