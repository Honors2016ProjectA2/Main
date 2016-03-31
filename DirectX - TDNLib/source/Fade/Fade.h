#ifndef FADE_H_
#define	FADE_H_

//=======================================================================
//						フェードクラス
//=======================================================================


class Fade
{
public:
	//------------------------------------------------------
	//	定数
	//------------------------------------------------------
	enum FLAG{ FADE_NONE, FADE_IN, FADE_OUT, MAX };

	//------------------------------------------------------
	//	基本パラメータ
	//------------------------------------------------------
	static FLAG mode;				/*	フェードイン、フェードアウト、何もしない	*/
	static unsigned char speed;		/*	フェードする速度							*/
	static unsigned char limit;		/*	どこまでフェードするか(0～255の範囲)		*/
	static unsigned char alpha;		/*	α値(0～255)								*/
	static unsigned long color;		/*	カラー(0x00000000 ～ 0x00ffffff)			*/

	//void None();
	//void In();
	//void Out();
	//static void(Fade::*Fade_mode_funk[FLAG::MAX])();

	//------------------------------------------------------
	//	初期化
	//------------------------------------------------------
	static void Initialize();


	//------------------------------------------------------
	//	設定
	//------------------------------------------------------
	static void Set(FLAG Mode, unsigned char Speed, unsigned long Color, unsigned char StartAlpha, unsigned char Limit);		/*	フル設定版			*/
	static void Set(FLAG Mode, unsigned char Speed, unsigned long Color);														/*	色設定できる版		*/
	static void Set(FLAG Mode, unsigned char Speed);																			/*	簡易版				*/


	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	static void Update();
	static void Render();

	//------------------------------------------------------
	//	Get_Set
	//------------------------------------------------------
	static bool isFadeStop(){ return (mode == FADE_NONE); }
};

#endif
