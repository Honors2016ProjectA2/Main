#pragma once
#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class StopPerson;


/*
	�\�����������l�̃X�e�[�g�}�V���̃J�[�g���b�W
*/


//-------------------�O���[�o�X�X�e�[�g
class StopPersonGlobalState :public State<StopPerson>
{
public:

	//this is a �V���O���g��
	static StopPersonGlobalState* GetInstance();

	// ����
	virtual void Enter(StopPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StopPerson* pPerson);

	// �A��
	virtual void Exit(StopPerson* pPerson);

	// �`��
	virtual void Render(StopPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	StopPersonGlobalState() {};
	~StopPersonGlobalState() {};

	StopPersonGlobalState(const StopPersonGlobalState&);
	StopPersonGlobalState& operator=(const StopPersonGlobalState&);
};



//--------------------�ŏ�

class StartStop :public State<StopPerson>
{
public:

	//this is a �V���O���g��
	static StartStop* GetInstance();

	// ����
	virtual void Enter(StopPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StopPerson* pPerson);

	// �A��
	virtual void Exit(StopPerson* pPerson);

	// �`��
	virtual void Render(StopPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	StartStop() {};
	~StartStop() {};

	StartStop(const StartStop&);
	StartStop& operator=(const StartStop&);
};


//--------------------�����Ă鎞�̑ҋ@
class ShedStop :public State<StopPerson>
{
public:

	//this is a �V���O���g��
	static ShedStop* GetInstance();

	// ����
	virtual void Enter(StopPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StopPerson* pPerson);

	// �A��
	virtual void Exit(StopPerson* pPerson);

	// �`��
	virtual void Render(StopPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	ShedStop() {};
	~ShedStop() {};

	ShedStop(const ShedStop&);
	ShedStop& operator=(const ShedStop&);
};


//--------------------�I��������̑ҋ@
class EndStop :public State<StopPerson>
{
public:

	//this is a �V���O���g��
	static EndStop* GetInstance();

	// ����
	virtual void Enter(StopPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StopPerson* pPerson);

	// �A��
	virtual void Exit(StopPerson* pPerson);

	// �`��
	virtual void Render(StopPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StopPerson* pPerson, const Message& msg);

private:
	EndStop() {};
	~EndStop() {};

	EndStop(const EndStop&);
	EndStop& operator=(const EndStop&);
};

