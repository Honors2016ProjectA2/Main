#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneTitle.h"
#include	"SceneSelect.h"
#include	"../Sound/SoundManager.h"


//******************************************************************
//		�������E���
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
//		�X�V�E�`��
//******************************************************************
bool sceneTitle::Update()
{



	if (tdnInput::KeyGet(KEY_ENTER) == 3)
	{
		// �V�[���`�F���W
		extern Framework  *MainFrame;
		MainFrame->ChangeScene(new sceneSelect);
	}

	return true;
}

void sceneTitle::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	tdnText::Draw(150, 0, 0xffffffff, "�^�C�g��");
}
