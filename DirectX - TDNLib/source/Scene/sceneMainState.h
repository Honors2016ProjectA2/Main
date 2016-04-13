#pragma once

//*****************************************************************************************************************************
//
//		�p�[�g�Ǘ��N���X
//
//*****************************************************************************************************************************

#include "../Scene/SceneMain.h"

// �萔�̑O���錾
enum class PERSON_TYPE;

// �N���X�̑O���錾
class TutorialManager;

// �{�^����ID�i�V�[�����C�����炱���ɈڐA���܂����j
enum BUTTON_ID
{
	CONTINUE_YES = 0,
	CONTINUE_NO = 1,
	RETRY = 2,
	BLUE = 3,
	RED = 4,
	GREEN = 5,
	START = 6,
	MAX
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
	void Enter(sceneMain* pMain);

	// ���s���܂�
	void Execute(sceneMain* pMain);

	// �A��
	void Exit(sceneMain* pMain);

	// �`��
	void Render(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(sceneMain* pMain, const Message& msg);

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
	void Enter(sceneMain* pMain);

	// ���s���܂�
	void Execute(sceneMain* pMain);

	// �A��
	void Exit(sceneMain* pMain);

	// �`��
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainIntro():m_timer(0) {};
	~sceneMainIntro() {};

	sceneMainIntro(const sceneMainIntro&);
	sceneMainIntro& operator=(const sceneMainIntro&);

	// �C���g���̃^�C�}�[
	int m_timer;

	// �X�e�b�v(���Ԃ�)
	int m_step;

	// �N���A�����̉摜
	tdn2DObj *m_pImage;
	Vector2 m_ImagePos;

	// �X�e�b�v��i�߂�v�`�֐�
	void NextStep()
	{
		m_step++;
		m_timer = 0;
	}
};

//--------------------�ݒu�p�[�g
class sceneMainSetPart :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainSetPart* GetInstance();

	// ����
	void Enter(sceneMain* pMain);

	// ���s���܂�
	void Execute(sceneMain* pMain);

	// �A��
	void Exit(sceneMain* pMain);

	// �`��
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainSetPart() :m_HoldMesh(nullptr),m_pTutorial(nullptr){};
	~sceneMainSetPart();

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

	// �`���[�g���A�������s����|�C���^
	TutorialManager *m_pTutorial;
};

//--------------------���킳������Ă�p�[�g
class sceneMainGossip :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainGossip* GetInstance();

	// ����
	void Enter(sceneMain* pMain);

	// ���s���܂�
	void Execute(sceneMain* pMain);

	// �A��
	void Exit(sceneMain* pMain);

	// �`��
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGossip() {};
	~sceneMainGossip() {};

	sceneMainGossip(const sceneMainGossip&);
	sceneMainGossip& operator=(const sceneMainGossip&);
};

//--------------------�Q�[���I�[�o�[
class sceneMainGameOver :public State<sceneMain>
{
public:

	//this is a �V���O���g��
	static sceneMainGameOver* GetInstance();

	// ����
	void Enter(sceneMain* pMain);

	// ���s���܂�
	void Execute(sceneMain* pMain);

	// �A��
	void Exit(sceneMain* pMain);

	// �`��
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(sceneMain* pMain, const Message& msg);

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
	void Enter(sceneMain* pMain);

	// ���s���܂�
	void Execute(sceneMain* pMain);

	// �A��
	void Exit(sceneMain* pMain);

	// �`��
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(sceneMain* pMain, const Message& msg);

private:
	sceneMainGameClear() {};
	~sceneMainGameClear() {};

	sceneMainGameClear(const sceneMainGameOver&);
	sceneMainGameClear& operator=(const sceneMainGameOver&);
};