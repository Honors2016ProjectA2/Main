#pragma once

//*****************************************************************************************************************************
//
//		�p�[�g�Ǘ��N���X
//
//*****************************************************************************************************************************

#include "../Scene/SceneMain.h"

// �萔�̑O���錾
enum class PERSON_TYPE;

// �{�^����ID�i�V�[�����C�����炱���ɈڐA���܂����j
enum BUTTON_ID
{
	YES = 0,	// �R���e�B�j���[����
	NO = 1,
	RETRY = 2,
	BLUE = 3,
	RED = 4,
	GREEN = 5,
	START = 6
};

// �Q�[����Ԓ萔
enum class GAME_STATE
{
	INTRO, MAIN, GAME_OVER, GAME_CLEAR
};
// �Q�[����Ԃ��O���[�o���Ŏ���
extern GAME_STATE g_GameState;

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

	// �`��
	virtual void Render(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGlobalState() {};
	~sceneMainGlobalState() {};

	sceneMainGlobalState(const sceneMainGlobalState&);
	sceneMainGlobalState& operator=(const sceneMainGlobalState&);
};

//--------------------�Q�[���J�n���̃C���g��
class sceneMainIntro :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainIntro* GetInstance();

	// ����
	virtual void Enter(sceneMain* pMain);

	// ���s���܂�
	virtual void Execute(sceneMain* pMain);

	// �A��
	virtual void Exit(sceneMain* pMain);

	// �`��
	virtual void Render(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainIntro():m_timer(0) {};
	~sceneMainIntro() {};

	sceneMainIntro(const sceneMainIntro&);
	sceneMainIntro& operator=(const sceneMainIntro&);

	// �C���g���̃^�C�}�[
	int m_timer;
};

//--------------------�ݒu�p�[�g
class sceneMainSetPart :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainSetPart* GetInstance();

	// ����
	virtual void Enter(sceneMain* pMain);

	// ���s���܂�
	virtual void Execute(sceneMain* pMain);

	// �A��
	virtual void Exit(sceneMain* pMain);

	// �`��
	virtual void Render(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainSetPart(){};
	~sceneMainSetPart() {};

	sceneMainSetPart(const sceneMainSetPart&);
	sceneMainSetPart& operator=(const sceneMainSetPart&);

	// �z�u�Ƃ��ĕ`�悷�郁�b�V��
	iex3DObj *m_HoldMesh;
	Vector3 m_HoldMeshPos;

	// �I�񂾍����̃{�^���̐F
	enum class SELECT_BUTTON_COLOR
	{
		BLUE,
		RED,
		GREEN,
		MAX
	};
	int m_SelectButtonColor;	// �v���C���[���I�������{�^��(++�Ƃ�����̂�int)

	// �����̃{�^���̐F�Ɛݒu����l�Ԃ̕R�Â�
	std::map<SELECT_BUTTON_COLOR, PERSON_TYPE> ButtonPersonMap;

	// �I���{�^���F�؂�ւ�
	void ChangeSelectButton(sceneMain *pMain, SELECT_BUTTON_COLOR next);
};

//--------------------���C��(���킳������Ă�p�[�g)
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

	// �`��
	virtual void Render(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGame() {};
	~sceneMainGame() {};

	sceneMainGame(const sceneMainGame&);
	sceneMainGame& operator=(const sceneMainGame&);
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

	// �`��
	virtual void Render(sceneMain* pMain);

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

	// �`��
	virtual void Render(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameClear() {};
	~sceneMainGameClear() {};

	sceneMainGameClear(const sceneMainGameOver&);
	sceneMainGameClear& operator=(const sceneMainGameOver&);
};