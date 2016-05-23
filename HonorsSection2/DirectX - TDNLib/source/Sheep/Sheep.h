#pragma once
#include "IEX_Expansion.h"
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
	float magnification;	// ���_�{��
	int SIZE;				// �T�C�Y(�R�}��)
	int NumPanel;			// �R�}�̐�
	int Animkankaku;		// �A�j���̊Ԋu 
	float percentage;		// �o����
};

struct SheepTextParam
{
	int startX;			// �r�o�����W
	int AppTime;		// �r�̏o���Ԋu(�~���b)
	int speed;			// ���x
	int VerticalWidth;	// �o���̏㉺��
	SheepData data[(int)SHEEP_TYPE::MAX];
	float FatSheepAccel;// �����Ă�r�̉����x
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
	bool m_bErase;	// �����t���O

	//const int PNGSIZE;
	int floor;

	// �r�̏��
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
		tdn2DObj *ase;
		int animex;

	}ase;

	void Get_out();
	void Walk();
	void Curve();
	void Caught();
	void Crushed();
	void Ran_over();
	void(Sheep::*Move[6])();
public:
	Sheep(const SheepData &data, tdn2DObj *ase, int floor, const SheepTextParam &textparam);
	~Sheep();

	void Update();

	void Render();

	/* ���� */
	Vector2 *Get_pos(){ return &pos; }
	/* ������ */
	Vector2 &GetCenterPos(){ return pos + Vector2(60, 60); }
	void Get_pos2(Vector2 &out)
	{
		out = pos;
		out.x += m_data.SIZE * 0.5f;
		out.y += m_data.SIZE;
	}
	void SetPos(const Vector2 &p){ pos = p; }
	void SetPosX(float x){ pos.x = x; }

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
	float TokutenBairitsu(){ return m_data.magnification; }

	// �����֌W
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

	bool col_check;
};


// �q���H���đ������r
class FatSheep
{
public:
	FatSheep(tdn2DObj *image, const Vector2 &pos);
	~FatSheep();

	// �X�V�E�`��
	void Update();
	void Render();

	// �Q�b�^�[
	Vector2 &GetCenterPos(){ return m_pos + Vector2(96, 96); }
	int GetWidth(){ return m_image->GetWidth(); }
	int GetFloor(){ return m_floor; }
	bool EraseOK(){ return m_bErase; }

	// �Z�b�^�[
	void SetMove(float val){ m_moveX = val; }
	void SetFloor(int val){ m_floor = val; }
	void Erase(){ m_bErase = true; }

private:
	Vector2 m_pos;		// ���W
	float m_moveX;		// �ړ��l
	tdn2DObj *m_image;	// �����̉摜
	float m_angle;		// �]����̂ŁA�A���O���B
	int m_floor;		// �����̂��郌�[��
	bool m_bErase;		// �����t���O
};


//**************************************************

class SheepManager
{
public:
	std::list<Sheep*> *Get_list(){ return &m_List; };
	std::list<FatSheep*> *GetFatList(){ return &m_FatList; }

	SheepManager();
	~SheepManager();

	void Update();
	void Render();

	void Set_pointers(StageManager *sp, DataManager *dmp){ this->sp = sp, this->dmp = dmp; };

	void Reset(){ for (auto it : m_List){ delete it; }m_List.clear(); }
	void Start(){ m_CurrentTime = clock(); }

	float FatSheepAccel(){ return m_TextParam.FatSheepAccel; }// �����Ă�r�̉����x

	// �q���H������Ăяo��
	void CreateFatSheep(Sheep *sheep)
	{
		sheep->Erase();	// ���̗r������
		FatSheep *set = new FatSheep(m_pFatSheepImage, *sheep->Get_pos() + Vector2(-32, -32));// �������r����
		set->SetFloor(sheep->Get_floor());	// �t���A�ݒ�
		m_FatList.push_back(set);			// ���X�g�ɓ˂�����
	}

private:
	//tdn2DObj *Getfile(int num){ return files[num]; }
	//tdn2DObj *files[(int)SHEEP_TYPE::MAX];
	tdn2DObj *m_pFatSheepImage, *ase;
	int m_CurveRange;		// ���ɑ΂��āA�Ȃ��ꖽ�ߏo���͈́B

	std::list<Sheep*> m_List;		// ���ʂ̗r���X�g
	std::list<FatSheep*> m_FatList;	// �������r���X�g
	void create(int floor);

	// ���o�߂��Ă鎞��(�~���b)
	UINT m_CurrentTime;

	// �e�L�X�g����̏��
	SheepTextParam m_TextParam;

	StageManager *sp;
	DataManager *dmp;
};