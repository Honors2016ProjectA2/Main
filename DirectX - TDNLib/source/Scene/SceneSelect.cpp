#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneSelect.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"
#include	"../IconButton/IconButton.h"
#include	"../Stage/Stage.h"
//******************************************************************
//		�������E���
//******************************************************************
bool sceneSelect::Initialize()
{
	tdnView::Init();
	tdnMouse::Initialize(TRUE);
	m_pIconMgr = new IconButtonManager;
	m_pIconMgr->TextLoad("DATA/Text/IconButton/select.txt");

	return true;
}

sceneSelect::~sceneSelect()
{
	delete m_pIconMgr;
}


//******************************************************************
//		�X�V�E�`��
//******************************************************************
bool sceneSelect::Update()
{
	tdnMouse::Update();
	m_pIconMgr->Update(tdnMouse::GetPos());

	if (tdnMouse::GetLeft() == 3)
	{
		int SelectStageNo = m_pIconMgr->GetInButtonNo();
		if (SelectStageNo != m_pIconMgr->NOT_IN_BUTTON)
		{
			// �����Ől�Ԃ̓ǂݍ��݂̐������ǂ��ɂ�
			Stage::SetStageNo(SelectStageNo);

			// �V�[���`�F���W
			MainFrame->ChangeScene(new sceneMain);
		}
	}

	return true;	
}

void sceneSelect::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	m_pIconMgr->Render();
	tdnText::Draw(0, 30, 0xff00ffff, "[%.1f] [%.1f]", tdnMouse::GetPos().x, tdnMouse::GetPos().y);
}
