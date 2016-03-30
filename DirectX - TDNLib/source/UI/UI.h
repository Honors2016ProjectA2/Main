#pragma once
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