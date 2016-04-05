#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class StartPerson;


/*
	�ŏ��̐l�̃X�e�[�g�}�V���̃J�[�g���b�W
*/


//-------------------�O���[�o�X�X�e�[�g
class StartPersonGlobalState :public State<StartPerson>
{
public:

	//this is a �V���O���g��
	static StartPersonGlobalState* GetInstance();

	// ����
	virtual void Enter(StartPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StartPerson* pPerson);

	// �A��
	virtual void Exit(StartPerson* pPerson);

	// �`��
	virtual void Render(StartPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	StartPersonGlobalState() {};
	~StartPersonGlobalState() {};

	StartPersonGlobalState(const StartPersonGlobalState&);
	StartPersonGlobalState& operator=(const StartPersonGlobalState&);
};



//--------------------�ŏ�

class FirstStart :public State<StartPerson>
{
public:

	//this is a �V���O���g��
	static FirstStart* GetInstance();

	// ����
	virtual void Enter(StartPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StartPerson* pPerson);

	// �A��
	virtual void Exit(StartPerson* pPerson);

	// �`��
	virtual void Render(StartPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	FirstStart() {};
	~FirstStart() {};

	FirstStart(const FirstStart&);
	FirstStart& operator=(const FirstStart&);
};


//--------------------�����Ă鎞�̑ҋ@
class FirstShed :public State<StartPerson>
{
public:

	//this is a �V���O���g��
	static FirstShed* GetInstance();

	// ����
	virtual void Enter(StartPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StartPerson* pPerson);

	// �A��
	virtual void Exit(StartPerson* pPerson);

	// �`��
	virtual void Render(StartPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	FirstShed() {};
	~FirstShed() {};

	FirstShed(const FirstShed&);
	FirstShed& operator=(const FirstShed&);
};


//--------------------�I��������̑ҋ@
class FirstEnd :public State<StartPerson>
{
public:

	//this is a �V���O���g��
	static FirstEnd* GetInstance();

	// ����
	virtual void Enter(StartPerson* pPerson);

	// ���s���܂�
	virtual void Execute(StartPerson* pPerson);

	// �A��
	virtual void Exit(StartPerson* pPerson);

	// �`��
	virtual void Render(StartPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(StartPerson* pPerson, const Message& msg);

private:
	FirstEnd() {};
	~FirstEnd() {};

	FirstEnd(const FirstEnd&);
	FirstEnd& operator=(const FirstEnd&);
};

