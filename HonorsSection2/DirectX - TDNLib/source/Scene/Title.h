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

	// �}�E�X�|�C���^�\
	MousePointer* m_pointer;

	// �w�i
	tdn2DObj* m_BG;

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

	// �X�g���[���v���C���[
	tdnStreamSound *m_pStreamSound;
};