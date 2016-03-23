#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"



//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneMain::Initialize()
{
	tdnView::Init();

	return true;
}

sceneMain::~sceneMain()
{
	SoundManager::Release();
}


//******************************************************************
//		更新・描画
//******************************************************************
bool sceneMain::Update()
{

	return true;	
}

void sceneMain::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	tdnText::Draw(150, 0, 0xfffffffff, "シーンメイン");

}
