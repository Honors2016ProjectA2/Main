#include "sceneResult.h"
#include "UI\ResultUIManager.h"
#include "PostEffect\PostEffect.h"

bool sceneResult::Initialize()
{
	tdnView::Init();
	back = new tdn2DObj("DATA/GameHaikei.png");

	// 
	RESULT_UIMNG;

	PostEffectMgr;
	return false;
}

sceneResult::~sceneResult()
{
	SAFE_DELETE(back);
	RESULT_UIMNG.Release();
	PostEffectMgr.Release();

}

bool sceneResult::Update()
{
	if (KeyBoard(KB_ENTER) == 3)
	{
		RESULT_UIMNG.Action();
	}
	if (KeyBoard(KB_SPACE) == 3)
	{
		RESULT_UIMNG.Stop();
	}

	RESULT_UIMNG.Update();

	tdnMouse::Update();

	return true;
}

void sceneResult::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	back->Render(0, 0);

	//PostEffectMgr.BloomBigin();
	//back->Render(0, 0);
	//PostEffectMgr.BloomEnd();

	RESULT_UIMNG.Render();
}
