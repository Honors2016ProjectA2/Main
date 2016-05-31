#pragma once
#include	"IEX_Expansion.h"

class Ready
{
public:
	Ready();
	~Ready();
	void Init();
	bool Update();
	void Render();
private:
	//tdn2DObjEx* sinnRun = nullptr;
	//tdn2DObjEx* sinnJump = nullptr;
	//tdn2DObjEx* ready = nullptr;
	//tdn2DObjEx* go = nullptr;
	int state = 0;
	int x;
	int animNum = 0;
	int animFlame = 0;
	float scale = .0f;

	tdn2DObj* sinnRun = nullptr;
	tdn2DObj* sinnJump = nullptr;
	tdn2DObj* ready = nullptr;
	tdn2DObj* go = nullptr;

	//èàóù
	void ReadyRun();
	void RunAnimation();

	void GoBigger();
	void GoAnimation();

	//ï`âÊ
	void ReadyRender();
	void GoRender();
};