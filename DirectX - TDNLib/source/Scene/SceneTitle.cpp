#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneTitle.h"
#include	"SceneSelect.h"
#include	"../Sound/SoundManager.h"

iex3DObj *airou;
Vector3 pos(0, 0, 0);

//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneTitle::Initialize()
{
	tdnView::Init();

	tdnMouse::Initialize(TRUE);

	airou = new iex3DObj("DATA/CHR/アイルー/airou_toire.IEM");
	airou->SetScale(1.0f);

	return true;
}

sceneTitle::~sceneTitle()
{
	delete airou;
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

	//	移動
	pos += move;

	tdnView::Set(pos, pos + Vector3(0, 0, 1));
	airou->Animation();
	airou->Update();

	// 左クリック
	if (tdnMouse::GetLeft() == 3)
	{
		se->Play("波紋出す", tdnMouse::GetPos());
	}

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

	airou->Render();

	tdnText::Draw(150, 0, 0xffffffff, "タイトル");
}
