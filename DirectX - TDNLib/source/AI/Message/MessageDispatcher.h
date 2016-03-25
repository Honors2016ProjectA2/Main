#pragma once
#include "Message.h"
#include <set>

/*
	Telegram�̍쐬�E���M�E�Ǘ��͂��̃N���X�ŏ�������B
	�N�������b�Z�[�W�𑗂�K�v������Ƃ��͂��̃C���X�^���X���g���B
	���b�Z�[�W�������ɑ��M����or���Ԃ�x�炷���߃L���[�ɓ���Ă����̓��

	���b�Z�[�W�𑗐M����O�ɂ��̃N���X�͑��M�҂̃G���e�B�e�B�̃|�C���^���K�v
	�C���X�^���X�����ꂽ�G�[�W�F���g�̃|�C���^���Q�Ƃł���}�l�[�W���[���~����
	(EntityManager)
*/

class BaseGameEntity;

// enum�I��
const double SEND_MSG_IMMEDIATELY = 0.0f;
const int   NO_ADDITIONAL_INFO = 0;

class MessageDispatcher
{
public:

	static MessageDispatcher* Instance();

	void DispatchMessage(double  delay,
		int    sender,
		int    receiver,
		int    msg,
		void*  ExtraInfo);

	void DispatchDelayedMessages();

private:
	// std::set�͒x�����b�Z�[�W�̃R���e�i�Ƃ��Ďg��
	// �����I�Ƀ\�[�g���d��������郁���b�g������B
	// ���b�Z�[�W���M�����Ń\�[�g�����
	std::set<Message> PriorityQ;// �D��

	// �f�B�X�p�b�`�x�����b�Z�[�W�ɂ���ė��p����܂�
	void Discharge(BaseGameEntity* pReceiver, const Message& msg);

	// ����
	MessageDispatcher() {};
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);
};

#define MsgMgr MessageDispatcher::Instance()