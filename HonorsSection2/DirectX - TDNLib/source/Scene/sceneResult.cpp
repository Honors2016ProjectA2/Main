#include "sceneResult.h"
#include "UI\ResultUIManager.h"
#include "PostEffect\PostEffect.h"
#include "Effect\EffectManager.h"

bool sceneResult::Initialize()
{
	tdnView::Init();
	back = new tdn2DObj("DATA/GameHaikei.png");

	// 
	RESULT_UIMNG;

	PostEffectMgr;
	EffectMgr;

	return false;
}

sceneResult::~sceneResult()
{
	SAFE_DELETE(back);
	RESULT_UIMNG.Release();
	PostEffectMgr.Release();
	EffectMgr.Release();

}

bool sceneResult::Update()
{
	if (KeyBoard(KB_ENTER) == 3)
	{
		RESULT_UIMNG.Action();
		//EffectMgr.AddEffect(500, 500, EFFECT_TYPE::PLUS);
	}
	if (KeyBoard(KB_SPACE) == 3)
	{
		RESULT_UIMNG.Stop();
	}

	RESULT_UIMNG.Update();

	EffectMgr.Update();

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

	EffectMgr.Render();

}
