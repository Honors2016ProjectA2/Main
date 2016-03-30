#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneTitle.h"
#include	"SceneSelect.h"
#include	"../Sound/SoundManager.h"
#include	"../IconButton/IconButton.h"

//******************************************************************
//		�������E���
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
//		�X�V�E�`��
//******************************************************************
bool sceneTitle::Update()
{
	tdnMouse::Update();

	Vector3 move;
	move.x = tdnInput::GetAxisX() * 0.0002f;
	move.z = -tdnInput::GetAxisY() * 0.0002f;
	move.y = 0;


	//button_mgr->Update(tdnMouse::GetPos());

	// ���N���b�N
	if (tdnMouse::GetLeft() == 3)
	{
		// �V�[���`�F���W
		MainFrame->ChangeScene(new sceneSelect);
	}

	return true;
}

void sceneTitle::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	//button_mgr->Render();

	tdnText::Draw(320, 280, 0xffffffff, "�^�C�g���V�[��");
	tdnText::Draw(320, 320, 0xffffffff, "�N���b�N�őI����ʂ�");
}
