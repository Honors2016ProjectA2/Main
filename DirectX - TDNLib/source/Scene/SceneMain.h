#pragma once

#include "../AI/State/StateMachine.h"
#include	"../system/System.h"
#include	"../system/Framework.h"

class IconButtonManager;

class sceneMain : public BaseScene
{
public:
	//	�������E���
	bool Initialize();
	~sceneMain();
	//	�X�V�E�`��
	bool Update();
	void Render();

	enum class MODE
	{
		MAIN, GAMECLEAR, GAMEOVER
	};

	enum BUTTON_ID
	{
		YES = 0,
		NO = 1
	};

	void SetMode(MODE m){ m_mode = m; }

	// �X�e�[�g�}�V���̃A�N�Z�T
	StateMachine<sceneMain>* GetFSM()const { return m_pStateMachine; }

	// �{�^���Ǘ����Ă�l�̃A�N�Z�T
	IconButtonManager *GetButtonMgr(){ return m_pButtonMgr; }

private:
	// ���X�e�[�g�}�V��
	StateMachine<sceneMain>*  m_pStateMachine;
	IconButtonManager *m_pButtonMgr;
	MODE m_mode;
};


