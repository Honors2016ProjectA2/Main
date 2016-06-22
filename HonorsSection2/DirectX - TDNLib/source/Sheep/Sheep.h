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
	tdn2DObj *CatchImage;	// �͂܂�摜
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
		bool isDead(){ return (process == MODE::CRUSHED); }
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

	SheepManager(bool bTitle = false);
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

	// �͂ݍX�V
	void CatchUpdate();

	// �^�C�g���p��
	bool m_bTitle;
};

// ����
extern SheepManager* g_pSheepMgr;
extern int g_CreateSheepFloor;










// ���U���g�ɂāA�Ǘ������X�̗r
class ResultSheep
{
public:
	enum MODE{ RUN, HAKERU };

	ResultSheep(const Vector2 &pos, float speed) :m_pos(pos), m_speed(speed), m_pMode(nullptr), m_bErase(false){ ChangeMode(MODE::RUN); };
	~ResultSheep(){ SAFE_DELETE(m_pMode); }
	void Update(){ m_pMode->Update(); }
	void Render(tdn2DObj *pImage){ m_pMode->Render(pImage); }

	// �����֘A
	bool EraseOK(){ return m_bErase; }
	void Erase(){ m_bErase = true; }

	void ChangeMode(MODE m)
	{
		if (m_pMode) delete m_pMode;
		switch (m)
		{
		case ResultSheep::RUN:
			m_pMode = new Mode::Run(this);
			break;
		case ResultSheep::HAKERU:
			m_pMode = new Mode::Hakeru(this);
			break;
		}
	}

	Vector2 &GetPos(){ return m_pos; }
private:

	Vector2 m_pos;	// ���W
	float m_speed;	// ���x
	bool m_bErase;	// �����t���O

	// ���[�h����p
	class Mode
	{
	public:
		class Base
		{
		protected:
			ResultSheep *m_pSheep;	// �e
			int m_AnimFrame, m_AnimPanel;	// �A�j���֌W
			float m_angle;
		public:
			Base(ResultSheep *pSheep) :m_pSheep(pSheep), m_AnimFrame(0), m_AnimPanel(rand() % 4),m_angle(0){}
			virtual void Update()
			{
				// ���ɂ�
				if (++m_AnimFrame > 7)
				{
					m_AnimFrame = 0;
					if (++m_AnimPanel >= 4)m_AnimPanel = 0;
				}
			}
			virtual void Render(tdn2DObj *pImage)
			{
				pImage->SetAngle(m_angle);
				pImage->Render((int)m_pSheep->m_pos.x, (int)m_pSheep->m_pos.y, 120, 120, m_AnimPanel * 120, 0, -120, 120);
			}
		};

		// ������E�ɑ���
		class Run:public Base
		{
		public:
			Run(ResultSheep *pSheep) :Base(pSheep){}
			void Update();
			//void Render(tdn2DObj *pImage);
		};

		// �|����
		class Hakeru:public Base
		{
		private:
			Vector2 m_vec;
		public:
			Hakeru(ResultSheep *pSheep);
			void Update();
			//void Render(tdn2DObj *pImage);
		};
	};

	// ���[�h�|�C���^
	Mode::Base *m_pMode;
};

class Number;

// ���U���g�p�r�Ǘ�
class ResultSheepManager
{
public:
	ResultSheepManager(float ScoreWakuY, float SohotaWakuY, float startX, float speed);
	~ResultSheepManager();

	void Update();
	void Render();

	// �X�R�A�̗r�X�^�[�g
	void StartScoreSheep();
	// �X�R�A���ڂ̗r���|����
	void HakeruScoreSheep(){ for (auto& it : m_ScoreSheepList)it->ChangeMode(ResultSheep::MODE::HAKERU); }

	// ���̑����ڂ̗r�X�^�[�g
	void StartOtherSheep();
	// ���̑����ڂ̗r���|����
	void HakeruOtherSheep(){ for (auto& it : m_OtherSheepList)it->ChangeMode(ResultSheep::MODE::HAKERU); }

	bool isScoreEnd(){ return m_waku[0].isSet; }
	bool isOtherEnd(){ return m_waku[1].isSet; }

	void Reset()
	{
		for (auto it = m_ScoreSheepList.begin(); it != m_ScoreSheepList.end();)
		{
			delete (*it);
			it = m_ScoreSheepList.erase(it);
		}
		for (auto it = m_OtherSheepList.begin(); it != m_OtherSheepList.end();)
		{
			delete (*it);
			it = m_OtherSheepList.erase(it);
		}
		m_waku[0].pos.x = m_waku[1].pos.x = -1280;
		m_waku[0].isSet = m_waku[1].isSet = false;
	}
private:
	// �X�R�A�g�p�̗r
	std::list <ResultSheep*> m_ScoreSheepList;

	// ���̑��g�p�̗r
	std::list <ResultSheep*> m_OtherSheepList;

	// �X�R�A�̃i���o�[
	enum NUMBER_TYPE{ SCORE, COMBO, WOLFKILL, SHEEPKILL, MAX };
	Number *m_pNumbers[NUMBER_TYPE::MAX];

	// �r�摜
	tdn2DObj *m_pSheepImage;

	// �g�摜
	struct Waku
	{
		tdn2DObj *pImage;
		Vector2 pos;
		bool isSet;
		Waku() :pos(-1280, 0), isSet(false){}
	}m_waku[2];	// [0]:�X�R�A�g, [1]:���̑��g

	// �g�r�̊J�n�n�_Y
	const float m_StartScoreY, m_StartOtherY;
	// �J�n���WX
	const float m_StartX;
	// �r�̑��x
	const float m_speed;

	// �g�̕�
	static const int WAKU_WIDTH = 756;
};