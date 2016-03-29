#include "TDNLIB.h"
#include "PersonLoader.h"
#include "AI\NPC\PersonManager.h"

//**************************************************************************************************
//		�Z�b�^�[�ƃQ�b�^�[
//**************************************************************************************************

void PersonLoader::LoadPerson(int StageNo)
{
	char filename[128];
	sprintf_s(filename, 128, "DATA/Text/Stage/stage%d.txt", StageNo);
	std::ifstream ifs(filename);

	// �����ň�������������X�e�[�W�̔ԍ��������������A�X�e�[�W�̃e�L�X�g�����݂��Ȃ�
	assert(ifs);

	while (!ifs.eof())
	{
		int type;
		Vector3 pos;
		ifs >> type;
		ifs >> pos.x;
		ifs >> pos.y;
		ifs >> pos.z;
		PersonMgr.AddPerson((PERSON_TYPE)type, pos);
	}
}