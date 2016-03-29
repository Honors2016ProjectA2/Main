#pragma once

//*****************************************************************************************************************************
//
//		�����̃{�^��
//
//*****************************************************************************************************************************

enum class EN_DIS_TYPE
{
	ENABLE,				// �ғ�������
	DISABLE_BLACK,		// �g���Ȃ��G�ȃ��b�Z�[�W
	DISABLE_WHITE,		// �����I�𒆓I�ȃ��b�Z�[�W
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

	IconButton() :m_pCollision(nullptr), m_EnDisType(EN_DIS_TYPE::ENABLE){}
	void Initialize(int ID, char *TexPath, int dstX, int dstY, BYTE InAction, char* se_ID);
	~IconButton();
	void Update(const Vector2 &CursorPos);
	void Render();
	// �����̍��W�������͈͓̔��ł��邩��Ԃ�
	//bool CheckPos(const Vector2 &CursorPos){ return(CursorPos.x >= m_dstX && CursorPos.x <= m_dstY&&CursorPos.y >= m_dstY && CursorPos.y <= m_dstH); }

	void SetEnable(EN_DIS_TYPE type)
	{
		m_EnDisType = type;
		m_In = false;
	}
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
			Squre();
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
			Circle(){}
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

	IconButtonManager();
	~IconButtonManager();
	void Update(const Vector2 &CursorPos);	// �����ɂ́A�}�E�X���̃J�[�\���̍��W������
	void Render();

	// �f�[�^�ǉ�
	//int Push(char *TexPath, int dstX, int dstY, BYTE InAction, char *se_ID = nullptr);
	void TextLoad(char *filename);

	// �͈͓��ł���΁A���̉摜���v�b�V�������Ƃ��ɕԂ��Ă������l���Ԃ��Ă���B�����͈͓��łȂ����
	int GetInButtonNo();

	void SetEnDis(UINT ID, EN_DIS_TYPE type);	// Enable,Disable�B�{�^���̗L�����Ɩ�����

	// ���X�g����
	void Clear();

private:
	std::vector<IconButton*> m_List;
	int m_Number;
};