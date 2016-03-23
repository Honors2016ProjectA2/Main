#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneSelect.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"


//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneSelect::Initialize()
{
	tdnView::Init();

	return true;
}

sceneSelect::~sceneSelect()
{

}


//******************************************************************
//		更新・描画
//******************************************************************
bool sceneSelect::Update()
{

	if (tdnInput::KeyGet(KEY_ENTER) == 3)
	{
		// シーンチェンジ
		extern Framework  *MainFrame;
		MainFrame->ChangeScene(new sceneMain);
	}

	return true;	
}

void sceneSelect::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	tdnText::Draw(150, 0, 0xffffffff, "ステージセレクト");
}
