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

	void TipsSelect();  // 状況に合わせて絵を変える

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
	// ヒント
	tdn2DAnim* m_typs[TIPS_TYPE::END];
	int m_selectTips;

public:
	


};