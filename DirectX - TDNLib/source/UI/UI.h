#pragma once

enum class HUKIDASHI_TYPE
{
	ORIGIN,		// �ŏ��̗����l
	SUCCESS,	// ����
	FAILED,		// ���s
};

class Hukidashi
{
public:
	Hukidashi(tdn2DObj *lpImage, const Vector2 &srcXY, int AppTime, const Vector3 &pos) :m_pImage(lpImage), m_SrcXY(srcXY), m_AppTime(AppTime), m_pos(pos) {}
	void Update(){ m_AppTime--; }
	void Render();

	bool EraseOK(){ return (m_AppTime <= 0); }

	static const int DEFAULT_APP_TIME = 120;

private:
	tdn2DObj *m_pImage;		// �摜�̎���(�Q�Ƃ��邾���Ȃ̂Ńf���[�g�͂��Ȃ�)
	Vector2 m_SrcXY;		// �摜�擾���W(1���̉摜�ɕ����̐����o�������邽��)
	int m_AppTime;			// �o������
	Vector3 m_pos;			// ���W
	Hukidashi();
};




//*****************************************************************************************************************************
//
//		UI�N���X(Singleton)
//
//*****************************************************************************************************************************

class UIManager
{
public:
	// ���̎擾
	static UIManager &GetInstance();

	// ������
	void Initialize();
	~UIManager();
	void Release();
	void Update();
	void Render();

	// �����o���̃Z�b�g
	void PushHukidashi(const Vector3 &pos, HUKIDASHI_TYPE success);

private:

	// �ÓI����
	static UIManager* pInstance;

	// ���X�g�ɓ˂�����UI�̃f�[�^
	struct UIData
	{
		tdn2DObj *lpImage;	// �摜�̎���
		Vector2 pos;		// �摜���W

		// �R���X�g���N�^�ƃf�X�g���N�^
		UIData() :lpImage(nullptr){}
		~UIData(){ SAFE_DELETE(lpImage); }
	};

	// UI�̏����i�[���Ă��郊�X�g
	std::vector<UIData*> m_Datas;

	// �����o���̏����i�[���Ă��郊�X�g
	std::list<Hukidashi*> m_HukidashiList;

	// DATA/Text/UI/main.txt��ID�ƌ��т���Bm_Datas[UI_ID::NOKORI]->lpImage->Render()�@�Ǝg��
	enum UI_ID
	{
		NOKORI				 = 0,	// �c��
		NOKORI_NUMBER		 = 1,	// �c��̉�
		BIKKURI				 = 2,	// �I�}�[�N
		HUKIDASHI_MINE		 = 3,	// �������o�������o��
		HUKIDASHI_CORRECT	 = 4,	// �������̐����o��
		HUKIDASHI_INCORRECT	 = 5	// �s����
	};

	// ����
	UIManager(){}
	UIManager(const UIManager&){}
	UIManager &operator=(const UIManager&){}

	// �v�`�}�N��(m_Datas[UI_ID::])�Ƃ�����̂߂�ǂ������p
	void ID_Render(UI_ID id);
};

#define UIMgr (UIManager::GetInstance())