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

	void TipsSelect();  // �󋵂ɍ��킹�ĊG��ς���

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
	// �q���g
	tdn2DAnim* m_typs[TIPS_TYPE::END];
	int m_selectTips;

public:
	


};