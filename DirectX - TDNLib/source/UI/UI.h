#pragma once

enum class HUKIDASHI_TYPE
{
	ORIGIN,		// 最初の流す人
	SUCCESS,	// 成功
	FAILED,		// 失敗
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
	tdn2DObj *m_pImage;		// 画像の実体(参照するだけなのでデリートはしない)
	Vector2 m_SrcXY;		// 画像取得座標(1枚の画像に複数の吹き出しがあるため)
	int m_AppTime;			// 出現時間
	Vector3 m_pos;			// 座標
	Hukidashi();
};




//*****************************************************************************************************************************
//
//		UIクラス(Singleton)
//
//*****************************************************************************************************************************

class UIManager
{
public:
	// 実体取得
	static UIManager &GetInstance();

	// いつもの
	void Initialize();
	~UIManager();
	void Release();
	void Update();
	void Render();

	// 吹き出しのセット
	void PushHukidashi(const Vector3 &pos, HUKIDASHI_TYPE success);

private:

	// 静的実体
	static UIManager* pInstance;

	// リストに突っ込むUIのデータ
	struct UIData
	{
		tdn2DObj *lpImage;	// 画像の実体
		Vector2 pos;		// 画像座標

		// コンストラクタとデストラクタ
		UIData() :lpImage(nullptr){}
		~UIData(){ SAFE_DELETE(lpImage); }
	};

	// UIの情報を格納しているリスト
	std::vector<UIData*> m_Datas;

	// 吹き出しの情報を格納しているリスト
	std::list<Hukidashi*> m_HukidashiList;

	// DATA/Text/UI/main.txtのIDと結びつける。m_Datas[UI_ID::NOKORI]->lpImage->Render()　と使う
	enum UI_ID
	{
		NOKORI				 = 0,	// 残り
		NOKORI_NUMBER		 = 1,	// 残りの回数
		BIKKURI				 = 2,	// ！マーク
		HUKIDASHI_MINE		 = 3,	// 自分が出す吹き出し
		HUKIDASHI_CORRECT	 = 4,	// 正解時の吹き出し
		HUKIDASHI_INCORRECT	 = 5	// 不正解
	};

	// 封印
	UIManager(){}
	UIManager(const UIManager&){}
	UIManager &operator=(const UIManager&){}

	// プチマクロ(m_Datas[UI_ID::])とかするのめんどくさい用
	void ID_Render(UI_ID id);
};

#define UIMgr (UIManager::GetInstance())