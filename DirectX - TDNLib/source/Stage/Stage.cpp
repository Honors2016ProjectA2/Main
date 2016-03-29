#include "TDNLIB.h"
#include "Stage.h"
#include "AI\NPC\PersonManager.h"

//**************************************************************************************************
//		�ÓI�����o�ϐ�
//**************************************************************************************************
int Stage::m_StageNo = 0;

//**************************************************************************************************
//		�Z�b�^�[�ƃQ�b�^�[
//**************************************************************************************************

int Stage::LoadPerson()
{
	char filename[128];
	sprintf_s(filename, 128, "DATA/Text/Stage/stage%d.txt", m_StageNo);
	std::ifstream ifs(filename);

	// �����ň�������������X�e�[�W�̔ԍ��������������A�X�e�[�W�̃e�L�X�g�����݂��Ȃ�
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