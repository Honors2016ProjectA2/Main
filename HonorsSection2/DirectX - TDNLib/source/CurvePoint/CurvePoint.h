#pragma once

//--------- CurvePointLock class -----------
//--------- CurvePoint class ---------

// �O���錾
class StageManager;

enum class CURVEPOINT_TYPE
{
	DOG,	// ��
	FIRE,	// ��
	MAX
};

namespace CurvePoint
{
	class Base
	{
	public:
		enum CurvePointState { OPEN, CLOSE };
		//--------- field ---------
	protected:
		tdn2DObj* m_pImage;
		const int W;
		const int H;
		const int SW;
		const int SH;
		Vector2 m_pos;
		CurvePointState state;
		int m_AnimFrame, m_AnimPanel;
		DIR m_dir;	// �r���Ȃ������
		int m_ID;	// ������ID
		int m_floor;
		StageManager *m_pMgr;
	public:
		const int LOCK_TIME;
		int lockTimer;

		bool bEnable;	// �L�����ǂ���

		//------- method ---------
	public:
		Base(StageManager *pMgr,tdn2DObj* image, Vector2 pos, DIR dir, int ID, int floor);
		virtual ~Base();
		virtual void Update();
		virtual void Render();
		void RenderFront();
		void SetPos(int y);
		virtual void Change()		// ON_OFF�̐؂�ւ�
		{
			state = IsOpening() ? CurvePointState::CLOSE : CurvePointState::OPEN;
		}
		bool IsOpening();
		int GetWidth();
		DIR GetDir(){ return m_dir; }		// �J�[�u�̕����擾
		Vector2 &GetPos(){ return m_pos; }
		void Get_pos2(Vector2 &out){ out = m_pos; }
		bool m_bCursorIn;	// �}�E�X�J�[�\�����͈͓��ɓ����Ă邩�ǂ���
		Vector2 GetCenterPos(){ return Vector2(m_pos.x + 60, m_pos.y + 60); }
		int GetID(){ return m_ID; }
		int GetFloor(){ return m_floor; }
	};

	// ��
	class Dog :public Base
	{
	public:
		Dog(StageManager *pMgr, tdn2DObj *image, Vector2 pos, DIR dir, int ID, int floor) :Base(pMgr, image, pos, dir, ID, floor){}
	};


	// ���X�e�[�g
	//enum class FIRE_MODE
	//{
	//	MAKI,		// �܂�
	//	MOEHAJIME,	// �R���n��
	//	MOETEKITA,	// �R���Ă���
	//	MOETA,		// �R����
	//	ENABLE,		// ���̏�Ԃŗr�����ɂȂ�
	//	HITED,		// �q�b�g������
	//	MAX
	//};
	//
	//// ��
	//class Fire : public Base
	//{
	//public:
	//	Fire(StageManager *pMgr, tdn2DObj *image, Vector2 pos, DIR dir, int ID, int floor);
	//	~Fire();
	//	void Update();
	//	void Render();
	//
	//	// �Q�b�^�[
	//	FIRE_MODE GetMode(){ return m_pMode->GetMode(); }
	//	int GetPosX(){ return (int)m_pos.x; }
	//	int GetPosY(){ return (int)m_pos.y; }
	//
	//	// �Z�b�^�[
	//	void Change();	// �I�[�o�[���C�h
	//	void Hit(){ ChangeMode(FIRE_MODE::HITED); }
	//	void ChangeMode(FIRE_MODE m);
	//
	//private:
	//	tdn2DObj *m_pFireImage;
	//
	//	// �q���Ϗ����[�h
	//	class Mode
	//	{
	//	public:
	//		class Base
	//		{
	//		protected:
	//			FIRE_MODE m_NextMode;
	//			tdn2DObj *m_pImage;			// �摜
	//			int m_timer, m_limit;		// �ڕW�ƍ��̎���
	//			int m_srcX;
	//			Fire *pFire;			// �Ϗ����̃|�C���^
	//		public:
	//			Base(Fire *me, tdn2DObj *image) :pFire(me), m_pImage(image), m_timer(0), m_limit(0){}
	//			virtual ~Base(){}
	//			virtual void Update();
	//			virtual void Render();
	//			virtual FIRE_MODE GetMode() = 0;
	//			void NextMode();
	//			void SetChangeModeTime(int val){ m_limit = val; }
	//		};
	//
	//		class Maki :public Base
	//		{
	//		public:
	//			Maki(Fire *me, tdn2DObj *image);
	//			//void Update(Fire *pFire);
	//			//void Render(Fire *pFire);
	//			FIRE_MODE GetMode(){ return FIRE_MODE::MAKI; }
	//		};
	//
	//		class Moehajime :public Base
	//		{
	//		public:
	//			Moehajime(Fire *me, tdn2DObj *image);
	//			//void Update(Fire *pFire);
	//			//void Render(Fire *pFire);
	//			FIRE_MODE GetMode(){ return FIRE_MODE::MOEHAJIME; }
	//		};
	//
	//		class Moetekita :public Base
	//		{
	//		public:
	//			Moetekita(Fire *me, tdn2DObj *image);
	//			//void Update(Fire *pFire);
	//			//void Render(Fire *pFire);
	//			FIRE_MODE GetMode(){ return FIRE_MODE::MOETEKITA; }
	//		};
	//
	//		class Moeta :public Base
	//		{
	//		public:
	//			Moeta(Fire *me, tdn2DObj *image);
	//			//void Update(Fire *pFire);
	//			//void Render(Fire *pFire);
	//			FIRE_MODE GetMode(){ return FIRE_MODE::MOETA; }
	//		};
	//
	//		class Enable :public Base
	//		{
	//		private:
	//			int m_KiraKiraCoolTime;
	//		public:
	//			Enable(Fire *me, tdn2DObj *image);
	//			void Update()override;
	//			//void Render(Fire *pFire);
	//			FIRE_MODE GetMode(){ return FIRE_MODE::ENABLE; }
	//		};
	//
	//		class Hited : public Base
	//		{
	//		public:
	//			Hited(Fire *me, tdn2DObj *image);
	//			void Update()override;
	//			//void Render(Fire *pFire);
	//			FIRE_MODE GetMode(){ return FIRE_MODE::HITED; }
	//		};
	//	};
	//
	//	// ���[�h�Ϗ��|�C���^
	//	Mode::Base *m_pMode;
	//
	//};

}
