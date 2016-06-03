#pragma once
#include "IEX_Expansion.h"

enum class ENEMY_TYPE
{
	WOLF,	// �T
	MAX
};


enum class FAT_WOLF_TYPE
{
	SMALL, MIDDLE, LARGE
};

enum class SHEEP_TYPE;

namespace Enemy
{
	class Base
	{
	protected:
		bool m_bErase;			// �����t���O
		tdn2DObj *m_pImage;		// �G�̉摜(�}�l�[�W���̂��Q�Ƃ��邾��)
		Vector2 m_pos;			// ���W
		Vector2 m_MoveVec;		// �ړ��̕���
		float m_speed;			// �ړ����x
		int m_floor;			// ���[��
		int m_AnimeFrame, m_AnimePanel;	// �A�j���֌W
		int W, H;			// ���ƍ���

		// �ړ��ʂ̗ނ̍X�V
		void MoveUpdate();
	public:
		Base(tdn2DObj *image, int floor, float speed) :col_check(false), m_bErase(false), m_floor(floor), m_pImage(image), m_AnimeFrame(0), m_AnimePanel(0), m_MoveVec(Vector2(-1, 0)), m_speed(speed){}
		virtual void Update() = 0;
		virtual void Render(){}

		// �Q�b�^�[
		int GetFloor(){ return m_floor; }
		Vector2 GetCenterPos(){ return Vector2(m_pos.x + W*.5f, m_pos.y + H*.5f); }
		Vector2 GetPos(){ return m_pos; }
		int GetWidth(){ return W; }

		// �Z�b�^�[
		void SetCenterPos(const Vector2 &pos){ m_pos = Vector2(pos.x - 60, pos.y - 60); }

		// �����֌W
		bool EraseOK(){ return m_bErase; }
		void Erase(){ m_bErase = true; }

		bool col_check;
	};

	class Wolf : public Base
	{
	public:
		Wolf(tdn2DObj *image, tdn2DObj *pniku, tdn2DObj *pHone, int floor, float speed, int nikustopTime);
		void Update();
		void Render();

		enum class MODE{ RUN, NIKU, DEAD, MAX }m_mode;
		void ChangeMode(MODE m)
		{
			m_AnimeFrame = m_AnimePanel = 0;
			m_mode = m;
			switch (m)
			{
			case MODE::RUN:
				m_MoveVec = Vector2(-1, 0);
				m_speed = m_OrgSpeed;
				break;
			case MODE::NIKU:
				m_MoveVec = Vector2(-1, 0);
				m_speed = 0;
				m_EatNikuTimer = 0;
				break;
			case MODE::DEAD:
				m_MoveVec = Vector2(1, 0);
				m_speed = 0;
				break;
			default:
				break;
			}
		}
		MODE GetMode(){ return m_mode; }

		void Kill(){ ChangeMode(MODE::DEAD); }

		void SetFatType(FAT_WOLF_TYPE type){ m_type = type; }
		void SetSheepType(SHEEP_TYPE type){ m_SheepType = type; }

	private:
		tdn2DObj *m_pNikukutteru, *m_pHoneImage;
		int m_EAT_NIKU_TIMER;	// �Œ�l
		int m_EatNikuTimer;
		float m_OrgSpeed;
		FAT_WOLF_TYPE m_type;	// ���H���I�������ɂȂ�^�C�v
		SHEEP_TYPE m_SheepType;
		BYTE m_alpha;

		void Run();
		void Niku();
		void Dead();
		void(Wolf::*ModeFunk[(int)MODE::MAX])();
	};
}

class FatWolf : public DebuBase
{
public:
	FatWolf(tdn2DObj *image, const Vector2 &pos, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType);
	~FatWolf();
	void Update();
	void Render();

	// �Q�b�^�[
	FAT_WOLF_TYPE GetType(){ return m_type; }
	SHEEP_TYPE GetSheepType(){ return m_SheepType; }

private:
	FAT_WOLF_TYPE m_type;
	SHEEP_TYPE m_SheepType;
};

class EnemyManager
{
public:
	// ���̂̎擾
	static EnemyManager *GetInstance(){ static EnemyManager i; return &i; }

	void Initialize();
	void Release();
	void Update();
	void Render();

	/* �f�[�^���Z�b�g */
	void Clear();

	std::list<Enemy::Wolf*> *GetList(){ return &m_list; }
	std::list<FatWolf*> *GetFatList(){ return &m_FatList; }
	void Set_Pointers(StageManager *sm, DataManager *dm){ sp = sm, dmp = dm; }

	// ���H���I�������Ăяo��
	void CreateFatWolf(Enemy::Wolf *wolf, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType);

private:

	// �V���O���g���̍�@
	EnemyManager();
	EnemyManager(const EnemyManager&){}
	EnemyManager &operator=(const EnemyManager){}

	// �G�̉摜
	tdn2DObj *m_pImages[(int)ENEMY_TYPE::MAX];

	// ���H���Ă�I�I�J�~�̉摜
	tdn2DObj *m_pNikukutteru, *m_pFatWolfImages[(int)SHEEP_TYPE::MAX], *m_pHoneImage;

	// �G�̈ړ����x
	float m_EnemySpeed[(int)ENEMY_TYPE::MAX];

	// �j�b�H���Ă鎞�Ԃ�
	int m_NikuTime;

	// �G�������i�[���郊�X�g
	std::list<Enemy::Wolf*> m_list;
	int m_CreateTimer;					// �G�����^�C�}�[
	int m_CREATETIME;					// ��������鎞��
	int m_NextFloor;

	// ���H���đ������T���X�g
	std::list<FatWolf*> m_FatList;

	StageManager *sp;
	DataManager *dmp;

	// �G����
	void Create(int floor, ENEMY_TYPE type);
};


// �C���X�^���X��
#define EnemyMgr (EnemyManager::GetInstance())