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

	void TipsSelect();  // ó‹µ‚É‡‚í‚¹‚ÄŠG‚ğ•Ï‚¦‚é

private:
	enum class STATE
	{
		START, EXECUTE, END
	};
	STATE m_state;


	enum  TIPS_TYPE
	{
		MEAT, MEAT_TYPE, HOUSE,KUSA_GAGE, END
	};
	// ƒqƒ“ƒg
	tdn2DAnim* m_typs[TIPS_TYPE::END];
	int m_selectTips;

public:
	


};