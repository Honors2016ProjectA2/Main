#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneTitle.h"
#include	"SceneSelect.h"
#include	"../Sound/SoundManager.h"
#include	"../IconButton/IconButton.h"

//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneTitle::Initialize()
{
	tdnView::Init();

	tdnMouse::Initialize(TRUE);

	//button_mgr = new IconButtonManager;
	//button_mgr->TextLoad("DATA/Text/IconButton/title.txt");

	return true;
}

sceneTitle::~sceneTitle()
{
	//delete button_mgr;
}


//******************************************************************
//		更新・描画
//******************************************************************
bool sceneTitle::Update()
{
	tdnMouse::Update();

	Vector3 move;
	move.x = tdnInput::GetAxisX() * 0.0002f;
	move.z = -tdnInput::GetAxisY() * 0.0002f;
	move.y = 0;


	//button_mgr->Update(tdnMouse::GetPos());

	// 左クリック
	if (tdnMouse::GetLeft() == 3)
	{
		// シーンチェンジ
		MainFrame->ChangeScene(new sceneSelect);
	}

	return true;
}

void sceneTitle::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	//button_mgr->Render();

	tdnText::Draw(320, 280, 0xffffffff, "タイトルシーン");
	tdnText::Draw(320, 320, 0xffffffff, "クリックで選択画面へ");
}
