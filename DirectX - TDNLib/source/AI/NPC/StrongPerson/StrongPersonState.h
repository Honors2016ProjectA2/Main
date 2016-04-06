#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class StrongPerson;


/*
���܂��Ă���l�̂̃X�e�[�g�}�V���̃J�[�g���b�W
*/


//-------------------�O���[�o�X�X�e�[�g
class StrongPersonGlobalState :public State<StrongPerson>
{
public:

	//this is a �V���O���g��
	static StrongPersonGlobalState* GetInstance();

	// ����
	virtual void Enter(StrongPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StrongPerson* pPerson);

	// �A��
	virtual void Exit(StrongPerson* pPerson);

	// �`��
	virtual void Render(StrongPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	StrongPersonGlobalState() {};
	~StrongPersonGlobalState() {};

	StrongPersonGlobalState(const StrongPersonGlobalState&);
	StrongPersonGlobalState& operator=(const StrongPersonGlobalState&);
};



//--------------------�ŏ�

class StartStrong :public State<StrongPerson>
{
public:

	//this is a �V���O���g��
	static StartStrong* GetInstance();

	// ����
	virtual void Enter(StrongPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StrongPerson* pPerson);

	// �A��
	virtual void Exit(StrongPerson* pPerson);

	// �`��
	virtual void Render(StrongPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	StartStrong() {};
	~StartStrong() {};

	StartStrong(const StartStrong&);
	StartStrong& operator=(const StartStrong&);
};


//--------------------�����Ă鎞�̑ҋ@
class ShedStrong :public State<StrongPerson>
{
public:

	//this is a �V���O���g��
	static ShedStrong* GetInstance();

	// ����
	virtual void Enter(StrongPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StrongPerson* pPerson);

	// �A��
	virtual void Exit(StrongPerson* pPerson);

	// �`��
	virtual void Render(StrongPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	ShedStrong() {};
	~ShedStrong() {};

	ShedStrong(const ShedStrong&);
	ShedStrong& operator=(const ShedStrong&);
};


//--------------------�I��������̑ҋ@
class EndStrong :public State<StrongPerson>
{
public:

	//this is a �V���O���g��
	static EndStrong* GetInstance();

	// ����
	virtual void Enter(StrongPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StrongPerson* pPerson);

	// �A��
	virtual void Exit(StrongPerson* pPerson);

	// �`��
	virtual void Render(StrongPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StrongPerson* pPerson, const Message& msg);

private:
	EndStrong() {};
	~EndStrong() {};

	EndStrong(const EndStrong&);
	EndStrong& operator=(const EndStrong&);
};

