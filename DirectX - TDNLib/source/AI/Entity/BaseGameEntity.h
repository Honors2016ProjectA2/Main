#pragma once
#include "../Message/Message.h"


/*
*	���̐��E�̏Z�l�͑S�Ă��̃N���X����h�����܂��B
*	�����ID�ԍ���U����N���X�ł��B
*	
*
*/

class BaseGameEntity
{


private:

	// �ǂ̃G���e�B�e�B����ӂ�ID�ԍ�������
	int m_ID;

	// ����͎��̗L����ID�B���̃N���X�����������x�Ɏ����I�ɂ��̒l�͍X�V�����
	static int m_isNextValidID;

	// ID���������ݒ肳�ꂽ�̂����m�F���邽�߂ɃR���X�g���N�^�̒��ŌĂ΂��
	// ID��ݒ肵���̗L����ID���C���N�������g����O�ɁA����ɓn���ꂽ�l�����̗L����ID�ȏ�ł��邩�m�F����B
	void SetID(int val);

public:
	BaseGameEntity(int id) { SetID(id); };
	virtual ~BaseGameEntity() {};

	// �S�ẴG���e�B�e�B��Update�������p���悤�������z�֐���
	virtual void Update() = 0;

	/*
		���b�Z�[�W���쐬�Ɣz�M������
		�ǂ̃N���X�ł����b�Z�[�W���g����p��
		HandleMessage��ǉ�����
	*/
	// �S�ẴT�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	virtual bool  HandleMessage(const Message& msg) = 0;

	// �A�N�Z�T�[
	int GetID() { return m_ID; };

};

