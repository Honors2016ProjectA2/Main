#pragma once
#include "IEX_Expansion.h"
class MousePointer;
class StageManager;

enum class SHEEP_TYPE
{
	NOMAL,	// 通常羊
	GOLD,	// 金羊
	REAL,	// 実写
	MAX
};

struct SheepData
{
	tdn2DObj *Image;		// 画像
	int score;				// 得点
	int SIZE;				// サイズ(コマの)
	int NumPanel;			// コマの数
	int Animkankaku;		// アニメの間隔 
};

struct SheepTextParam
{
	int startX;			// 羊出現座標
	int AppTime;		// 羊の出現間隔(ミリ秒)
	int speed;			// 速度
	int VerticalWidth;	// 出現の上下幅
	SheepData data[(int)SHEEP_TYPE::MAX];
};

class Sheep
{
private:
	int frame, animeframe, m_AnimePanel;
	Vector2 pos, animepos,
		move,		// カーブとかで、あとから足すようの移動値
		C_MOVE;		// 本来持っている固定の移動値
	DIR m_CurveDir;	// カーブの向き
	bool m_bTurned;	// サインで直角に曲げた後、戻る野に必要
	float m_sinAngle;	// カーブをサインカーブでするので、それ用の変数

	//const int PNGSIZE;
	int floor;

	SheepData m_data;

	enum MODE{ 
		GETOUT,		// Initialize的なやつっぽい
		WALK,		// 普通の歩き
		CURVE,		// 方向転換
		CAUGHT,		// 普通の死に
		CRUSHED,	// ゴリラに殺された
		RAN_OVER	// 轢き殺された？
	};
	int process;
	DWORD alpha;

	void Get_frame_pos(Vector2 &pos, int frame);

	struct
	{
		tdn2DObj *byu;
		int animex;
	}byu;
	struct
	{
		tdn2DObj *ase;
		int animex;

	}ase;

	float anim = .0f;	//ビューン用追加

	bool Get_out();
	bool Walk();
	bool Curve();
	bool Caught();
	bool Crushed();
	bool Ran_over();
	bool(Sheep::*Move[6])();
public:
	Sheep(const SheepData &data, tdn2DObj *byu, tdn2DObj *ase, int floor, const SheepTextParam &textparam);
	~Sheep();

	bool Update();

	void Render();

	/* 左上 */
	Vector2 *Get_pos(){ return &pos; }
	/* 中央↓ */
	void Get_pos2(Vector2 &out)
	{
		out = pos;
		out.x += m_data.SIZE * 0.5f;
		out.y += m_data.SIZE;
	}
	int Get_size(){ return m_data.SIZE; }
	int Get_floor(){ return floor; }
	/* やられた */
	void Be_caught(int type);
	void Be_crushed();
	void Be_ran_over();
	void SetCurve(DIR dir)
	{
		if (process != MODE::WALK) return;
		m_CurveDir = dir; process = MODE::CURVE; m_sinAngle = 0; m_bTurned = false;
	}

	bool col_check;
};

//**************************************************

class SheepManager
{
public:
	std::list<Sheep*> *Get_list(){ return &m_List; };

	SheepManager();
	~SheepManager();

	void Update();
	void Render();

	void Set_pointers(MousePointer *mp, StageManager *sp, DataManager *dmp){ this->mp = mp, this->sp = sp, this->dmp = dmp; };

	void Reset(){ for (auto it : m_List){ delete it; }m_List.clear(); }
	void Start(){ m_CurrentTime = clock(); }

private:
	//tdn2DObj *Getfile(int num){ return files[num]; }
	//tdn2DObj *files[(int)SHEEP_TYPE::MAX];
	tdn2DObj *byu, *ase;
	int m_CurveRange;		// 犬に対して、曲がれ命令出す範囲。

	std::list<Sheep*> m_List;
	void create(int floor);

	// 今経過してる時間(ミリ秒)
	UINT m_CurrentTime;

	// テキストからの情報
	SheepTextParam m_TextParam;

	MousePointer *mp;
	StageManager *sp;
	DataManager *dmp;
};