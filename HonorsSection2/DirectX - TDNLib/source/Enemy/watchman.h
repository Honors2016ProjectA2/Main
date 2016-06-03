#pragma once
#include "IEX_Expansion.h"

enum class ENEMY_TYPE
{
	WOLF,	// 狼
	MAX
};


enum class FAT_WOLF_TYPE
{
	SMALL, MIDDLE, LARGE
};

enum class SHEEP_TYPE;

namespace Enemy
{
	class Base
	{
	protected:
		bool m_bErase;			// 消去フラグ
		tdn2DObj *m_pImage;		// 敵の画像(マネージャのを参照するだけ)
		Vector2 m_pos;			// 座標
		Vector2 m_MoveVec;		// 移動の方向
		float m_speed;			// 移動速度
		int m_floor;			// レーン
		int m_AnimeFrame, m_AnimePanel;	// アニメ関係
		int W, H;			// 幅と高さ

		// 移動量の類の更新
		void MoveUpdate();
	public:
		Base(tdn2DObj *image, int floor, float speed) :col_check(false), m_bErase(false), m_floor(floor), m_pImage(image), m_AnimeFrame(0), m_AnimePanel(0), m_MoveVec(Vector2(-1, 0)), m_speed(speed){}
		virtual void Update() = 0;
		virtual void Render(){}

		// ゲッター
		int GetFloor(){ return m_floor; }
		Vector2 GetCenterPos(){ return Vector2(m_pos.x + W*.5f, m_pos.y + H*.5f); }
		Vector2 GetPos(){ return m_pos; }
		int GetWidth(){ return W; }

		// セッター
		void SetCenterPos(const Vector2 &pos){ m_pos = Vector2(pos.x - 60, pos.y - 60); }

		// 消去関係
		bool EraseOK(){ return m_bErase; }
		void Erase(){ m_bErase = true; }

		bool col_check;
	};

	class Wolf : public Base
	{
	public:
		Wolf(tdn2DObj *image, tdn2DObj *pniku, tdn2DObj *pHone, int floor, float speed, int nikustopTime);
		void Update();
		void Render();

		enum class MODE{ RUN, NIKU, DEAD, MAX }m_mode;
		void ChangeMode(MODE m)
		{
			m_AnimeFrame = m_AnimePanel = 0;
			m_mode = m;
			switch (m)
			{
			case MODE::RUN:
				m_MoveVec = Vector2(-1, 0);
				m_speed = m_OrgSpeed;
				break;
			case MODE::NIKU:
				m_MoveVec = Vector2(-1, 0);
				m_speed = 0;
				m_EatNikuTimer = 0;
				break;
			case MODE::DEAD:
				m_MoveVec = Vector2(1, 0);
				m_speed = 0;
				break;
			default:
				break;
			}
		}
		MODE GetMode(){ return m_mode; }

		void Kill(){ ChangeMode(MODE::DEAD); }

		void SetFatType(FAT_WOLF_TYPE type){ m_type = type; }
		void SetSheepType(SHEEP_TYPE type){ m_SheepType = type; }

	private:
		tdn2DObj *m_pNikukutteru, *m_pHoneImage;
		int m_EAT_NIKU_TIMER;	// 固定値
		int m_EatNikuTimer;
		float m_OrgSpeed;
		FAT_WOLF_TYPE m_type;	// 肉食い終わった後になるタイプ
		SHEEP_TYPE m_SheepType;
		BYTE m_alpha;

		void Run();
		void Niku();
		void Dead();
		void(Wolf::*ModeFunk[(int)MODE::MAX])();
	};
}

class FatWolf : public DebuBase
{
public:
	FatWolf(tdn2DObj *image, const Vector2 &pos, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType);
	~FatWolf();
	void Update();
	void Render();

	// ゲッター
	FAT_WOLF_TYPE GetType(){ return m_type; }
	SHEEP_TYPE GetSheepType(){ return m_SheepType; }

private:
	FAT_WOLF_TYPE m_type;
	SHEEP_TYPE m_SheepType;
};

class EnemyManager
{
public:
	// 実体の取得
	static EnemyManager *GetInstance(){ static EnemyManager i; return &i; }

	void Initialize();
	void Release();
	void Update();
	void Render();

	/* データリセット */
	void Clear();

	std::list<Enemy::Wolf*> *GetList(){ return &m_list; }
	std::list<FatWolf*> *GetFatList(){ return &m_FatList; }
	void Set_Pointers(StageManager *sm, DataManager *dm){ sp = sm, dmp = dm; }

	// 肉食い終わったら呼び出す
	void CreateFatWolf(Enemy::Wolf *wolf, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType);

private:

	// シングルトンの作法
	EnemyManager();
	EnemyManager(const EnemyManager&){}
	EnemyManager &operator=(const EnemyManager){}

	// 敵の画像
	tdn2DObj *m_pImages[(int)ENEMY_TYPE::MAX];

	// 肉食ってるオオカミの画像
	tdn2DObj *m_pNikukutteru, *m_pFatWolfImages[(int)SHEEP_TYPE::MAX], *m_pHoneImage;

	// 敵の移動速度
	float m_EnemySpeed[(int)ENEMY_TYPE::MAX];

	// ニッ食ってる時間ｎ
	int m_NikuTime;

	// 敵たちを格納するリスト
	std::list<Enemy::Wolf*> m_list;
	int m_CreateTimer;					// 敵生成タイマー
	int m_CREATETIME;					// 生成される時間
	int m_NextFloor;

	// 肉食って太った狼リスト
	std::list<FatWolf*> m_FatList;

	StageManager *sp;
	DataManager *dmp;

	// 敵生成
	void Create(int floor, ENEMY_TYPE type);
};


// インスタンス化
#define EnemyMgr (EnemyManager::GetInstance())