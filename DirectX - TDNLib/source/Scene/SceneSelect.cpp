#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneSelect.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"


//******************************************************************
//		�������E���
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
//		�X�V�E�`��
//******************************************************************
bool sceneSelect::Update()
{

	if (tdnInput::KeyGet(KEY_ENTER) == 3)
	{
		// �V�[���`�F���W
		extern Framework  *MainFrame;
		MainFrame->ChangeScene(new sceneMain);
	}

	return true;	
}

void sceneSelect::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	tdnText::Draw(150, 0, 0xffffffff, "�X�e�[�W�Z���N�g");
}
