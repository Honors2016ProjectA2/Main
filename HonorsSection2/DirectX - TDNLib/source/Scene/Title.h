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

	// 犬
	struct 
	{
		tdn2DObj* pic;
		Vector2 pos;
		int anim;
		int animFlame;
	}m_dog;
	bool m_bDogFlag;

	// マウスポインタ―
	MousePointer* m_pointer;

	// 背景
	tdn2DObj* m_BG;

	// タイトル
	tdn2DObj* m_titleLogo;

	// 小屋
	struct
	{
		tdn2DObj* pic;
		tdn2DObj* picBack;
		Vector2 pos;
	}m_koya;
	bool m_bKoyaFlag;

	// ストリームプレイヤー
	tdnStreamSound *m_pStreamSound;
};