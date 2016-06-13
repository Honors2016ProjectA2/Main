#pragma once

//***********************************
//		�q���N���X

// �q���X�e�[�g
enum class BOKUSOU_MODE
{
	HUTABA,		// �o�t��
	HONBA,		// �{�t����
	TSUBOMI,	// �ڂ�(Absolute Nine)
	SAITA,		// �ԍ炢��(�����l�^�؂�)
	BORN,		// �q�������܂ꂽ�B���̒i�K�ŗr���E���ƃp���[�A�b�v
	MAX
};

// �O���錾
class Bokusou;

// �q���Ϗ����[�h
namespace BokusouMode
{
	class Base
	{
	protected:
		BOKUSOU_MODE m_NextMode;
		tdn2DObj *m_pImage;			// �摜
		int m_timer, m_limit;		// �ڕW�ƍ��̎���
		int m_srcX;
		Bokusou *pBokusou;			// �Ϗ����̃|�C���^
	public:
		Base(Bokusou *me, tdn2DObj *image) :pBokusou(me), m_pImage(image), m_timer(0), m_limit(0){}
		virtual ~Base(){}
		virtual void Update();
		virtual void Render();
		virtual BOKUSOU_MODE GetMode() = 0;
		void NextMode();
		void SetChangeModeTime(int val){ m_limit = val; }
	};

	class Hutaba:public Base
	{
	public:
		Hutaba(Bokusou *me, tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::HUTABA; }
	};

	class Honba :public Base
	{
	public:
		Honba(Bokusou *me, tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::HONBA; }
	};

	class Tsubomi :public Base
	{
	public:
		Tsubomi(Bokusou *me, tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::TSUBOMI; }
	};

	class Saita :public Base
	{
	public:
		Saita(Bokusou *me, tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::SAITA; }
	};

	class Born :public Base
	{
	private:
		int m_KiraKiraCoolTime;
	public:
		Born(Bokusou *me, tdn2DObj *image);
		void Update()override;
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::BORN; }
	};

}

// �q���N���X
class Bokusou
{
public:
	Bokusou(const Vector2 &pos, int point);
	~Bokusou();
	void Update();
	void Render();

	// ���[�h�ύX
	void ChangeMode(BOKUSOU_MODE m);

	// �Z�b�^�[
	void SetFloor(int val){ m_floor = val; }

	// �Q�b�^�[
	BOKUSOU_MODE GetMode(){ return m_pMode->GetMode(); }
	int GetPosX(){ return (int)m_pos.x; }
	int GetPosY(){ return (int)m_pos.y; }
	Vector2 &GetPos(){ return m_pos; }
	Vector2 &GetCenterPos(){ return m_pos + Vector2(64, 64); }
	int GetFloor(){ return m_floor; }
	int GetPoint(){ return m_point; }

	// ����
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

private:

	// �q���̉摜
	tdn2DObj *m_pBokusouFlower, *m_pBokusouRoll;

	// �q���̃��[�h�Ϗ��|�C���^
	BokusouMode::Base *m_pMode;

	Vector2 m_pos;	// ���W
	int m_floor;
	bool m_bErase;
	int m_point;
};

class BokusouManager;

class BokusouTarget
{
public:
	BokusouTarget();
	~BokusouTarget();
	void Update(BokusouManager *pMgr);
	void Render();

	void ChangePos(const Vector2 &pos);
private:
	tdn2DAnim *m_pTargetAnim;
	int m_AnimFrame;
	const int ANIM_FRAME;
	tdn2DObj *m_pKusa;
	Vector2 m_pos;
	float m_angle;
	float m_scale;
	BYTE m_alpha;
	int m_srcX;
};


// �q�����Ǘ�����N���X
class BokusouManager
{
public:
	static BokusouManager *GetInstance()
	{
		if (!pInstance) pInstance = new BokusouManager;
		return pInstance;
	}

	~BokusouManager();

	void Initialize();
	void Release();
	void Update();
	void Render();

	// �Q�b�^
	std::list<Bokusou*> *GetList(){ return &m_list; }

	// ��UI�}�l�[�W���[�ŌĂяo��
	float GetGaugePercentage(){ return (float)m_CreateTimer / m_CREATETIME; }	// �q���Q�[�W��percent

	// �x�W�G���񂩂�Ăяo��
	void CreateByBazier();

private:
	// 1�����Ȃ�����
	static BokusouManager *pInstance;

	// ���X�g
	std::list<Bokusou*> m_list;

	BokusouTarget *m_pBokusouTarget;

	struct CreatePos
	{
		Vector2 pos;
		int floor;		// �����邽�߂����ɍ\����
		CreatePos(const Vector2 &pos, int floor) :pos(pos), floor(floor){}
	};
	std::vector<CreatePos> m_CreatePosList;	// �����_���ɐ����������W����
	int m_CreateTimer;					// �q�������^�C�}�[
	int m_CREATETIME;					// ��������鎞��
	int m_NextPoint;					// ��������鎟�̃|�C���g
	int m_PrevPoint;

	// �V���O���g���̍�@
	BokusouManager();
	BokusouManager(const BokusouManager&){}
	BokusouManager &operator=(const BokusouManager&){}
};


#define BokusouMgr (BokusouManager::GetInstance())