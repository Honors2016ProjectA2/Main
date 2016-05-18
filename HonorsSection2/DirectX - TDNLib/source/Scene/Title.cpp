#include "../system/Framework.h"
#include "Title.h"
#include "Explain.h"
#include "../system/FadeCtrl.h"
#include "../Sound/SoundManager.h"

namespace{
	namespace STATE{
		enum{
			SELECT_WAIT, START, EXIT, RET_TRUE, EXIT_END,
		};
	}


	namespace LOGO{
		const int X = 645;
		const int Y = 204;
		const int W = 500;
		const int H = 301;
	}
	namespace START{
		const int X = 320;
		const int Y = 640;
		const int W = 462;
		const int H = 62;
	}
	namespace END{
		const int X = 944;
		const int Y = 641;
		const int W = 464;
		const int H = 61;
	}

	const float SELECT_SCALE = 1.2f;
	const float SCALE_SPEED = (SELECT_SCALE-1.0f)/10.0f;

	namespace BOX_ID{
		enum{
			START, END
		};
	}

	const float SHAKE_SPEED = 0.5f;
	const float SHAKE_WIDHT = 0.02f;

	const float ALPHA_SPEED = 1.0f;

	const int EX_POINT_WAIT_TIME = 15;
	const int GORILLA_WAIT_TIME = 70;

	namespace SINN{
		const int START_X = 466;
		const int START_Y = 190;
		const float START_ROLL = -15.0f/180.0f*PI;
		const int START_ANIM = 2;
		const int MAX_ANIM = 8;
		const int ANIM_FRAME = 1;
		const int MOVE_SPEED = 10;

		const int MAX_ANIM_DEATH = 12;
		const int ANIM_FRAME_DEATH = 3;
	}

	namespace EX_POINT{
		const int START_X = 640;
		const int START_Y = 150;
		const float SCALE = 1.6f;
		const int START_ANIM = 0;
		const int MAX_ANIM = 10;
		const int ANIM_FRAME = 3;
		const int MOVE_SPEED = 13;
	}
	const int SPLIT_NUM = 4;
	const int SPLIT_GORI = 2;

	const int RENDER_SIZE = 128;

	namespace GORILLA{
		const int START_X = 1280+128;
		const int START_Y = 296;
		const int START_ANIM = 0;
		const int MAX_ANIM = 4;
		const int ANIM_FRAME = 5;
		const int MOVE_SPEED = 7;
	}

	//しんにょうとゴリラの衝突判定（しんにょうの半分+ゴリラの半分
	const int COLLID_LEN = RENDER_SIZE/2 + RENDER_SIZE;

	const int EXIT_WAIT_TIME = 60;
}

//*************************************************************************************************
//
//		初期化・解放
//
//*************************************************************************************************

Title::~Title()
{
	SAFE_DELETE(logo.obj);
	SAFE_DELETE(start.obj);
	SAFE_DELETE(end.obj);
	SAFE_DELETE(sinn.normal);
	SAFE_DELETE(sinn.change);
	SAFE_DELETE(sinn.death);
	SAFE_DELETE(exPoint.obj);
	SAFE_DELETE(gorilla.obj);
}

bool Title::Initialize()
{
	state = STATE::SELECT_WAIT;

	alphaTheta = shakeTheta = .0f;
	start.scale = end.scale = .0f;
	sinn.Init(SINN::START_X, SINN::START_Y, SINN::START_ROLL, SINN::START_ANIM);
	exPoint.Init(EX_POINT::START_X, EX_POINT::START_Y, .0f, EX_POINT::START_ANIM);
	gorilla.Init(GORILLA::START_X, GORILLA::START_Y, .0f, GORILLA::START_ANIM);
	timer = 0;

	logo.Init("DATA/TITLE/TitleLogo.png");
	start.Init("DATA/TITLE/GameStart.png");
	end.Init("DATA/TITLE/GameEnd.png");
	sinn.normal = new tdn2DObjEx("DATA/CHR/sinnnyou_aruki.png");

	// 本来、やられの絵があるのですが、ここでは同じ絵を使いまわしてます
	sinn.change = new tdn2DObjEx("DATA/CHR/sinnnyou_aruki.png");
	sinn.death = new tdn2DObjEx("DATA/CHR/sinnnyou hajike.png");
	sinn.obj = sinn.normal;
	exPoint.obj = new tdn2DObjEx("DATA/CHR/「！」左移動.png");
	exPoint.roll = .0f;
	gorilla.obj = new tdn2DObjEx("DATA/CHR/!-kai.png");


	box[BOX_ID::START].Set(START::X, START::Y, START::W, START::H);
	box[BOX_ID::END].Set(END::X, END::Y, END::W, END::H);

	sinn.SetAnimData(SINN::ANIM_FRAME, SINN::MAX_ANIM);
	sinn.moveSpeed = SINN::MOVE_SPEED;
	sinn.landHeight = 720 / 2;
	exPoint.SetAnimData(EX_POINT::ANIM_FRAME, EX_POINT::MAX_ANIM);
	exPoint.moveSpeed = EX_POINT::MOVE_SPEED;
	exPoint.landHeight = 720 / 2 - (int)(128 / 2 * 0.6f);
	gorilla.SetAnimData(GORILLA::ANIM_FRAME, GORILLA::MAX_ANIM);
	gorilla.moveSpeed = GORILLA::MOVE_SPEED;

	FadeControl::Setting(FadeControl::MODE::FADE_IN, 30.0f);

	bgm->Play("TITLE");
	//bgm->Play("MAIN");

	return true;
}

//*************************************************************************************************
//
//		処理
//
//*************************************************************************************************

bool Title::Update()
{
	FadeControl::Update();
	tdnMouse::Update();

	switch (state){
	case STATE::SELECT_WAIT:	SelectWait();	break;
	case STATE::START:			Start();		break;
	case STATE::EXIT:			Exit();			break;
	case STATE::RET_TRUE:		return true;
	case STATE::EXIT_END:		ExitEnd();		break;
	}

	//MainFrame->ChangeScene(new Explain);
	return true;
}

//*************************************************************************************************
//
//		描画
//
//*************************************************************************************************

void Title::Render()
{
	tdnView::Clear(0xffffffff);
	tdnView::Activate();
	FadeControl::Render();

	logo.Render(LOGO::X, LOGO::Y, LOGO::W, LOGO::H, shakeTheta, alphaTheta);
	start.Render(START::X, START::Y, START::W, START::H, shakeTheta, alphaTheta);
	end.Render(END::X, END::Y, END::W, END::H, shakeTheta, alphaTheta);

	sinn.obj->RenderCC(sinn.x, sinn.y, (int)(RENDER_SIZE*sinn.scaleX), RENDER_SIZE, sinn.animNum%SPLIT_NUM*RENDER_SIZE, sinn.animNum / SPLIT_NUM*RENDER_SIZE, RENDER_SIZE, RENDER_SIZE, sinn.roll);
	exPoint.obj->RenderCC(exPoint.x, exPoint.y, (int)(RENDER_SIZE*EX_POINT::SCALE), (int)(RENDER_SIZE*EX_POINT::SCALE),
		exPoint.animNum%SPLIT_NUM*RENDER_SIZE, exPoint.animNum / SPLIT_NUM*RENDER_SIZE, RENDER_SIZE, RENDER_SIZE, .0f);
	int renderSize = RENDER_SIZE * 2;//ゴリラでかい
	gorilla.obj->RenderCC(gorilla.x, gorilla.y, renderSize, renderSize, gorilla.animNum%SPLIT_GORI*renderSize, gorilla.animNum / SPLIT_GORI*renderSize, renderSize, renderSize, .0f);
}





void Title::Object::Bigger()
{
	isSelected = true;

	scale += SCALE_SPEED;
	if( scale > SELECT_SCALE ){
		scale = SELECT_SCALE;
	}
}

void Title::Object::Smaller()
{
	isSelected = false;

	scale -= SCALE_SPEED;
	if( scale < 1.0f ){
		scale = 1.0f;
	}
}

void Title::Object::Init(char* fileName)
{
	obj = new tdn2DObjEx(fileName);
	scale = 1.0f;
	isSelected = false;
}

void Title::Object::Render(s32 DstX, s32 DstY, s32 DstW, s32 DstH, float shake, float alpha)
{
	float roll = .0f;
	float a = 1.0f;
	if( isSelected ){
		roll = SHAKE_WIDHT*sinf(shake);
		a = (cosf(alpha)+1.0f)/2.0f;
	}
	obj->RenderCC(DstX, DstY, (int)(DstW*scale), (int)(DstH*scale), 0,0, DstW, DstH, roll, NULL, ARGB((int)(a*255), 255,255,255));
}

void Title::SelectWait()
{
	ShakeUpdate();

	static bool canPlaySelectSE = true;

	if( box[BOX_ID::START].OnMouse() ){//スタート文字選択中
		start.Bigger();
		end.Smaller();
		if( canPlaySelectSE ){
			canPlaySelectSE = false;
			se->Play("CURSUR");
		}
	}else if( box[BOX_ID::END].OnMouse() ){//エンド文字選択中
		end.Bigger();
		start.Smaller();
		if( canPlaySelectSE ){
			canPlaySelectSE = false;
			se->Play("CURSUR");
		}
	}else{
		shakeTheta = .0f;
		end.Smaller();
		start.Smaller();
		canPlaySelectSE = true;
	}

	if( tdnMouse::GetLeft() == 3 ){
		if( start.isSelected ){
			state = STATE::START;
			sinn.SetSelect(STATE::START);
			exPoint.SetSelect(STATE::START);
			se->Play("CLICK");
		}
		if( end.isSelected ){
			state = STATE::EXIT;
			sinn.SetSelect(STATE::EXIT);
			exPoint.SetSelect(STATE::EXIT);
			se->Play("CLICK");
			bgm->Stop("TITLE");
		}
	}
}

void Title::Start()
{
	AlphaUpdate();

	sinn.Update();

	if( timer >= EX_POINT_WAIT_TIME ){
		exPoint.Update();
	}else{
		timer++;
	}

	if( exPoint.x <= -RENDER_SIZE/2 ){
		MainFrame->ChangeScene(new Explain);
	}
}

void Title::Exit()
{
	AlphaUpdate();
	sinn.Update();

	if( timer >= GORILLA_WAIT_TIME ){
		gorilla.Update();
	}else{
		timer++;
	}

	int diff = gorilla.x - sinn.x;
	if( diff*diff < COLLID_LEN*COLLID_LEN ){
		sinn.SetDeath();
	}

	if( sinn.endDeath ){
		timer = 0;
		state = STATE::EXIT_END;
	}
}

void Title::ExitEnd()
{
	AlphaUpdate();
	gorilla.Update();

	if( timer++ > EXIT_WAIT_TIME ){
		isEndGame = true; 
		PostQuitMessage(0);
	}
}

void Title::ShakeUpdate()
{ 
	shakeTheta += SHAKE_SPEED;
	if( shakeTheta > PI*2.0f ){
		shakeTheta -= PI*2.0f;
	}
}

void Title::AlphaUpdate()
{ 
	alphaTheta += ALPHA_SPEED;
	if( shakeTheta > PI*2.0f ){
		shakeTheta -= PI*2.0f;
	}
}

void Title::ClickBox::Set(const int x, const int y, const int width, const int height)
{
	this->x = x-width/2; this->y = y-height/2; this->width = width; this->height = height;
}

bool Title::ClickBox::OnMouse()
{
	Vector2 pPos = tdnMouse::GetPos();

	if( pPos.x < x || pPos.x > x+width )  return false;
	if( pPos.y < y || pPos.y > y+height ) return false;
	return true;
}


//*************************************************************************************************
//	演出
//*************************************************************************************************

namespace PERFORMANCE{
	enum{
		JUMP, RUN_START, REVERSE, RUN_EXIT, DEATH
	};
	const int UP_MOVE = 15;
	const int ADD_GRAVITY = 1;

	const float ROLL_ADJ = 0.01f;	//回転調整（この範囲無いなら0として扱う
}

//*************************************************************************************************
//	ベース及びビックリマーク
//*************************************************************************************************

void Title::Performance::Init(const int startX, const int startY, const float startRoll, const int startAnim)
{
	x = startX; y = startY; roll = startRoll; animNum = startAnim;
	state = 0; g = 0;
	animFrame = 0;
}

void Title::Performance::SetAnimData(const int animFrame, const int animMax)
{
	animChangeFrame = animFrame;
	this->animMax = animMax;
}

void Title::Performance::SetSelect(int select)
{
	titleSelect = select;
}

bool Title::Performance::Animation()
{
	//アニメーション
	if( animFrame++ >= animChangeFrame ){
		animFrame = 0;
		if( ++animNum >= animMax ){
			animNum = 0;
			return true;
		}
	}
	return false;
}

void Title::Performance::Update()
{
	switch(state){
	case PERFORMANCE::JUMP:			Jump();		break;
	case PERFORMANCE::RUN_START:	Run();		break;
	}
}

void Title::Performance::Jump()
{
	y += -PERFORMANCE::UP_MOVE + g;
	g += PERFORMANCE::ADD_GRAVITY;

	if( y >= landHeight ){
		y = landHeight;
		if( titleSelect == STATE::START ){
			state = PERFORMANCE::RUN_START;
		}else{
			state = PERFORMANCE::REVERSE;
		}
	}

	roll += (.0f - roll)/10;
	if( roll > -PERFORMANCE::ROLL_ADJ && roll < PERFORMANCE::ROLL_ADJ ){
		roll = .0f;
	}
}

void Title::Performance::Run()
{
	Animation();
	x -= moveSpeed;
}


//*************************************************************************************************
//	しんにょう
//*************************************************************************************************

namespace{
	const int REVERSE_TIME_HALF = 5;
}

void Title::PerformanceSinn::Init(const int startX, const int startY, const float startRoll, const int startAnim)
{
	Performance::Init(startX, startY, startRoll, startAnim);
	step = 0;
	scaleX = 1.0f;
	endDeath = false;
}

void Title::PerformanceSinn::Update()
{
	switch(state){
	case PERFORMANCE::JUMP:			Jump();		break;
	case PERFORMANCE::RUN_START:	Run();		break;
	case PERFORMANCE::REVERSE:		Reverse();	break;
	case PERFORMANCE::RUN_EXIT:		RunExit();	break;
	case PERFORMANCE::DEATH:		Death();	break;
	}
}

void Title::PerformanceSinn::Reverse()
{
	enum{
		SCALE_DOWN, CHANGE, SCALE_UP,
	};

	switch(step){
	case SCALE_DOWN:
		scaleX -= 1.0f/REVERSE_TIME_HALF;
		if( scaleX <= .0f ){
			scaleX = .0f;
			step = CHANGE;
		}
		break;
	case CHANGE:
		obj = change;
		step = SCALE_UP;
		//break;
	case SCALE_UP:
		scaleX += 1.0f/REVERSE_TIME_HALF;
		if( scaleX >= 1.0f ){
			scaleX = 1.0f;
			state = PERFORMANCE::RUN_EXIT;
		}
		break;

	}
}

void Title::PerformanceSinn::RunExit()
{
	Animation();
	x += moveSpeed;
}

void Title::PerformanceSinn::Death()
{
	if( Animation() ){
		animNum = animMax;
		endDeath = true;
	}
}

void Title::PerformanceSinn::SetDeath()
{
	if( state == PERFORMANCE::DEATH ) return;
	obj = death;
	SetAnimData(SINN::ANIM_FRAME_DEATH, SINN::MAX_ANIM_DEATH);
	animNum = 0;
	state = PERFORMANCE::DEATH;
	se->Play("DAMAGE");
}


//*************************************************************************************************
//	ゴリラ
//*************************************************************************************************

void Title::PerformanceGorilla::Update()
{
	Animation();
	Run();
}