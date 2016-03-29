#pragma once
#include "TDNLIB.h"
#include <iostream>
#include <math.h>
#include <string>
#include "AI\Entity\EntityEnum.h"

// ���b�Z�[�W�^�C�v
enum MESSAGE_TYPE
{
	RIPPLE_VS_PERSON,
	Riple_Red,
	B,
};

enum class PERSON_TYPE;	// �O���錾
// EX_INFO�p�̍\����
struct RIPPLE_INFO
{
	PERSON_TYPE type;
	Vector3 pos;
	float size;
};

/*
	���b�Z�[�W�̓V���v���ȗ񋓌^�B
	���e�͂Ȃ�ł�����B
	
	�E�N��
	�E�N��
	�E���b�Z�[�W�̗�
	�E���b�Z�[�W�̒ǉ����(Void*)
	�E�f�B���C�p
*/

/*�d��\����*/
struct Message
{
	// ����Telegram�𑗂����G���e�B�e�B
	ENTITY_ID          Sender;

	// ����Telegram���󂯎��G���e�B�e�B
	ENTITY_ID          Receiver;

	// ���b�Z�[�W�̓��e�B�@�i�񋓌^�ɐݒ肳��Ă�����́j
	int          Msg;

	// ���b�Z�[�W�̒ǉ���� (�L�����N�^�[�������� [�ǉ����] X�ɂQ�ړ� )
	// �ǉ��������O�ɒm�邱�Ƃ��s�\�Ȃ̂�Void*�Œ�
	void*        ExtraInfo;

	// ���b�Z�[�W�̒x������
	double       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::id_error),
		Receiver(ENTITY_ID::id_error),
		Msg(-1)
	{}

	Message(double time,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		int    msg,
		void*  info = NULL) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};