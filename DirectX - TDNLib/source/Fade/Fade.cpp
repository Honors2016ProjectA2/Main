#include	"TDNLIB.h"

#include	"Fade.h"

//*********************************************************************************************
//		パラメータの設定
//*********************************************************************************************
Fade::FLAG Fade::mode;		/*	フェードイン、フェードアウト、何もしない	*/
unsigned char Fade::speed;	/*	フェードする速度	*/
unsigned char Fade::limit;	/*	どこまでフェードするか(0～255の範囲)	*/
unsigned char Fade::alpha;	/*	α値(0～255)	*/
unsigned long Fade::color;	/*	カラー(0x○○000000 ～ 0x○○ffffff)ここの○○の部分を必ず00にしておくこと！	*/


//=============================================================================================
//		初	期	化
void Fade::Initialize()
{
	mode = FADE_NONE;
	speed = 1;
	alpha = 255;
	color = 0x00000000;
}
//
//=============================================================================================


//=============================================================================================
//		フェード設定
void Fade::Set(FLAG Mode, unsigned char Speed, unsigned long Color, unsigned char StartAlpha, unsigned char Limit)
{
	mode = Mode;		// フェードモード(FADE_IN or FADE_OUT)
	speed = Speed;		// フェードを増減させるスピード(5なら、毎フレーム「5」が増減される)
	limit = Limit;		// 目標alpha値(FADE_INなら「0」、FADE_OUTなら「255」が基本)
	alpha = StartAlpha;	// フェード開始時のalpha値(FADE_INなら「255」、DADE_OUTなら「0」が基本)
	color = Color;		// フェード色

	// 0x○○ffffffの○の部分を0にする処理(alpha値の調整)
	color &= 0x00ffffff;
}


void Fade::Set(FLAG Mode, unsigned char Speed, unsigned long Color)
{
	mode = Mode;
	speed = Speed;
	color = Color;

	switch (mode)
	{
	case FADE_IN:
		alpha = 255;
		limit = 0;
		break;

	case FADE_OUT:
		alpha = 0;
		limit = 255;
		break;

	case FADE_NONE:
		break;
	}
}

void Fade::Set(FLAG Mode, unsigned char Speed)	// alphaは0～255の範囲
{
	mode = Mode;
	speed = Speed;

	switch (mode)
	{
	case FADE_IN:
		limit = 0;
		break;

	case FADE_OUT:
		limit = 255;
		break;

	case FADE_NONE:
		break;
	}
}
//
//=============================================================================================



//=============================================================================================
//		更		新
void Fade::Update()
{
	switch (mode)
	{
	case FADE_IN:

		// 目的αまでついたら、ストップ
		if (alpha <= limit + speed){		// unsigned charなのでオーバーフロー防止
			alpha = limit;
			mode = FADE_NONE;
		}
		else
			alpha -= speed;

		break;

	case FADE_OUT:

		// 目的αまでついたら、ストップ
		if (alpha >= limit - speed){		// unsigned charなのでオーバーフロー防止
			alpha = limit;
			mode = FADE_NONE;
		}
		else
			alpha += speed;

		break;

	case FADE_NONE:
		break;	// NothingDo
	}
}


//=============================================================================================
//		描			画
void Fade::Render()
{
	const unsigned long c = (alpha << 24) | color;	// argbのフォーマットにする

	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, RS::COPY, c);
}
//
//=============================================================================================