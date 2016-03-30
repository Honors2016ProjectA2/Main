#pragma once
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