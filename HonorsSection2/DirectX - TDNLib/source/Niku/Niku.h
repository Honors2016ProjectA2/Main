#pragma once

//***********************************
//		�q���N���X

// �q���X�e�[�g
enum class YAKINIKU_MODE
{
	NAMA,		// ��
	RARE,		// ���A
	MEDIAM,		// �~�f�B�A��
	WELDAN,		// �E�F���_��
	KOGETA,		// �ł���
	MAX
};

// �O���錾
class Yakiniku;

// �q���Ϗ����[�h
namespace YakinikuMode
{
	class Base
	{
	protected:
		YAKINIKU_MODE m_NextMode;
		int m_timer, m_limit;		// �ڕW�ƍ��̎���
		int m_srcX;
		Yakiniku *m_pYakiniku;			// �Ϗ����̃|�C���^
	public:
		Base(Yakiniku *me) :m_pYakiniku(me), m_timer(0), m_limit(0){}
		virtual ~Base(){}
		virtual void Update();
		virtual void Render();
		virtual YAKINIKU_MODE GetMode() = 0;
		void NextMode();
		void SetChangeModeTime(int val){ m_limit = val; }
	};

	class Nama :public Base
	{
	public:
		Nama(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::NAMA; }
	};

	class Rare :public Base
	{
	public:
		Rare(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::RARE; }
	};

	class Mediam :public Base
	{
	public:
		Mediam(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::MEDIAM; }
	};

	class Weldan :public Base
	{
	public:
		Weldan(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::WELDAN; }
	};

	class Kogeta :public Base
	{
	private:
		int m_KiraKiraCoolTime;
	public:
		Kogeta(Yakiniku *me);
		void Update()override;
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::KOGETA; }
	};

}

// ���Ă��Ă��ԃN���X
class Yakiniku
{
public:
	Yakiniku(tdn2DObj *image);
	~Yakiniku();
	void Update();
	void Render();

	// ���[�h�ύX
	void ChangeMode(YAKINIKU_MODE m);

	// �Q�b�^�[
	YAKINIKU_MODE GetMode(){ return m_pMode->GetMode(); }
	int GetPosX(){ return (int)m_pos.x; }
	int GetPosY(){ return (int)m_pos.y; }
	Vector2 &GetPos(){ return m_pos; }
	Vector2 &GetCenterPos(){ return m_pos + Vector2(64, 64); }

	// ����
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

	// �q���̉摜(�Ϗ��|�C���^����A�N�Z�X)
	tdn2DObj *pImage;


private:

	// �q���̃��[�h�Ϗ��|�C���^
	YakinikuMode::Base *m_pMode;

	Vector2 m_pos;	// ���W
	bool m_bErase;
};


// �������ꂽ���̃N���X
class Niku
{
public:
	Niku(const Vector2 &pos, YAKINIKU_MODE type, tdn2DObj *image);
	void Update();
	void Render();

	void SetPos(const Vector2 &pos){ m_pos = pos; }
	void SetPosY(float y){ m_pos.y = y; }
	Vector2 GetCenterPos(){ return Vector2(m_pos.x + 60, m_pos.y + 60); };
	int GetFloor(){ return m_floor; }
	void SetFloor(int in){ m_floor = in; }
	YAKINIKU_MODE GetType(){ return m_type; }

	// �����֘A
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

	// ���ݒu�n
	bool isSeted(){ return m_bSet; }
	void Set(){ m_bSet = true; }
	void UnSet(){ m_bSet = false; }

private:
	int m_floor;
	YAKINIKU_MODE m_type;
	tdn2DObj *m_pImage;
	Vector2 m_pos;
	Vector2 m_move;
	Vector2 m_BoundPow;
	bool m_bErase;
	float m_orgY;
	float m_gravity;
	bool m_bSet;		// �z�u�������ǂ���
};


// �����Ǘ�����N���X
class NikuManager
{
public:
	static NikuManager *GetInstance()
	{
		if (!pInstance) pInstance = new NikuManager;
		return pInstance;
	}

	~NikuManager();

	void Initialize();
	void Release();
	void Update();
	void Render();

	// �����
	void StartYakiniku();
	void CreateNiku();

	// �Q�b�^
	Yakiniku *GetYakiniku(){ return m_pYakiniku; }
	Niku *GetNiku(){ return m_pNiku; }

	// ��UI�}�l�[�W���[�ŌĂяo��
	float GetGaugePercentage(){ return (float)m_CreateTimer / m_CREATETIME; }	// �q���Q�[�W��percent

private:
	// 1�����Ȃ�����
	static NikuManager *pInstance;

	// �摜
	tdn2DObj *m_pIkenieImage, *m_pNikuImage;

	// �ē����
	Yakiniku* m_pYakiniku;

	// �������������
	Niku* m_pNiku;

	bool m_bHoldNiku;

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

	// �V���O���g���̍�@
	NikuManager();
	NikuManager(const NikuManager&){}
	NikuManager &operator=(const NikuManager&){}
};

#define NikuMgr (NikuManager::GetInstance())