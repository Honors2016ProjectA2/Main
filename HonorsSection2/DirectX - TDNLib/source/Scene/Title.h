#pragma once

class SheepManager;
class MousePointer;

class Title : public BaseScene
{
public:
	~Title();
	bool Initialize();
	bool Update();
	void DogUpdate();
	void DogVsMouse();
	void DogVsSheep();

	void KoyaVsSheep();
	void Render();

private:
	SheepManager *m_pSheepMgr;

	// ��
	struct 
	{
		tdn2DObj* pic;
		Vector2 pos;
		int anim;
		int animFlame;
	}m_dog;
	bool m_bDogFlag;

	// EXIT
	struct ExitInfo
	{
		tdn2DObj *pImage;	// �摜
		Vector2 pos;		// ���W
		Vector2 size;		// �摜�T�C�Y
		bool bPoint;		// �J�[�\���͈͓���
		ExitInfo(char *filename, const Vector2 &pos, const Vector2 &size) :pImage(new tdn2DObj(filename)), pos(pos), size(size), bPoint(false){}
		~ExitInfo(){delete pImage; }
		void Render()
		{
			pImage->SetARGB(bPoint ? 0xffffffff : 0xffa0a0a0);
			pImage->SetScale(bPoint ? 1.1f: 1.0f);
			pImage->Render((int)pos.x, (int)pos.y);
		}
		bool CheckCurosrIn(const Vector2 &mPos){ return (mPos.x > pos.x && mPos.x < pos.x + size.x && mPos.y > pos.y && mPos.y < pos.y + size.y); }
	}*m_pExit;

	// �`���[�g���A���Ŕ�
	static bool m_bTutorial;		// �`���[�g���A���t���O
	struct TutorialInfo
	{
		tdn2DObj *pImage;	// �摜
		Vector2 pos;		// ���W
		Vector2 size;		// �摜�T�C�Y
		bool bPoint;		// �J�[�\���͈͓���
		TutorialInfo(char *filename, const Vector2 &pos, const Vector2 &size) :pImage(new tdn2DObj(filename)), pos(pos), size(size), bPoint(false){}
		~TutorialInfo(){ delete pImage; }
		void Render()
		{
			pImage->SetARGB(bPoint ? 0xffffffff : 0xffcccccc);
			pImage->SetScale(bPoint ? 1.1f : 1.0f);
			pImage->Render((int)pos.x, (int)pos.y, (int)size.x, (int)size.y, m_bTutorial ? 150 : 0, 0, 150, 150);
		}
		bool CheckCurosrIn(const Vector2 &mPos){ return (mPos.x > pos.x && mPos.x < pos.x + size.x && mPos.y > pos.y && mPos.y < pos.y + size.y); }
	}*m_pTutorial;

	// �}�E�X�|�C���^�\
	MousePointer* m_pointer;

	// �w�i
	tdn2DObj* m_BG;
	//tdn2DObj* m_BG_flont;

	// �^�C�g��
	tdn2DObj* m_titleLogo;

	// ����
	struct
	{
		tdn2DObj* pic;
		tdn2DObj* picBack;
		Vector2 pos;
	}m_koya;
	bool m_bKoyaFlag;

	tdnStreamSound *m_pStreamSound;

	tdn2DAnim* m_gameStart;
};


// ������Փx�X�e�[�W
extern bool g_bExtraStage;