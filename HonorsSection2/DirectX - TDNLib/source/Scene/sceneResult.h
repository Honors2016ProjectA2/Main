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

	//‰Šú‰»E‰ğ•ú
	bool Initialize();
	~sceneResult();
	//ˆ—
	bool Update();
	//•`‰æ
	void Render();
private:
	tdn2DObj* back = nullptr;


};