#pragma once

//***********************************
//		牧草クラス

// 牧草ステート
enum class YAKINIKU_MODE
{
	NAMA,		// 生
	RARE,		// レア
	MEDIAM,		// ミディアム
	WELDAN,		// ウェルダン
	KOGETA,		// 焦げた
	MAX
};

// 前方宣言
class Yakiniku;

// 牧草委譲モード
namespace YakinikuMode
{
	class Base
	{
	protected:
		YAKINIKU_MODE m_NextMode;
		int m_timer, m_limit;		// 目標と今の時間
		int m_srcX;
		Yakiniku *m_pYakiniku;			// 委譲元のポインタ
	public:
		Base(Yakiniku *me) :m_pYakiniku(me), m_timer(0), m_limit(0){}
		virtual ~Base(){}
		virtual void Update();
		virtual void Render();
		virtual YAKINIKU_MODE GetMode() = 0;
		void NextMode();
		void SetChangeModeTime(int val){ m_limit = val; }
	};

	class Nama :public Base
	{
	public:
		Nama(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::NAMA; }
	};

	class Rare :public Base
	{
	public:
		Rare(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::RARE; }
	};

	class Mediam :public Base
	{
	public:
		Mediam(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::MEDIAM; }
	};

	class Weldan :public Base
	{
	public:
		Weldan(Yakiniku *me);
		//void Update(Niku *pNiku);
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::WELDAN; }
	};

	class Kogeta :public Base
	{
	private:
		int m_KiraKiraCoolTime;
	public:
		Kogeta(Yakiniku *me);
		void Update()override;
		//void Render(Niku *pNiku);
		YAKINIKU_MODE GetMode(){ return YAKINIKU_MODE::KOGETA; }
	};

}

// 肉焼いてる状態クラス
class Yakiniku
{
public:
	Yakiniku(tdn2DObj *image);
	~Yakiniku();
	void Update();
	void Render();

	// モード変更
	void ChangeMode(YAKINIKU_MODE m);

	// ゲッター
	YAKINIKU_MODE GetMode(){ return m_pMode->GetMode(); }
	int GetPosX(){ return (int)m_pos.x; }
	int GetPosY(){ return (int)m_pos.y; }
	Vector2 &GetPos(){ return m_pos; }
	Vector2 &GetCenterPos(){ return m_pos + Vector2(64, 64); }

	// 消去
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

	// 牧草の画像(委譲ポインタからアクセス)
	tdn2DObj *pImage;


private:

	// 牧草のモード委譲ポインタ
	YakinikuMode::Base *m_pMode;

	Vector2 m_pos;	// 座標
	bool m_bErase;
};


// 生成された肉のクラス
class Niku
{
public:
	Niku(const Vector2 &pos, YAKINIKU_MODE type, tdn2DObj *image);
	void Update();
	void Render();

	void SetPos(const Vector2 &pos){ m_pos = pos; }
	void SetPosY(float y){ m_pos.y = y; }
	Vector2 GetCenterPos(){ return Vector2(m_pos.x + 60, m_pos.y + 60); };
	int GetFloor(){ return m_floor; }
	void SetFloor(int in){ m_floor = in; }
	YAKINIKU_MODE GetType(){ return m_type; }

	// 消去関連
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

	// 肉設置系
	bool isSeted(){ return m_bSet; }
	void Set(){ m_bSet = true; }
	void UnSet(){ m_bSet = false; }

private:
	int m_floor;
	YAKINIKU_MODE m_type;
	tdn2DObj *m_pImage;
	Vector2 m_pos;
	Vector2 m_move;
	Vector2 m_BoundPow;
	bool m_bErase;
	float m_orgY;
	float m_gravity;
	bool m_bSet;		// 配置したかどうか
};


// 肉を管理するクラス
class NikuManager
{
public:
	static NikuManager *GetInstance()
	{
		if (!pInstance) pInstance = new NikuManager;
		return pInstance;
	}

	~NikuManager();

	void Initialize();
	void Release();
	void Update();
	void Render();

	// 肉作る
	void StartYakiniku();
	void CreateNiku();

	// ゲッタ
	Yakiniku *GetYakiniku(){ return m_pYakiniku; }
	Niku *GetNiku(){ return m_pNiku; }

	// ★UIマネージャーで呼び出す
	float GetGaugePercentage(){ return (float)m_CreateTimer / m_CREATETIME; }	// 牧草ゲージのpercent

private:
	// 1つしかない実体
	static NikuManager *pInstance;

	// 画像
	tdn2DObj *m_pIkenieImage, *m_pNikuImage;

	// 焼肉状態
	Yakiniku* m_pYakiniku;

	// 完成した肉状態
	Niku* m_pNiku;

	bool m_bHoldNiku;

	struct CreatePos
	{
		Vector2 pos;
		int floor;		// これ作るためだけに構造体
		CreatePos(const Vector2 &pos, int floor) :pos(pos), floor(floor){}
	};
	std::vector<CreatePos> m_CreatePosList;	// ランダムに生成される座標たち
	int m_CreateTimer;					// 牧草生成タイマー
	int m_CREATETIME;					// 生成される時間
	int m_NextPoint;					// 生成される次のポイント

	// シングルトンの作法
	NikuManager();
	NikuManager(const NikuManager&){}
	NikuManager &operator=(const NikuManager&){}
};

#define NikuMgr (NikuManager::GetInstance())