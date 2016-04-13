#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
�\�̔g��}�l�[�W���[.h

*/

class BasePerson;


// �N���A�����萔
enum class CLEAR_FLAG
{
	ALL_SHED = 1,			// �S���ɗ�������
	GOAL_PERSON = 2,		// ����L�����ɉ\�𗬂�����
	DONT_SHED_PERSON = 4	// �����ɂ͗����ȁI
};

class JudgeManager :public BaseGameEntity
{
public:
	static JudgeManager& GetInstance();
	static void Release();
	virtual ~JudgeManager();

	// �������z�֐��ɂ��p����������
	void Update();
	bool HandleMessage(const Message& msg);

	// �N���A�����ݒ�
	void SetClearFlag(CLEAR_FLAG flag);
	void SetClearFlag(BYTE flag);			// �r�b�g���Z�𗘗p���āA����l���ɗ����Ȃ����A�S����or����l���ɗ������ł���I�I

	// �����̃A�N�Z�T
	CLEAR_FLAG GetClearFlag(){ return m_ClearFlag; }
	bool isDontShedPerson(){ return m_bDontShed; }

	// ���݂̏�Ԓ萔
	enum class JUDGEMODE
	{
		NONE,			// �Q�[���i�s��
		GAME_CLEAR,		// �Q�[���N���A
		GAME_OVER		// �Q�[���I�[�o�[
	}m_JudgeMode;

	// ���N���A�������A�Q�[���I�[�o�[�ɂȂ������̃Q�b�^�[
	bool isGameClear(){ return (m_JudgeMode == JUDGEMODE::GAME_CLEAR); }
	bool isGameOver(){ return (m_JudgeMode == JUDGEMODE::GAME_OVER); }

	// �N���A�������A�Q�[���I�[�o�[�ɂȂ������̃Z�b�^�[
	// �N���A������[���b�Z�[�W����M����̂̓}�l�[�W���[�ł͂Ȃ��A�����Ă���X�e�[�g�}�V������M����̂ŁA�X�e�[�g�}�V������̃A�N�Z�X���K�v�ɂȂ�)
	void SetJudgeMode(JUDGEMODE mode){ if (m_JudgeMode != JUDGEMODE::GAME_OVER) m_JudgeMode = mode; }

private:
	static JudgeManager* pInstance;

	// �X�e�[�g�}�V��(HandleMessage�Ŏg���̂�)
	StateMachine<JudgeManager> *m_pStateMachine;

	// �N���A����(UI����̃A�N�Z�X�Ƃ���)
	CLEAR_FLAG m_ClearFlag;

	// �����ɂ͗����ȃt���O
	bool m_bDontShed;

	// ����
	JudgeManager();
	JudgeManager(const JudgeManager&);
	JudgeManager& operator=(const JudgeManager&);


};

#define JudgeMgr (JudgeManager::GetInstance())





//*****************************************************************************************************************************
//
//		�N���A����̃X�e�[�g(�ʓ|�Ȃ̂ł����ɏ���)
//
//*****************************************************************************************************************************

// ���O�����\��������̂ŁAnamespace�ŋ�؂�
namespace Judge
{

//--------------------�O���[�o���X�e�[�g
	class GlobalState :public State<JudgeManager>
	{
	public:

		//this is a �V���O���g��
		static GlobalState* GetInstance()
		{
			static GlobalState inst;
			return &inst;
		}

		// ����
		void Enter(JudgeManager* pJudge){}

		// ���s���܂�
		void Execute(JudgeManager* pJudge){}

		// �A��
		void Exit(JudgeManager* pJudge){}

		// �`��
		void Render(JudgeManager* pJudge){}

		// �g���̂̓��b�Z�[�W���炢
		bool OnMessage(JudgeManager* pJudge, const Message& msg);

	private:
		GlobalState() {};
		~GlobalState() {};
		GlobalState(const GlobalState&){}
		GlobalState& operator=(const GlobalState&){}
	};

	//--------------------�S���ɗ���
	class AllShed :public State<JudgeManager>
	{
	public:

		//this is a �V���O���g��
		static AllShed* GetInstance()
		{
			static AllShed inst;
			return &inst;
		}

		// ����
		void Enter(JudgeManager* pJudge);

		// ���s���܂�
		void Execute(JudgeManager* pJudge);

		// �A��
		void Exit(JudgeManager* pJudge){}

		// �`��
		void Render(JudgeManager* pJudge){}

		// �g���̂̓��b�Z�[�W���炢
		virtual bool OnMessage(JudgeManager* pJudge, const Message& msg);

	private:
		int m_ShedCount;	// ���l��������
		int m_GoalCount;	// �N���A�ɕK�v�ȉ�

		AllShed() :m_ShedCount(0), m_GoalCount(0) {};
		~AllShed() {};
		AllShed(const AllShed&){}
		AllShed& operator=(const AllShed&){}
	};

	//--------------------�S�[���l���ɉ\�𗬂�
	class GoalPerson :public State<JudgeManager>
	{
	public:

		//this is a �V���O���g��
		static GoalPerson* GetInstance()
		{
			static GoalPerson inst;
			return &inst;
		}

		// ����
		virtual void Enter(JudgeManager* pJudge){}

		// ���s���܂�
		virtual void Execute(JudgeManager* pJudge);

		// �A��
		virtual void Exit(JudgeManager* pJudge){}

		// �`��
		virtual void Render(JudgeManager* pJudge){}

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(JudgeManager* pJudge, const Message& msg);

	private:
		GoalPerson() {};
		~GoalPerson() {};
		GoalPerson(const GoalPerson&){}
		GoalPerson& operator=(const GoalPerson&){}
	};
}