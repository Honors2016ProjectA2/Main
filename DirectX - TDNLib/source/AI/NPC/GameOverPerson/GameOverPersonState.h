#pragma once
#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"

class GameOverPerson;


/*
	�\�����������l�̃X�e�[�g�}�V���̃J�[�g���b�W
*/


//-------------------�O���[�o�X�X�e�[�g
class GameOverPersonGlobalState :public State<GameOverPerson>
{
public:

	//this is a �V���O���g��
	static GameOverPersonGlobalState* GetInstance();

	// ����
	virtual void Enter(GameOverPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GameOverPerson* pPerson);

	// �A��
	virtual void Exit(GameOverPerson* pPerson);

	// �`��
	virtual void Render(GameOverPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	GameOverPersonGlobalState() {};
	~GameOverPersonGlobalState() {};

	GameOverPersonGlobalState(const GameOverPersonGlobalState&);
	GameOverPersonGlobalState& operator=(const GameOverPersonGlobalState&);
};



//--------------------�ŏ�

class StartGameOver :public State<GameOverPerson>
{
public:

	//this is a �V���O���g��
	static StartGameOver* GetInstance();

	// ����
	virtual void Enter(GameOverPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GameOverPerson* pPerson);

	// �A��
	virtual void Exit(GameOverPerson* pPerson);

	// �`��
	virtual void Render(GameOverPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	StartGameOver() {};
	~StartGameOver() {};

	StartGameOver(const StartGameOver&);
	StartGameOver& operator=(const StartGameOver&);
};


//--------------------�����Ă鎞�̑ҋ@
class ShedGameOver :public State<GameOverPerson>
{
public:

	//this is a �V���O���g��
	static ShedGameOver* GetInstance();

	// ����
	virtual void Enter(GameOverPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GameOverPerson* pPerson);

	// �A��
	virtual void Exit(GameOverPerson* pPerson);

	// �`��
	virtual void Render(GameOverPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	ShedGameOver() {};
	~ShedGameOver() {};

	ShedGameOver(const ShedGameOver&);
	ShedGameOver& operator=(const ShedGameOver&);
};


//--------------------�I��������̑ҋ@
class EndGameOver :public State<GameOverPerson>
{
public:

	//this is a �V���O���g��
	static EndGameOver* GetInstance();

	// ����
	virtual void Enter(GameOverPerson* pPerson);

	// ���s���܂�
	virtual void Execute(GameOverPerson* pPerson);

	// �A��
	virtual void Exit(GameOverPerson* pPerson);

	// �`��
	virtual void Render(GameOverPerson* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(GameOverPerson* pPerson, const Message& msg);

private:
	EndGameOver() {};
	~EndGameOver() {};

	EndGameOver(const EndGameOver&);
	EndGameOver& operator=(const EndGameOver&);
};

