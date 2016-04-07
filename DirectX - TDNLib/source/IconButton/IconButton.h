#pragma once

//*****************************************************************************************************************************
//
//		�����̃{�^��
//
//*****************************************************************************************************************************

// �{�^���L���E�����t���O�萔
enum class EN_DIS_TYPE
{
	ENABLE,				// �ғ�������
	DISABLE,			// ���ʂɕ`�悳��邪�g���Ȃ�
	DISABLE_BLACK,		// �g���Ȃ��I�ȃ��b�Z�[�W
	DISABLE_WHITE,		// �����I�𒆓I�ȃ��b�Z�[�W
	DISABLE_VANISH		// ������
};

class IconButton
{
public:
	// �J�[�\�����͈͓��ɓ������Ƃ��̃A�N�V����
	enum IN_ACTION
	{
		UP_SCALE = 0x1,		// �X�P�[���������グ��
		PUT_WHITE = 0x1 << 1	// ����������悹����
	};

	// �������E����E�X�V�E�`��
	IconButton() :m_pCollision(nullptr), m_EnDisType(EN_DIS_TYPE::ENABLE), m_SEReceive(-1){}
	void Initialize(int ID, char *TexPath, int dstX, int dstY, BYTE InAction, char* se_ID);
	~IconButton();
	void Update(const Vector2 &CursorPos);
	void Render();

	// �{�^���̗L���E�����̐ݒ�
	void SetEnable(EN_DIS_TYPE type)
	{
		m_EnDisType = type;

		// ����������Ƃ���m_In(�J�[�\�����͈͓��ɓ����Ă��邩�̃t���O)��OFF�ɂ���
		if(type != EN_DIS_TYPE::ENABLE) m_In = false;
	}

	// �Q�b�^�E�Z�b�^
	bool isIn(){ return m_In; }
	int GetID(){ return m_ID; }


	// ����̐ݒ�(�o�^)
	void SetCollisionSqure(const Vector2 MinPos, const Vector2 MaxPos)
	{
		SAFE_DELETE(m_pCollision);
		m_pCollision = new Collision2D::Squre(MinPos, MaxPos);
	}
	void SetCollisionCircle(const Vector2 CenterPos, float Radius)
	{
		SAFE_DELETE(m_pCollision);
		m_pCollision = new Collision2D::Circle(CenterPos, Radius);
	}

private:
	int m_ID;				// �ԍ�
	int m_dstX;				// �摜�����W
	int m_dstY;				// �摜����W
	//int m_dstW;				// �摜�E���W
	//int m_dstH;				// �摜�����W
	//int m_srcW;
	//int m_srcH;
	bool m_In;				// �͈͓�
	BYTE m_InActionFlag;		//
	tdn2DObj *m_pButton;		// �摜����
	char m_SE_ID[64];			// �͈͓��ɓ������ۂ�SE��ID
	EN_DIS_TYPE m_EnDisType;	// �L�����ǂ���

	int m_SEReceive;		// 1�t���[������SE���Ă΂Ȃ��悤�Ɏg���p


	// ����Ϗ��N���X
	class Collision2D
	{
	public:
		class Base
		{
		public:
			virtual bool Collision(const Vector2 &pos) = 0;
			virtual void RenderDebug() = 0;
		};

		class Squre : public Base
		{
		private:
			Vector2 m_MinPos;	// ����
			Vector2 m_MaxPos;	// �E��
			Squre(){}			// �f�t�H���g�R���X�g���N�^����
		public:
			Squre(const Vector2 MinPos, const Vector2 MaxPos) :m_MinPos(MinPos), m_MaxPos(MaxPos){}
			bool Collision(const Vector2 &pos);
			void RenderDebug();
		};

		class Circle : public Base
		{
		private:
			Vector2 m_CenterPos;	// �~�̒��S
			float m_Radius;			// ���a
			Circle(){}				// �f�t�H���g�R���X�g���N�^����
		public:
			Circle(const Vector2 CenterPos, float Radius) :m_CenterPos(CenterPos), m_Radius(Radius){}
			bool Collision(const Vector2 &pos);
			void RenderDebug();
		};
	};
	Collision2D::Base *m_pCollision;			// ����Ϗ�����

};

class IconButtonManager
{
public:

	static const int NOT_IN_BUTTON = -1;	// �{�^���͈͓����Ȃ�����

	// �������E����E�X�V�E�`��
	IconButtonManager();
	~IconButtonManager();
	void Update(const Vector2 &CursorPos);	// �����ɂ́A�}�E�X���̃J�[�\���̍��W������
	void Render();

	// �f�[�^�ǉ�
	void TextLoad(char *filename);

	// �͈͓��ł���΁A���̉摜���v�b�V�������Ƃ��ɕԂ��Ă������l���Ԃ��Ă���B�����͈͓��łȂ����-1
	int GetInButtonNo();

	// Enable,Disable�B�{�^���̗L�����Ɩ�����
	void SetEnDis(UINT ID, EN_DIS_TYPE type);

	// ���X�g����
	void Clear();

private:
	std::vector<IconButton*> m_List;	// �{�^�����i�[�ϒ��z��
};