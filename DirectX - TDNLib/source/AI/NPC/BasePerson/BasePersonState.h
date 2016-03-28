#pragma once
#include "AI\State\StateMachine.h"
//#include "AI\Message\Message.h"
//#include "AI\Message\MessageDispatcher.h"

class BasePerson;

/*
	�l�̃X�e�[�g�}�V���̃J�[�g���b�W

*/


//-------------------�O���[�o�X�X�e�[�g
class PersonGlobalState :public State<BasePerson>
{
public:

	//this is a �V���O���g��
	static PersonGlobalState* GetInstance();

	// ����
	virtual void Enter(BasePerson* pPerson);

	// ���s���܂�
	virtual void Execute(BasePerson* pPerson);

	// �A��
	virtual void Exit(BasePerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonGlobalState() {};
	~PersonGlobalState() {};

	PersonGlobalState(const PersonGlobalState&);
	PersonGlobalState& operator=(const PersonGlobalState&);
};



//--------------------�ҋ@
class PersonWait:public State<BasePerson>
{
public:

	//this is a �V���O���g��
	static PersonWait* GetInstance();

	// ����
	virtual void Enter(BasePerson* pPerson);

	// ���s���܂�
	virtual void Execute(BasePerson* pPerson);

	// �A��
	virtual void Exit(BasePerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonWait() {};
	~PersonWait() {};

	PersonWait(const PersonWait&);
	PersonWait& operator=(const PersonWait&);
};


//--------------------�����Ă鎞�̑ҋ@
class PersonShedWait :public State<BasePerson>
{
public:

	//this is a �V���O���g��
	static PersonShedWait* GetInstance();

	// ����
	virtual void Enter(BasePerson* pPerson);

	// ���s���܂�
	virtual void Execute(BasePerson* pPerson);

	// �A��
	virtual void Exit(BasePerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonShedWait() {};
	~PersonShedWait() {};

	PersonShedWait(const PersonShedWait&);
	PersonShedWait& operator=(const PersonShedWait&);
};


//--------------------�I��������̑ҋ@
class PersonEndWait :public State<BasePerson>
{
public:

	//this is a �V���O���g��
	static PersonEndWait* GetInstance();

	// ����
	virtual void Enter(BasePerson* pPerson);

	// ���s���܂�
	virtual void Execute(BasePerson* pPerson);

	// �A��
	virtual void Exit(BasePerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(BasePerson* pPerson, const Message& msg);

private:
	PersonEndWait() {};
	~PersonEndWait() {};

	PersonEndWait(const PersonEndWait&);
	PersonEndWait& operator=(const PersonEndWait&);
};

