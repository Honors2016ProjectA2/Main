#pragma once
#include "IEX_Expansion.h"

enum class ENEMY_TYPE
{
	WOLF,	// �T
	METEO,	// ���e�I
	MAX
};

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
		Base(tdn2DObj *image, int floor, float speed) :m_bErase(false), m_floor(floor), m_pImage(image), m_AnimeFrame(0), m_AnimePanel(0), m_MoveVec(Vector2(-1, 0)), m_speed(speed){}
		virtual void Update() = 0;
		virtual void Render(){}

		// �Q�b�^�[
		int GetFloor(){ return m_floor; }
		Vector2 GetCenterPos(){ return Vector2(m_pos.x + W*.5f, m_pos.y + H*.5f); }
		int GetWidth(){ return W; }

		// �����֌W
		bool EraseOK(){ return m_bErase; }
		bool Erase(){ m_bErase = true; }
	};

	class Wolf : public Base
	{
	public:
		Wolf(tdn2DObj *image, int floor, float speed);
		void Update();
		void Render();
	};

	class Meteo : public Base
	{
	public:
		Meteo(tdn2DObj *image, int floor, float speed);
		void Update();
		void Render();
	};
}

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

	std::list<Enemy::Base*> *GetList(){ return &m_list; }
	void Set_Pointers(StageManager *sm, DataManager *dm){ sp = sm, dmp = dm; }

private:

	// �V���O���g���̍�@
	EnemyManager();
	EnemyManager(const EnemyManager&){}
	EnemyManager &operator=(const EnemyManager){}

	// �G�̉摜
	tdn2DObj *m_pImages[(int)ENEMY_TYPE::MAX];

	// �G�̈ړ����x
	float m_EnemySpeed[(int)ENEMY_TYPE::MAX];

	// �G�������i�[���郊�X�g
	std::list<Enemy::Base*> m_list;
	int m_CreateTimer;					// �G�����^�C�}�[
	int m_CREATETIME;					// ��������鎞��

	StageManager *sp;
	DataManager *dmp;

	// �G����
	void Create(int floor, ENEMY_TYPE type);
};


// �C���X�^���X��
#define EnemyMgr (EnemyManager::GetInstance())