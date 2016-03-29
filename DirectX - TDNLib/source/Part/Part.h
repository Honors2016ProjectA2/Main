#pragma once

//*****************************************************************************************************************************
//
//		�p�[�g�Ǘ��N���X
//
//*****************************************************************************************************************************

class sceneMain;

class sceneMainGlobalState :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainGlobalState* GetInstance();

	// ����
	virtual void Enter(sceneMain* pMain);

	// ���s���܂�
	virtual void Execute(sceneMain* pMain);

	// �A��
	virtual void Exit(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGlobalState() {};
	~sceneMainGlobalState() {};

	sceneMainGlobalState(const sceneMainGlobalState&);
	sceneMainGlobalState& operator=(const sceneMainGlobalState&);
};


//--------------------���C��
class sceneMainGame :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainGame* GetInstance();

	// ����
	virtual void Enter(sceneMain* pMain);

	// ���s���܂�
	virtual void Execute(sceneMain* pMain);

	// �A��
	virtual void Exit(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGame() {};
	~sceneMainGame() {};

	sceneMainGame(const sceneMainGame&);
	sceneMainGame& operator=(const sceneMainGame&);

	// �g��̉�
	int m_RippleCount;

};

//--------------------�Q�[���I�[�o�[
class sceneMainGameOver :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainGameOver* GetInstance();

	// ����
	virtual void Enter(sceneMain* pMain);

	// ���s���܂�
	virtual void Execute(sceneMain* pMain);

	// �A��
	virtual void Exit(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameOver() {};
	~sceneMainGameOver() {};

	sceneMainGameOver(const sceneMainGameOver&);
	sceneMainGameOver& operator=(const sceneMainGameOver&);
};

//--------------------�Q�[���N���A�[
class sceneMainGameClear :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainGameClear* GetInstance();

	// ����
	virtual void Enter(sceneMain* pMain);

	// ���s���܂�
	virtual void Execute(sceneMain* pMain);

	// �A��
	virtual void Exit(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameClear() {};
	~sceneMainGameClear() {};

	sceneMainGameClear(const sceneMainGameOver&);
	sceneMainGameClear& operator=(const sceneMainGameOver&);
};