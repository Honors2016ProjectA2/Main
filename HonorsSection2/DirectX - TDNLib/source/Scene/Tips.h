#pragma once
#include "TDNLIB.h"


class Tips
{
public:
	Tips();
	~Tips();
	void Init();
	bool Update();
	void Render();

private:
	enum class STATE
	{
		START, EXECUTE, END
	};
	STATE m_state;

	// ƒqƒ“ƒg
	tdn2DObj* m_typs;


public:


};