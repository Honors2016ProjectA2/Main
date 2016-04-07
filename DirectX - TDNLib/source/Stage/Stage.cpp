#include "TDNLIB.h"
#include "Stage.h"
#include "AI\NPC\PersonManager.h"
#include "../JudgeManager/JudgeManager.h"

//**************************************************************************************************
//		���̎擾
//**************************************************************************************************
Stage &Stage::GetInstance()
{
	static Stage inst;
	return inst;
}

//**************************************************************************************************
//		�l�ԏ��ǂݍ���
//**************************************************************************************************
// �R�Â��p�\����(Map����) �֐����ŏ����ƃG���[
template<typename T>
struct MyMap
{
	char *str;				// ������ "GOAL_PERSON"
	T enum_flag;			// �萔	�@CLEAR_FLAG::GOAL_PERSON

	bool Check(char argstr[]){ return (strcmp(str, argstr) == 0); }

	// ���ڂ炵�悤�Ƃ������ǖ����ł���
	//static bool Search(MyMap Array[], char argstr[], T *out)
	//{
	//	// ���[�v���Ńe�L�X�g����Ƃ��Ă���������ɉ����Ē萔��ݒ�
	//	for (int i = 0; i < _countof(Array); i++)
	//	{
	//		if (Array[i].Check(str))
	//		{
	//			// �������v�����̂ŁA�萔��ݒ�
	//			*out = Array[i].enum_flag;
	//			return true;
	//		}
	//	}
	//	return false;	// ������Ȃ������I
	//}
};

int Stage::LoadPerson()
{
	char filename[128];

	// ���d�v�@�v���O������0����̃X�^�[�g�����A�e�L�X�g�ł̃X�e�[�W�̖��O��1����̃X�^�[�g�ŁA���̌덷�������Ŗ��߂Ă���I(+1)
	sprintf_s(filename, 128, "DATA/Text/Stage/stage%d.txt", m_StageNo + 1);

	std::ifstream ifs(filename);

	// �����ň�������������X�e�[�W�̔ԍ��������������A�X�e�[�W�̃e�L�X�g�����݂��Ȃ�
	assert(ifs);

	char skip[32];		// �ǂݔ�΂��p�ϐ�

	// �g��񐔓ǂݍ���
	int RippleCount;
	ifs >> skip;
	ifs >> RippleCount;

	// �N���A�����ǂݍ���
	char cClearFlag[16];
	ifs >> skip;
	ifs >> cClearFlag;

	/*
	������ƒ萔�̕R�Â������B�ȉ��̂悤�ȃ\�[�X�ɂȂ�Ȃ��悤�ɂ���

	if (strcmp(cClearFlag, "GOAL_PERSON") == 0)
	{
	m_ClearType = CLEAR_FLAG::GOAL_PERSON;
	}
	else if (strcmp(cClearFlag,"ALL_SHED")==0)
	{
	}
	*/

	// ��map�ŕ�����ƒ萔�̕R�Â� ��Release�ɂ���Ƃ��܂����삵�Ȃ������o��������̂ŁA��U�����͎g��Ȃ����j�ŁB�����Ȃ邯�ǎ����ō��
	//std::map<LPSTR, CLEAR_FLAG> ClearTypeList;
	//std::map<LPSTR, PERSON_TYPE> PersonTypeList;
	//
	//ClearTypeList["GOAL_PERSON"] = CLEAR_FLAG::GOAL_PERSON;
	//ClearTypeList["ALL_SHED"] = CLEAR_FLAG::ALL_SHED;
	//
	//PersonTypeList["WAIT"] = PERSON_TYPE::WAIT;
	//PersonTypeList["START"] = PERSON_TYPE::START;
	//PersonTypeList["GOAL"] = PERSON_TYPE::GOAL;


	MyMap<CLEAR_FLAG> ClearFlagList[]=
	{
		{ "GOAL_PERSON", CLEAR_FLAG::GOAL_PERSON },
		{ "ALL_SHED", CLEAR_FLAG::ALL_SHED },
		{ "DONT_SHED_PERSON", CLEAR_FLAG::DONT_SHED_PERSON }
	};

	MyMap<PERSON_TYPE> PersonTypeList[]=
	{
		{ "WAIT", PERSON_TYPE::WAIT },
		{ "START", PERSON_TYPE::START },
		{ "GOAL", PERSON_TYPE::GOAL }
	};

	// ���[�v���Ńe�L�X�g����Ƃ��Ă���������ɉ����Ē萔��ݒ�(�N���A����)
	BYTE ClearFlagBit(0x00);
	for (int i = 0; i < _countof(ClearFlagList); i++)
	{
		if (ClearFlagList[i].Check(cClearFlag))
		{
			// ���������v�����̂ŁA�N���A�����͂��ꂾ�I
			ClearFlagBit |= (BYTE)ClearFlagList[i].enum_flag;
			break;
		}
	}

	// ���킳�𗬂��Ă͂����Ȃ��l�Ԃ����邩
	char DontShed[4];
	ifs >> skip;
	ifs >> DontShed;
	
	if (strcmp(DontShed, "ON") == 0)
		ClearFlagBit |= (BYTE)CLEAR_FLAG::DONT_SHED_PERSON;	// �r�b�g���Z�ŗ����Ă͂����Ȃ��t���O��ݒ�(�����������ɗ����ȁB������������)

	while (!ifs.eof())
	{
	
		PERSON_TYPE type;		// �l�Ԃ̃^�C�v�̒萔
		char cPersonType[16];	// ������ł̐l�Ԃ̃^�C�v
		Vector3 pos;			// ���W

		// ������ł̒萔��ǂ�
		ifs >> cPersonType;

		// ���[�v���Ńe�L�X�g����Ƃ��Ă���������ɉ����Ē萔��ݒ�(�l�Ԃ̃^�C�v)
		for (int i = 0; i < _countof(PersonTypeList); i++)
		{
			if (PersonTypeList[i].Check(cPersonType))
			{
				// �������v�����̂ŁA�萔��ݒ�
				type = PersonTypeList[i].enum_flag;
				break;
			}
		}

		ifs >> pos.x;
		ifs >> pos.y;
		ifs >> pos.z;

		// �l�ԃ��X�g�ɒǉ�
		PersonMgr.AddPerson(type, pos);
	}

	// ���W���b�W�N�ɃN���A������ݒ�(���̈�ԉ��ɏ����̂��Ƃ����ƁA�ݒ肵�����_�Ől�Ԃ̐ݒu�����Q�Ƃ���ꍇ�����邩��)
	JudgeMgr.SetClearFlag(ClearFlagBit);

	return RippleCount;
}