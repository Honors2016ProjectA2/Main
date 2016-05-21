#pragma once

//***********************************
//		牧草クラス

// 牧草ステート
enum class BOKUSOU_MODE
{
	HUTABA,		// 双葉杏
	HONBA,		// 本葉未央
	TSUBOMI,	// つぼみ(Absolute Nine)
	SAITA,		// 花咲いた(もうネタ切れ)
	BORN,		// 牧草が生まれた。この段階で羊が拾うとパワーアップ
	MAX
};

// 前方宣言
class Bokusou;

// 牧草委譲モード
namespace BokusouMode
{
	class Base
	{
	protected:
		BOKUSOU_MODE m_NextMode;
		tdn2DObj *m_pImage;			// 画像
		int m_timer, m_limit;		// 目標と今の時間
		int m_srcX;
	public:
		Base(tdn2DObj *image) :m_pImage(image), m_timer(0), m_limit(0){}
		virtual ~Base(){}
		virtual void Update(Bokusou *pBokusou);
		virtual void Render(Bokusou *pBokusou);
		virtual BOKUSOU_MODE GetMode() = 0;
		void NextMode(Bokusou *pBokusou);
		void SetChangeModeTime(int val){ m_limit = val; }
	};

	class Hutaba:public Base
	{
	public:
		Hutaba(tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::HUTABA; }
	};

	class Honba :public Base
	{
	public:
		Honba(tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::HONBA; }
	};

	class Tsubomi :public Base
	{
	public:
		Tsubomi(tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::TSUBOMI; }
	};

	class Saita :public Base
	{
	public:
		Saita(tdn2DObj *image);
		//void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::SAITA; }
	};

	class Born :public Base
	{
	public:
		Born(tdn2DObj *image);
		void Update(Bokusou *pBokusou);
		//void Render(Bokusou *pBokusou);
		BOKUSOU_MODE GetMode(){ return BOKUSOU_MODE::BORN; }
	};

}

// 牧草クラス
class Bokusou
{
public:
	Bokusou(const Vector2 &pos);
	~Bokusou();
	void Update();
	void Render();

	// モード変更
	void ChangeMode(BOKUSOU_MODE m);

	// ゲッター
	BOKUSOU_MODE GetMode(){ return m_pMode->GetMode(); }
	int GetPosX(){ return (int)m_pos.x; }
	int GetPosY(){ return (int)m_pos.y; }
	Vector2 &GetPos(){ return m_pos; }

	// 消去
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

private:

	// 牧草の画像
	tdn2DObj *m_pBokusouFlower, *m_pBokusouRoll;

	// 牧草のモード委譲ポインタ
	BokusouMode::Base *m_pMode;

	// 座標
	Vector2 m_pos;

	bool m_bErase;
};



// 牧草を管理するクラス
class BokusouManager
{
public:
	static BokusouManager *GetInstance()
	{
		if (!pInstance) pInstance = new BokusouManager;
		return pInstance;
	}

	~BokusouManager();

	void Initialize();
	void Release();
	void Update();
	void Render();

	// ★UIマネージャーで呼び出す
	float GetGaugePercentage(){ return (float)m_CreateTimer / m_CREATETIME; }	// 牧草ゲージのpercent

private:
	// 1つしかない実体
	static BokusouManager *pInstance;

	// リスト
	std::list<Bokusou*> m_list;

	std::vector<Vector2> m_CreatePosList;	// ランダムに生成される座標たち
	int m_CreateTimer;					// 牧草生成タイマー
	int m_CREATETIME;					// 生成される時間

	// シングルトンの作法
	BokusouManager();
	BokusouManager(const BokusouManager&){}
	BokusouManager &operator=(const BokusouManager&){}
};


#define BokusouMgr (BokusouManager::GetInstance())