#pragma once
#pragma once

#include	"system/BaseScene.h"
#include	"IEX_Expansion.h"
class Ready;
class End;
class MousePointer;
class StageManager;
class DataManager;
class SheepManager;
class Result;


class sceneResult: public BaseScene
{
public:

	//初期化・解放
	bool Initialize();
	~sceneResult();
	//処理
	bool Update();
	//描画
	void Render();
private:
	tdn2DObj* back = nullptr;


};