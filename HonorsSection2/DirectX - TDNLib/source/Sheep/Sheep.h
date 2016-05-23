#pragma once
#include "IEX_Expansion.h"
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
	float magnification;	// 得点倍率
	int SIZE;				// サイズ(コマの)
	int NumPanel;			// コマの数
	int Animkankaku;		// アニメの間隔 
	float percentage;		// 出現率
};

struct SheepTextParam
{
	int startX;			// 羊出現座標
	int AppTime;		// 羊の出現間隔(ミリ秒)
	int speed;			// 速度
	int VerticalWidth;	// 出現の上下幅
	SheepData data[(int)SHEEP_TYPE::MAX];
	float FatSheepAccel;// 太ってる羊の加速度
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
	bool m_bErase;	// 消去フラグ

	//const int PNGSIZE;
	int floor;

	// 羊の情報
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
		tdn2DObj *ase;
		int animex;

	}ase;

	void Get_out();
	void Walk();
	void Curve();
	void Caught();
	void Crushed();
	void Ran_over();
	void(Sheep::*Move[6])();
public:
	Sheep(const SheepData &data, tdn2DObj *ase, int floor, const SheepTextParam &textparam);
	~Sheep();

	void Update();

	void Render();

	/* 左上 */
	Vector2 *Get_pos(){ return &pos; }
	/* 中央↓ */
	Vector2 &GetCenterPos(){ return pos + Vector2(60, 60); }
	void Get_pos2(Vector2 &out)
	{
		out = pos;
		out.x += m_data.SIZE * 0.5f;
		out.y += m_data.SIZE;
	}
	void SetPos(const Vector2 &p){ pos = p; }
	void SetPosX(float x){ pos.x = x; }

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
	float TokutenBairitsu(){ return m_data.magnification; }

	// 消去関係
	void Erase(){ m_bErase = true; }
	bool EraseOK(){ return m_bErase; }

	bool col_check;
};


// 牧草食って太った羊
class FatSheep
{
public:
	FatSheep(tdn2DObj *image, const Vector2 &pos);
	~FatSheep();

	// 更新・描画
	void Update();
	void Render();

	// ゲッター
	Vector2 &GetCenterPos(){ return m_pos + Vector2(96, 96); }
	int GetWidth(){ return m_image->GetWidth(); }
	int GetFloor(){ return m_floor; }
	bool EraseOK(){ return m_bErase; }

	// セッター
	void SetMove(float val){ m_moveX = val; }
	void SetFloor(int val){ m_floor = val; }
	void Erase(){ m_bErase = true; }

private:
	Vector2 m_pos;		// 座標
	float m_moveX;		// 移動値
	tdn2DObj *m_image;	// 自分の画像
	float m_angle;		// 転がるので、アングル。
	int m_floor;		// 自分のいるレーン
	bool m_bErase;		// 消去フラグ
};


//**************************************************

class SheepManager
{
public:
	std::list<Sheep*> *Get_list(){ return &m_List; };
	std::list<FatSheep*> *GetFatList(){ return &m_FatList; }

	SheepManager();
	~SheepManager();

	void Update();
	void Render();

	void Set_pointers(StageManager *sp, DataManager *dmp){ this->sp = sp, this->dmp = dmp; };

	void Reset(){ for (auto it : m_List){ delete it; }m_List.clear(); }
	void Start(){ m_CurrentTime = clock(); }

	float FatSheepAccel(){ return m_TextParam.FatSheepAccel; }// 太ってる羊の加速度

	// 牧草食ったら呼び出す
	void CreateFatSheep(Sheep *sheep)
	{
		sheep->Erase();	// 元の羊を消去
		FatSheep *set = new FatSheep(m_pFatSheepImage, *sheep->Get_pos() + Vector2(-32, -32));// 太った羊生成
		set->SetFloor(sheep->Get_floor());	// フロア設定
		m_FatList.push_back(set);			// リストに突っ込む
	}

private:
	//tdn2DObj *Getfile(int num){ return files[num]; }
	//tdn2DObj *files[(int)SHEEP_TYPE::MAX];
	tdn2DObj *m_pFatSheepImage, *ase;
	int m_CurveRange;		// 犬に対して、曲がれ命令出す範囲。

	std::list<Sheep*> m_List;		// 普通の羊リスト
	std::list<FatSheep*> m_FatList;	// 太った羊リスト
	void create(int floor);

	// 今経過してる時間(ミリ秒)
	UINT m_CurrentTime;

	// テキストからの情報
	SheepTextParam m_TextParam;

	StageManager *sp;
	DataManager *dmp;
};