#include "../system/Framework.h"
#include "Explain.h"
#include "../system/FadeCtrl.h"
#include "../Sound/SoundManager.h"
#include "../MousePointer.h"
#include "SceneMain.h"

namespace{
	namespace STATE{
		enum{
			FADE_START, TEXT1, TEXT2, FADE_END, RET_TRUE
		};
	}
	const float FADE_ALPHA = 0.5f;

	const int SIZE_X = 1197;
	const int SIZE_Y = 663;
	const float SCALE_CHANGE = 1.0f/15.0f;

	namespace NEXT_ICON{
		const int X = 1206;
		const int Y = 631;
		const int SIZE_X = 61;
		const int SIZE_Y = 103;
		const float SCALE_UP_SIZE = 0.5f;
		const float SCALE_UP_FRAME = 0.1f;
	}
}


Explain::~Explain()
{
	SAFE_DELETE(game.obj);
	SAFE_DELETE(howTo.obj);
	SAFE_DELETE(nextIcon[0]);
	SAFE_DELETE(nextIcon[1]);
	delete pointer;
}

bool Explain::Initialize()
{
	state = STATE::FADE_START;
	game.Init();
	howTo.Init();

	game.obj = new tdn2DObjEx("DATA/Explain/GameSetumei.png");
	howTo.obj = new tdn2DObjEx("DATA/Explain/GameSyoukai.png");
	nextIcon[0] = new tdn2DObjEx("DATA/Explain/nextIcon01.png");
	nextIcon[1] = new tdn2DObjEx("DATA/Explain/nextIcon02.png");

	pointer = new MousePointer();

	return true;
}

bool Explain::Update()
{
	FadeControl::Update();
	//tdnMouse::Update();

	switch(state){
	case STATE::FADE_START:	FadeStart();	break;
	case STATE::TEXT1:		Text1();		break;
	case STATE::TEXT2:		Text2();		break;
	case STATE::FADE_END:	FadeEnd();		break;
	case STATE::RET_TRUE:	return true;
	}

	//MainFrame->ChangeScene(new sceneMain);
	return false;
}

void Explain::FadeStart()
{
	if( FadeControl::IsFade() ){
		return;
	}
	if( FadeControl::IsEndFade() ){
		howTo.openFlg = true;
		state = STATE::TEXT1;
		return;
	}
	FadeControl::Setting(FadeControl::MODE::FADE_OUT, 30.0f, FADE_ALPHA);
	
}

void Explain::Text1()
{
	ScaleMove();
	howTo.Open();
	if( tdnMouse::GetLeft() == 3 ){
		howTo.openFlg = false;
		howTo.closeFlg = true;
		se->Play("CLICK");
	}

	if( howTo.Close() ){
		game.openFlg = true;
		state = STATE::TEXT2;
	}

}

void Explain::Text2()
{
	ScaleMove();
	game.Open();
	if( tdnMouse::GetLeft() == 3 ){
		game.openFlg = false;
		game.closeFlg = true;
		se->Play("CLICK");
	}

	if( game.Close() ){
		//FadeControl::Setting(FadeControl::MODE::FADE_IN, 30.0f, .0f, FADE_ALPHA);
		state = STATE::FADE_END;
		bgm->Fade_out("TITLE", .025f);
	}
}

// ‚©‚«‚©‚¦‚½‚Á‚½
void Explain::FadeEnd()
{
	//if( FadeControl::IsEndFade() )
	{
		state = STATE::RET_TRUE;
		//MainFrame->ChangeScene(new sceneMain);
	}
}

void Explain::ScaleMove()
{
	scaleTheta += NEXT_ICON::SCALE_UP_FRAME;
	if( scaleTheta > PI*2.0f ){
		scaleTheta -= PI*2.0f;
	}
}

void Explain::Picture::Init()
{
	scale = .0f;
	openFlg = false;
	closeFlg = false;
}

bool Explain::Picture::Open()
{
	if( !openFlg ) return false;

	scale += SCALE_CHANGE;
	if( scale >= 1.0f ){
		scale = 1.0f;
		return true;
	}
	return false;
}

bool Explain::Picture::Close()
{
	if( !closeFlg ) return false;

	scale -= SCALE_CHANGE;
	if( scale <= .0f ){
		scale = .0f;
		return true;
	}
	return false;
}

void Explain::Picture::Render()
{
	obj->RenderCC(1280/2,720/2,(int)(SIZE_X*scale), SIZE_Y, 0,0,SIZE_X, SIZE_Y, .0f);
}

void Explain::Render()
{
	FadeControl::Render();

	switch(state){
	case STATE::TEXT1:
		howTo.Render();
		break;
	case STATE::TEXT2:
		game.Render();
		break;
	}
	float scale = 1.0f;
	int alpha = (int)(((sinf(scaleTheta)+1.0f)/2.0f)*255);
	nextIcon[0]->RenderCC(NEXT_ICON::X, NEXT_ICON::Y, (int)(NEXT_ICON::SIZE_X*scale), (int)(NEXT_ICON::SIZE_Y*scale), 0,0, NEXT_ICON::SIZE_X, NEXT_ICON::SIZE_Y,.0f);
	nextIcon[1]->RenderCC(NEXT_ICON::X, NEXT_ICON::Y, (int)(NEXT_ICON::SIZE_X*scale), (int)(NEXT_ICON::SIZE_Y*scale), 0,0, NEXT_ICON::SIZE_X, NEXT_ICON::SIZE_Y,.0f, NULL, ARGB(alpha,255,255,255));

	pointer->Render();
}