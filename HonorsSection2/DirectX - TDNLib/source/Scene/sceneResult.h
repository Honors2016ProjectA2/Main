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

	//�������E���
	bool Initialize();
	~sceneResult();
	//����
	bool Update();
	//�`��
	void Render();
private:
	tdn2DObj* back = nullptr;


};