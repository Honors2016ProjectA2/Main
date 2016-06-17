#pragma once

//***********************************
//		�q���N���X
// �q���X�e�[�g
enum class YAKINIKU_MODE
{
	NAMA,		// ��
	RARE,		// ���A
	MEDIAM,		// �~�f�B�A��
	KOGETA,		// �ł���
	MAX
};

// �O���錾
class Yakiniku;
enum class SHEEP_TYPE;

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
		void RenderFront();
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

	//class Weldan :public Base
	//{
	//public:
	//	Weldan(Yakiniku *me);
	//	//void Update(Niku *pNiku);
	//	//void Render(Niku *pNiku);
	//	YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::WELDAN; }
	//};

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
	Yakiniku(tdn2DObj *image, SHEEP_TYPE SheepType);
	~Yakiniku();
	void Update();
	void Render();
	void RenderFront();

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

	SHEEP_TYPE GetSheepType(){ return m_SheepType; }

	// �q���̉摜(�Ϗ��|�C���^����A�N�Z�X)
	tdn2DObj *pImage;


private:

	// �q���̃��[�h�Ϗ��|�C���^
	YakinikuMode::Base *m_pMode;

	Vector2 m_pos;	// ���W
	bool m_bErase;

	SHEEP_TYPE m_SheepType;	// �Ă��Ă�r�̃^�C�v
};


// �������ꂽ���̃N���X
class Niku
{
public:
	Niku(const Vector2 &pos, YAKINIKU_MODE type, tdn2DObj *image, SHEEP_TYPE SheepType);
	void Update();
	void Render();

	void SetPos(const Vector2 &pos){ m_pos = pos; }
	void SetPosY(float y){ m_pos.y = y; }
	Vector2 GetCenterPos(){ return Vector2(m_pos.x + 60, m_pos.y + 60); };
	int GetFloor(){ return m_floor; }
	void SetFloor(int in){ m_floor = in; }
	YAKINIKU_MODE GetType(){ return m_type; }
	void MoveOff(){ m_BoundPow = m_move = Vector2(0, 0); }

	// �����֘A
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

	// ���ݒu�n
	bool isSeted(){ return m_bSet; }
	void Set(){ m_bSet = true; }
	void UnSet(){ m_bSet = false; }

	// �x�W�G�Ŏg��
	bool isVisible(){ return m_bVisible; }
	void SetVisible(){ m_bVisible = true; m_bSet = false; }

	SHEEP_TYPE GetSheepType(){ return m_SheepType; }

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
	bool m_bVisible;
	SHEEP_TYPE m_SheepType;	// �Ă��Ă�r�̃^�C�v
};

#include "../UI/SendPower/SendPower.h"

// ���x�W�G
class NikuBazier : public SendPower
{
public:
	NikuBazier(char* filename, Vector3 startPos, Vector3 centerPos,
		Vector3 center2Pos, Vector3 endPos, int endFlame, int power = 10) :SendPower(filename, startPos, centerPos,
		center2Pos, endPos, endFlame, power, 0){}

	void Move()override;
};

// �����Ǘ�����N���X
class NikuManager
{
public:
	static NikuManager *GetInstance();

	~NikuManager();

	void Initialize();
	void Release();
	void Update();
	void RenderYakiniku();
	void RenderNiku();

	// �����
	void StartYakiniku(SHEEP_TYPE type);
	void StartNikuBazier();
	void CreateNiku();

	// �Q�b�^
	Yakiniku *GetYakiniku(){ return m_pYakiniku; }
	Niku *GetNiku(){ return m_pNiku; }

	// ��UI�}�l�[�W���[�ŌĂяo��
	float GetGaugePercentage(){ return (float)m_CreateTimer / m_CREATETIME; }	// �q���Q�[�W��percent

	bool isNikuHold(){ return m_bHoldNiku; }
private:
	// 1�����Ȃ�����
	static NikuManager *pInstance;

	// �摜
	tdn2DObj **m_pIkenieImages, **m_pNikuImages;

	// �ē����
	Yakiniku* m_pYakiniku;

	// �������������
	Niku* m_pNiku;

	// ���H�����޼޴
	NikuBazier *m_pNikuBazier;

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