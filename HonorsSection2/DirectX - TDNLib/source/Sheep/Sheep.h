#pragma once
#include "IEX_Expansion.h"
class MousePointer;
class StageManager;

enum class SHEEP_TYPE
{
	NOMAL,	// �ʏ�r
	GOLD,	// ���r
	REAL,	// ����
	MAX
};

struct SheepData
{
	tdn2DObj *Image;		// �摜
	int score;				// ���_
	int SIZE;				// �T�C�Y(�R�}��)
	int NumPanel;			// �R�}�̐�
	int Animkankaku;		// �A�j���̊Ԋu 
};

struct SheepTextParam
{
	int startX;			// �r�o�����W
	int AppTime;		// �r�̏o���Ԋu(�~���b)
	int speed;			// ���x
	int VerticalWidth;	// �o���̏㉺��
	SheepData data[(int)SHEEP_TYPE::MAX];
};

class Sheep
{
private:
	int frame, animeframe, m_AnimePanel;
	Vector2 pos, animepos,
		move,		// �J�[�u�Ƃ��ŁA���Ƃ��瑫���悤�̈ړ��l
		C_MOVE;		// �{�������Ă���Œ�̈ړ��l
	DIR m_CurveDir;	// �J�[�u�̌���
	bool m_bTurned;	// �T�C���Œ��p�ɋȂ�����A�߂��ɕK�v
	float m_sinAngle;	// �J�[�u���T�C���J�[�u�ł���̂ŁA����p�̕ϐ�

	//const int PNGSIZE;
	int floor;

	SheepData m_data;

	enum MODE{ 
		GETOUT,		// Initialize�I�Ȃ���ۂ�
		WALK,		// ���ʂ̕���
		CURVE,		// �����]��
		CAUGHT,		// ���ʂ̎���
		CRUSHED,	// �S�����ɎE���ꂽ
		RAN_OVER	// 瀂��E���ꂽ�H
	};
	int process;
	DWORD alpha;

	void Get_frame_pos(Vector2 &pos, int frame);

	struct
	{
		tdn2DObj *byu;
		int animex;
	}byu;
	struct
	{
		tdn2DObj *ase;
		int animex;

	}ase;

	float anim = .0f;	//�r���[���p�ǉ�

	bool Get_out();
	bool Walk();
	bool Curve();
	bool Caught();
	bool Crushed();
	bool Ran_over();
	bool(Sheep::*Move[6])();
public:
	Sheep(const SheepData &data, tdn2DObj *byu, tdn2DObj *ase, int floor, const SheepTextParam &textparam);
	~Sheep();

	bool Update();

	void Render();

	/* ���� */
	Vector2 *Get_pos(){ return &pos; }
	/* ������ */
	void Get_pos2(Vector2 &out)
	{
		out = pos;
		out.x += m_data.SIZE * 0.5f;
		out.y += m_data.SIZE;
	}
	int Get_size(){ return m_data.SIZE; }
	int Get_floor(){ return floor; }
	/* ���ꂽ */
	void Be_caught(int type);
	void Be_crushed();
	void Be_ran_over();
	void SetCurve(DIR dir)
	{
		if (process != MODE::WALK) return;
		m_CurveDir = dir; process = MODE::CURVE; m_sinAngle = 0; m_bTurned = false;
	}

	bool col_check;
};

//**************************************************

class SheepManager
{
public:
	std::list<Sheep*> *Get_list(){ return &m_List; };

	SheepManager();
	~SheepManager();

	void Update();
	void Render();

	void Set_pointers(MousePointer *mp, StageManager *sp, DataManager *dmp){ this->mp = mp, this->sp = sp, this->dmp = dmp; };

	void Reset(){ for (auto it : m_List){ delete it; }m_List.clear(); }
	void Start(){ m_CurrentTime = clock(); }

private:
	//tdn2DObj *Getfile(int num){ return files[num]; }
	//tdn2DObj *files[(int)SHEEP_TYPE::MAX];
	tdn2DObj *byu, *ase;
	int m_CurveRange;		// ���ɑ΂��āA�Ȃ��ꖽ�ߏo���͈́B

	std::list<Sheep*> m_List;
	void create(int floor);

	// ���o�߂��Ă鎞��(�~���b)
	UINT m_CurrentTime;

	// �e�L�X�g����̏��
	SheepTextParam m_TextParam;

	MousePointer *mp;
	StageManager *sp;
	DataManager *dmp;
};