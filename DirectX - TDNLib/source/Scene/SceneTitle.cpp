#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneTitle.h"
#include	"SceneSelect.h"
#include	"../Sound/SoundManager.h"


//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneTitle::Initialize()
{
	tdnView::Init();

	return true;
}

sceneTitle::~sceneTitle()
{

}


//******************************************************************
//		更新・描画
//******************************************************************
bool sceneTitle::Update()
{



	if (tdnInput::KeyGet(KEY_ENTER) == 3)
	{
		// シーンチェンジ
		extern Framework  *MainFrame;
		MainFrame->ChangeScene(new sceneSelect);
	}

	return true;
}

void sceneTitle::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	tdnText::Draw(150, 0, 0xffffffff, "タイトル");
}
