#include	"TDNLIB.h"

//#include	"system.h"
#include	"Framework.h"

//#include <time.h>


//*****************************************************************************************************************************
//
//		フレームワーク
//
//*****************************************************************************************************************************

//------------------------------------------------------
//		初期化・解放
//------------------------------------------------------
Framework::Framework( int FPSMode )
{
	this->FPSMode = FPSMode;

	scene = NULL;

	dwGameFrame = 0;
	dwFrameTime = clock();

	dwFPS = dwRenderFPS = 0;
	dwCurFrame = dwRCurFrame = 0;
}

Framework::~Framework()
{
	if( scene != NULL ) delete scene;
}


//*****************************************************************************************************************************
//		更新
//*****************************************************************************************************************************
bool Framework::Update()
{
	static 	DWORD	dwSec = 0;
	DWORD CurrentTime = clock()*10;

	//	フレーム制限
	if( CurrentTime < dwFrameTime+167 ) return false;

	//	経過時間
	DWORD	dTime = CurrentTime - dwFrameTime;
	if( dTime > 2000 ) dwFrameTime = CurrentTime;

	//	スキップタイプ 
	switch( FPSMode ){
	case FPS_60:	bRender = TRUE;	break;
	case FPS_30:	if( dwGameFrame & 0x01 ) bRender=TRUE; else bRender=FALSE;
					break;
	case FPS_FLEX:	if( dTime > 167*2 ) bRender = FALSE; else bRender = TRUE;
					break;
	}

	//	フレーム時間更新
	if( GetKeyState(VK_LCONTROL) < 0 ) dwFrameTime += 300;
	dwFrameTime += 167;

	//	秒間フレーム数保存
	if( dwSec < CurrentTime ){
		dwFPS       = dwCurFrame;
		dwRenderFPS = dwRCurFrame;
		dwCurFrame  = dwRCurFrame = 0;
		dwSec += 10000;
	}
	dwCurFrame ++;	//	フレーム数更新
	dwGameFrame++;	//	ゲームフレーム数更新

	tdnInput::Update();

	/*動画を再生していたら描画させない*/
	if (scene != NULL){
		movie_play = scene->Update();
		return movie_play;
	}

	return true;
}

//*****************************************************************************************************************************
//		描画
//*****************************************************************************************************************************
void Framework::Render()
{
	if( !bRender ) return;
	//	シーン開始
	tdnSystem::BeginScene(); 
	//	シーン描画
	if( scene != NULL ) scene->Render();

	
#ifdef _DEBUG
	//	フレーム表示
	tdnText::Draw(0, 0, 0xff00ffff, "FPS %d/%d", dwFPS, dwRenderFPS);
#endif


	// シーン終了
	tdnSystem::EndScene();

	dwRCurFrame ++;	//	描画フレーム数更新
}
