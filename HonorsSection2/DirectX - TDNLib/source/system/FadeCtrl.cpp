#include "TDNLIB.h"
#include	"FadeCtrl.h"

int	FadeControl::mode = 0;
float	FadeControl::count = 0.0f;
float	FadeControl::param = 0.0f;
int	FadeControl::stop = 255;
bool FadeControl::isFade = false;
bool FadeControl::isEndFade = false;

void FadeControl::Setting(int modeNum, float speed, float stopNum, float startNum)
{
	mode = modeNum;
	param = 255.0f/speed;
	
	switch( mode ){
	case FADE_IN:
	case WHITE_IN:
		count = 255.0f;
		stop = 0;
		break;
		
	case FADE_OUT:
	case WHITE_OUT:
		count = 0;
		stop = 255;
		break;
	}

	if( (int)stopNum != -1 )	stop = (int)(stopNum*255);
	if( (int)startNum != -1 )	count = startNum*255.0f;

	isFade = true;

}

void FadeControl::Update()
{
	isEndFade = false;
	if( !isFade ) return;


	switch( mode ){
		//	フェード＆ホワイト　イン
	case FADE_IN:
	case WHITE_IN:
		count -= param;
		if( count <= 0.0f || count < stop ){
			isFade = false;
			isEndFade = true;
			count = (float)stop;
		}
		break;

		//	フェード＆ホワイト　アウト
	case FADE_OUT:
	case WHITE_OUT:
		count += param;
		if( count >= 255.0f || count > stop ){
			isFade = false;
			isEndFade = true;
			count = (float)stop;
		}
		break;
	}
}

void FadeControl::Render()
{
	//	フェード処理
	DWORD	color;
	switch( mode ){
	case FADE_IN:
	case FADE_OUT:
		color = ARGB( (int)count, 0,0,0 );
		break;

	case WHITE_IN:
	case WHITE_OUT:
		color = ARGB( (int)count, 255,255,255 );
		break;
	}
	tdnPolygon::Rect( 0,0,1280,720, RS::COPY, color );
}
