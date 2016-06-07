#pragma once
#include "IEX_Expansion.h"
class StageManager;
class DataManager;
#include "../CharaBase/CharaBase.h"

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
	tdn2DObj *BoneImage;	// ���񂾍�
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

namespace Sheep
{
	class Base
	{
	protected:
		int frame, animeframe, m_AnimePanel;
		Vector2 pos, animepos,
			move,			// �J�[�u�Ƃ��ŁA���Ƃ��瑫���悤�̈ړ��l
			C_MOVE;			// �{�������Ă���Œ�̈ړ��l
		DIR m_CurveDir;		// �J�[�u�̌���
		bool m_bTurned;		// �T�C���Œ��p�ɋȂ�����A�߂��ɕK�v
		float m_sinAngle;	// �J�[�u���T�C���J�[�u�ł���̂ŁA����p�̕ϐ�
		bool m_bErase;		// �����t���O
		bool m_bLeftCatch;	// ���N���b�N�ł��񂾂�

		//const int PNGSIZE;
		int m_floor;

		// �r�̏��
		SheepData m_data;

		enum MODE{
			WALK,		// ���ʂ̕���
			CURVE,		// �����]��
			PUSH,		// �f�u����
			CRUSHED,	// �H��ꂽ
			CAUGHT,		// �߂܂���
			MAX
		};
		int process;
		BYTE alpha;

		void Get_frame_pos(Vector2 &pos, int frame);

		struct
		{
			tdn2DObj *ase;
			int animex;

		}ase;

		void Get_out();
		void Walk();
		void Curve();
		void Push();
		void Crushed();
		void Caught();
		void(Base::*Move[MODE::MAX])();
	public:
		Base(const SheepData &data, int floor, const SheepTextParam &textparam);
		virtual ~Base();

		virtual void Update();
		virtual void Render();

		/* ���� */
		Vector2 *Get_pos(){ return &pos; }
		/* ������ */
		Vector2 GetCenterPos(){ return pos + Vector2(60, 60); }
		void Get_pos2(Vector2 &out)
		{
			out = pos;
			out.x += m_data.SIZE * 0.5f;
			out.y += m_data.SIZE;
		}
		void SetPos(const Vector2 &p){ pos = p; }
		void SetPosX(float x){ pos.x = x; }

		int Get_size(){ return m_data.SIZE; }
		int Get_floor(){ return m_floor; }
		void Be_Walk(){ if(process == MODE::PUSH) process = MODE::WALK; }
		void Be_Push(){ if(process == MODE::WALK) process = MODE::PUSH; }
		void Be_crushed();
		void Be_caught(bool bLeft);
		bool isPushOK(){ return (process == MODE::WALK || process == MODE::PUSH); }
		bool isCaught(){ return (process == MODE::CAUGHT); }
		void SetCurve(DIR dir)
		{
			if (process != MODE::WALK) return;
			m_CurveDir = dir; process = MODE::CURVE; m_sinAngle = 0; m_bTurned = false;
		}
		float TokutenBairitsu(){ return m_data.magnification; }

		// �����֌W
		void Erase(){ m_bErase = true; }
		bool EraseOK(){ return m_bErase; }

		// �^�C�v�擾
		virtual SHEEP_TYPE GetType() = 0;

		bool col_check;
	};

	class Normal:public Base
	{
	private:
	public:
		Normal(const SheepData &data, int floor, const SheepTextParam &textparam) :Base(data, floor, textparam){}
		SHEEP_TYPE GetType(){ return SHEEP_TYPE::NOMAL; }
	};

	class Gold:public Base
	{
	private:
		int m_seID;
	public:
		Gold(const SheepData &data, int floor, const SheepTextParam &textparam);
		~Gold();
		void Update();
		SHEEP_TYPE GetType(){ return SHEEP_TYPE::GOLD; }
	};

	class Real:public Base
	{
	private:
	public:
		Real(const SheepData &data, int floor, const SheepTextParam &textparam) :Base(data, floor, textparam){}
		void Update();
		SHEEP_TYPE GetType(){ return SHEEP_TYPE::REAL; }
	};
}

// �q���H���đ������r
class FatSheep : public DebuBase
{
public:
	FatSheep(tdn2DObj *image, const Vector2 &pos, SHEEP_TYPE type);
	~FatSheep();

	SHEEP_TYPE GetType(){ return m_type; }

private:
	SHEEP_TYPE m_type;
};


//**************************************************
// �O���[�o���̈�
extern int g_CreateSheepFloor;		// �r�𐶐�����t���A

class SheepManager
{
public:
	std::list<Sheep::Base*> *Get_list(){ return &m_List; };
	std::list<FatSheep*> *GetFatList(){ return &m_FatList; }

	SheepManager();
	~SheepManager();

	void Update();
	void Render();
	void RenderFat();

	void Set_pointers(StageManager *sp, DataManager *dmp){ this->sp = sp, this->dmp = dmp; };

	void Reset(){ for (auto it : m_List){ delete it; }m_List.clear(); }

	float FatSheepAccel(){ return m_TextParam.FatSheepAccel; }// �����Ă�r�̉����x

	// �q���H������Ăяo��
	void CreateFatSheep(Sheep::Base *sheep);

	// �Q�b�^
	int GetOpenFloor(){ return g_CreateSheepFloor; }

	//	�^�C�g���Ŏg���p
	void SetDownLane() { g_CreateSheepFloor = m_NextChangeFloor = 2; }
	SheepTextParam *GetSheepTextParam() { return &m_TextParam; }

private:
	//tdn2DObj *Getfile(int num){ return files[num]; }
	tdn2DObj *m_pFatSheepImages[(int)SHEEP_TYPE::MAX];
	tdn2DObj *m_pBoneImage;
	int m_CurveRange;		// ���ɑ΂��āA�Ȃ��ꖽ�ߏo���͈́B

	std::list<Sheep::Base*> m_List;		// ���ʂ̗r���X�g
	std::list<FatSheep*> m_FatList;	// �������r���X�g
	void create(int floor);

	int m_NextChangeFloor;				// �ς������̃��[��
	int MakeNextFloor(int current);		// �O�񃌁[���ł͂Ȃ����[���������_���ɍ쐬����֐�

	// ���o�߂��Ă鎞��(�t���[��)
	int m_CreateFrame;

	// �e�L�X�g����̏��
	SheepTextParam m_TextParam;

	StageManager *sp;
	DataManager *dmp;
};

// ����
extern SheepManager* g_pSheepMgr;
extern int g_CreateSheepFloor;