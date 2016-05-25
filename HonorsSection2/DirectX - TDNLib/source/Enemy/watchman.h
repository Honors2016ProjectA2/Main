#pragma once
#include "IEX_Expansion.h"

enum class ENEMY_TYPE
{
	WOLF,	// 狼
	METEO,	// メテオ
	MAX
};

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
		Base(tdn2DObj *image, int floor, float speed) :m_bErase(false), m_floor(floor), m_pImage(image), m_AnimeFrame(0), m_AnimePanel(0), m_MoveVec(Vector2(-1, 0)), m_speed(speed){}
		virtual void Update() = 0;
		virtual void Render(){}

		// ゲッター
		int GetFloor(){ return m_floor; }
		Vector2 GetCenterPos(){ return Vector2(m_pos.x + W*.5f, m_pos.y + H*.5f); }
		int GetWidth(){ return W; }

		// 消去関係
		bool EraseOK(){ return m_bErase; }
		bool Erase(){ m_bErase = true; }
	};

	class Wolf : public Base
	{
	public:
		Wolf(tdn2DObj *image, int floor, float speed);
		void Update();
		void Render();
	};

	class Meteo : public Base
	{
	public:
		Meteo(tdn2DObj *image, int floor, float speed);
		void Update();
		void Render();
	};
}

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

	std::list<Enemy::Base*> *GetList(){ return &m_list; }
	void Set_Pointers(StageManager *sm, DataManager *dm){ sp = sm, dmp = dm; }

private:

	// シングルトンの作法
	EnemyManager();
	EnemyManager(const EnemyManager&){}
	EnemyManager &operator=(const EnemyManager){}

	// 敵の画像
	tdn2DObj *m_pImages[(int)ENEMY_TYPE::MAX];

	// 敵の移動速度
	float m_EnemySpeed[(int)ENEMY_TYPE::MAX];

	// 敵たちを格納するリスト
	std::list<Enemy::Base*> m_list;
	int m_CreateTimer;					// 敵生成タイマー
	int m_CREATETIME;					// 生成される時間

	StageManager *sp;
	DataManager *dmp;

	// 敵生成
	void Create(int floor, ENEMY_TYPE type);
};


// インスタンス化
#define EnemyMgr (EnemyManager::GetInstance())