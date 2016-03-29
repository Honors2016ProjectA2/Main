#pragma once

//*****************************************************************************************************************************
//
//		�p�[�g�Ǘ��N���X
//
//*****************************************************************************************************************************

class  PartManager
{
public:
	PartManager();
	~PartManager();
	void Update();
	void Render();

	enum class PART
	{
		MAIN,			// �Q�[���̃��C��
		GAME_CLEAR,		// �Q�[���I�[�o�[���̏���
		GAME_OVER,		// �Q�[���N���A���̏���
		MAX
	};

	// �p�[�g�؂�ւ�
	void ChangePart(PART p);

private:

	class Part
	{
	public:
		class Base
		{
		protected:
			PartManager *mgr;		// �Ǘ����Ă�l�̎���(�Ǘ����Ă�l�̃X�e�b�v��i�߂�̂Ɏg��)
			int process;		// step�S�����Ă邯�ǂ���ɒ��̃X�e�b�v
			int timer;			// �ėp�I�ȃ^�C�}�[
			enum class CURSOR{ LEFT, RIGHT, NONE }cursor;
		public:
			Base(PartManager *mgr) :mgr(mgr), process(0), timer(0){}
			virtual ~Base(){}
			virtual void Initialize(){}
			virtual void Update() = 0;
			virtual void Render() = 0;
		};

		class Main : public Base
		{
		public:
			Main(PartManager *mgr) :Base(mgr){}
			~Main(){}
			void Initialize();
			void Update();
			void Render();
		};

		class GameClear : public Base
		{
		public:
			GameClear(PartManager *mgr) :Base(mgr){}
			~GameClear(){}
			void Initialize();
			void Update();
			void Render();
		};

		class GameOver : public Base
		{
		public:
			GameOver(PartManager *mgr) :Base(mgr){}
			~GameOver(){}
			void Initialize();
			void Update();
			void Render();
		};
	};

	Part::Base *part_worker;

};