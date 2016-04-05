#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class GoalPerson;


/*
�ŏ��̐l�̃X�e�[�g�}�V���̃J�[�g���b�W
*/


//-------------------�O���[�o�X�X�e�[�g
class GoalPersonGlobalState :public State<GoalPerson>
{
public:

	//this is a �V���O���g��
	static GoalPersonGlobalState* GetInstance();

	// ����
	virtual void Enter(GoalPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GoalPerson* pPerson);

	// �A��
	virtual void Exit(GoalPerson* pPerson);

	// �`��
	virtual void Render(GoalPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalPersonGlobalState() {};
	~GoalPersonGlobalState() {};

	GoalPersonGlobalState(const GoalPersonGlobalState&);
	GoalPersonGlobalState& operator=(const GoalPersonGlobalState&);
};



//--------------------�ŏ�

class GoalStart :public State<GoalPerson>
{
public:

	//this is a �V���O���g��
	static GoalStart* GetInstance();

	// ����
	virtual void Enter(GoalPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GoalPerson* pPerson);

	// �A��
	virtual void Exit(GoalPerson* pPerson);

	// �`��
	virtual void Render(GoalPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalStart() {};
	~GoalStart() {};

	GoalStart(const GoalStart&);
	GoalStart& operator=(const GoalStart&);
};


//--------------------�����Ă鎞�̑ҋ@
class GoalShed :public State<GoalPerson>
{
public:

	//this is a �V���O���g��
	static GoalShed* GetInstance();

	// ����
	virtual void Enter(GoalPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GoalPerson* pPerson);

	// �A��
	virtual void Exit(GoalPerson* pPerson);

	// �`��
	virtual void Render(GoalPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalShed() {};
	~GoalShed() {};

	GoalShed(const GoalShed&);
	GoalShed& operator=(const GoalShed&);
};


//--------------------�I��������̑ҋ@
class GoalEnd :public State<GoalPerson>
{
public:

	//this is a �V���O���g��
	static GoalEnd* GetInstance();

	// ����
	virtual void Enter(GoalPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GoalPerson* pPerson);

	// �A��
	virtual void Exit(GoalPerson* pPerson);

	// �`��
	virtual void Render(GoalPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GoalPerson* pPerson, const Message& msg);

private:
	GoalEnd() {};
	~GoalEnd() {};

	GoalEnd(const GoalEnd&);
	GoalEnd& operator=(const GoalEnd&);
};

