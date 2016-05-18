#pragma once
//*****************************************************************************************************************************
//
//		フェードインフェードアウト
//
//*****************************************************************************************************************************

class	FadeControl
{
private:
	static int	mode;		//	モード
	static float	count;		//	モード用カウンタ
	static float	param;	//	モード用パラメータ
	static int	stop;		//	α限度設定用
	static bool	isFade;		//	フェード中かどうか
	static bool isEndFade;	//	フェード終了フレームかどうか
public:
	enum MODE{
		FADE_IN, WHITE_IN, FADE_OUT, WHITE_OUT
	};

	static void Setting( int mode, float speed, float stop = -1.0f, float start = -1.0f );
	static void Update();
	static void Render();
	static bool IsFade(){
		return isFade;
	}
	static bool IsEndFade(){
		return isEndFade;
	}
};