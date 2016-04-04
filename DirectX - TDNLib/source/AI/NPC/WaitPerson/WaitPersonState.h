#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class WaitPerson;


/*
	���܂��Ă���l�̂̃X�e�[�g�}�V���̃J�[�g���b�W
*/


//-------------------�O���[�o�X�X�e�[�g
class WaitPersonGlobalState :public State<WaitPerson>
{
public:

	//this is a �V���O���g��
	static WaitPersonGlobalState* GetInstance();

	// ����
	virtual void Enter(WaitPerson* pPerson);

	// ���s���܂�
	virtual void Execute(WaitPerson* pPerson);

	// �A��
	virtual void Exit(WaitPerson* pPerson);

	// �`��
	virtual void Render(WaitPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	WaitPersonGlobalState() {};
	~WaitPersonGlobalState() {};

	WaitPersonGlobalState(const WaitPersonGlobalState&);
	WaitPersonGlobalState& operator=(const WaitPersonGlobalState&);
};



//--------------------�ŏ�

class StartWait :public State<WaitPerson>
{
public:

	//this is a �V���O���g��
	static StartWait* GetInstance();

	// ����
	virtual void Enter(WaitPerson* pPerson);

	// ���s���܂�
	virtual void Execute(WaitPerson* pPerson);

	// �A��
	virtual void Exit(WaitPerson* pPerson);

	// �`��
	virtual void Render(WaitPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	StartWait() {};
	~StartWait() {};

	StartWait(const StartWait&);
	StartWait& operator=(const StartWait&);
};


//--------------------�����Ă鎞�̑ҋ@
class ShedWait :public State<WaitPerson>
{
public:

	//this is a �V���O���g��
	static ShedWait* GetInstance();

	// ����
	virtual void Enter(WaitPerson* pPerson);

	// ���s���܂�
	virtual void Execute(WaitPerson* pPerson);

	// �A��
	virtual void Exit(WaitPerson* pPerson);

	// �`��
	virtual void Render(WaitPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	ShedWait() {};
	~ShedWait() {};

	ShedWait(const ShedWait&);
	ShedWait& operator=(const ShedWait&);
};


//--------------------�I��������̑ҋ@
class EndWait :public State<WaitPerson>
{
public:

	//this is a �V���O���g��
	static EndWait* GetInstance();

	// ����
	virtual void Enter(WaitPerson* pPerson);

	// ���s���܂�
	virtual void Execute(WaitPerson* pPerson);

	// �A��
	virtual void Exit(WaitPerson* pPerson);

	// �`��
	virtual void Render(WaitPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(WaitPerson* pPerson, const Message& msg);

private:
	EndWait() {};
	~EndWait() {};

	EndWait(const EndWait&);
	EndWait& operator=(const EndWait&);
};

