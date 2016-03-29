#include "TDNLIB.h"
#include "Stage.h"
#include "AI\NPC\PersonManager.h"

//**************************************************************************************************
//		静的メンバ変数
//**************************************************************************************************
int Stage::m_StageNo = 0;

//**************************************************************************************************
//		セッターとゲッター
//**************************************************************************************************

int Stage::LoadPerson()
{
	char filename[128];
	sprintf_s(filename, 128, "DATA/Text/Stage/stage%d.txt", m_StageNo);
	std::ifstream ifs(filename);

	// ここで引っかかったらステージの番号がおかしいか、ステージのテキストが存在しない
	assert(ifs);

	char skip[16];
	int RippleCount;

	ifs >> skip;
	ifs >> RippleCount;

	while (!ifs.eof())
	{
		ifs >> skip;
		ifs >> skip;

		int type;
		Vector3 pos;
		ifs >> type;
		ifs >> pos.x;
		ifs >> pos.y;
		ifs >> pos.z;
		PersonMgr.AddPerson((PERSON_TYPE)type, pos);
	}

	return RippleCount;
}