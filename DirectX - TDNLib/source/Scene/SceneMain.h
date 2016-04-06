#pragma once

#include "../AI/State/StateMachine.h"
#include	"../system/System.h"
#include	"../system/Framework.h"

class IconButtonManager;

class sceneMain : public BaseScene
{
public:
	//	初期化・解放
	bool Initialize();
	~sceneMain();
	//	更新・描画
	bool Update();
	void Render();

	// ステートマシンのアクセサ
	StateMachine<sceneMain>* GetFSM()const { return m_pStateMachine; }

	// ボタン管理してる人のアクセサ
	IconButtonManager *GetButtonMgr(){ return m_pButtonMgr; }

private:
	// ★ステートマシン
	StateMachine<sceneMain>*  m_pStateMachine;
	IconButtonManager *m_pButtonMgr;
};


