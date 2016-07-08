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

	// EXIT
	struct ExitInfo
	{
		tdn2DObj *pImage;	// 画像
		Vector2 pos;		// 座標
		Vector2 size;		// 画像サイズ
		bool bPoint;		// カーソル範囲内か
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

	// チュートリアル看板
	static bool m_bTutorial;		// チュートリアルフラグ
	struct TutorialInfo
	{
		tdn2DObj *pImage;	// 画像
		Vector2 pos;		// 座標
		Vector2 size;		// 画像サイズ
		bool bPoint;		// カーソル範囲内か
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

	// マウスポインタ―
	MousePointer* m_pointer;

	// 背景
	tdn2DObj* m_BG;
	//tdn2DObj* m_BG_flont;

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

	tdnStreamSound *m_pStreamSound;

	tdn2DAnim* m_gameStart;
};


// 超高難易度ステージ
extern bool g_bExtraStage;