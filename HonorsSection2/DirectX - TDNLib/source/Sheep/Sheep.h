#pragma once
#include "IEX_Expansion.h"
class StageManager;
class DataManager;
#include "../CharaBase/CharaBase.h"

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
	tdn2DObj *BoneImage;	// 死んだ骨
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

namespace Sheep
{
	class Base
	{
	protected:
		int frame, animeframe, m_AnimePanel;
		Vector2 pos, animepos,
			move,			// カーブとかで、あとから足すようの移動値
			C_MOVE;			// 本来持っている固定の移動値
		DIR m_CurveDir;		// カーブの向き
		bool m_bTurned;		// サインで直角に曲げた後、戻る野に必要
		float m_sinAngle;	// カーブをサインカーブでするので、それ用の変数
		bool m_bErase;		// 消去フラグ
		bool m_bLeftCatch;	// 左クリックでつかんだか

		//const int PNGSIZE;
		int m_floor;

		// 羊の情報
		SheepData m_data;

		enum MODE{
			WALK,		// 普通の歩き
			CURVE,		// 方向転換
			PUSH,		// デブ押す
			CRUSHED,	// 食われた
			CAUGHT,		// 捕まった
			MAX
		};
		int process;
		BYTE alpha;

		void Get_frame_pos(Vector2 &pos, int frame);

		struct
		{
			tdn2DObj *ase;
			int animex;

		}ase;

		void Get_out();
		void Walk();
		void Curve();
		void Push();
		void Crushed();
		void Caught();
		void(Base::*Move[MODE::MAX])();
	public:
		Base(const SheepData &data, int floor, const SheepTextParam &textparam);
		virtual ~Base();

		virtual void Update();
		virtual void Render();

		/* 左上 */
		Vector2 *Get_pos(){ return &pos; }
		/* 中央↓ */
		Vector2 GetCenterPos(){ return pos + Vector2(60, 60); }
		void Get_pos2(Vector2 &out)
		{
			out = pos;
			out.x += m_data.SIZE * 0.5f;
			out.y += m_data.SIZE;
		}
		void SetPos(const Vector2 &p){ pos = p; }
		void SetPosX(float x){ pos.x = x; }

		int Get_size(){ return m_data.SIZE; }
		int Get_floor(){ return m_floor; }
		void Be_Walk(){ if(process == MODE::PUSH) process = MODE::WALK; }
		void Be_Push(){ if(process == MODE::WALK) process = MODE::PUSH; }
		void Be_crushed();
		void Be_caught(bool bLeft);
		bool isPushOK(){ return (process == MODE::WALK || process == MODE::PUSH); }
		bool isCaught(){ return (process == MODE::CAUGHT); }
		void SetCurve(DIR dir)
		{
			if (process != MODE::WALK) return;
			m_CurveDir = dir; process = MODE::CURVE; m_sinAngle = 0; m_bTurned = false;
		}
		float TokutenBairitsu(){ return m_data.magnification; }

		// 消去関係
		void Erase(){ m_bErase = true; }
		bool EraseOK(){ return m_bErase; }

		// タイプ取得
		virtual SHEEP_TYPE GetType() = 0;

		bool col_check;
	};

	class Normal:public Base
	{
	private:
	public:
		Normal(const SheepData &data, int floor, const SheepTextParam &textparam) :Base(data, floor, textparam){}
		SHEEP_TYPE GetType(){ return SHEEP_TYPE::NOMAL; }
	};

	class Gold:public Base
	{
	private:
		int m_seID;
	public:
		Gold(const SheepData &data, int floor, const SheepTextParam &textparam);
		~Gold();
		void Update();
		SHEEP_TYPE GetType(){ return SHEEP_TYPE::GOLD; }
	};

	class Real:public Base
	{
	private:
	public:
		Real(const SheepData &data, int floor, const SheepTextParam &textparam) :Base(data, floor, textparam){}
		void Update();
		SHEEP_TYPE GetType(){ return SHEEP_TYPE::REAL; }
	};
}

// 牧草食って太った羊
class FatSheep : public DebuBase
{
public:
	FatSheep(tdn2DObj *image, const Vector2 &pos, SHEEP_TYPE type);
	~FatSheep();

	SHEEP_TYPE GetType(){ return m_type; }

private:
	SHEEP_TYPE m_type;
};


//**************************************************
// グローバル領域
extern int g_CreateSheepFloor;		// 羊を生成するフロア

class SheepManager
{
public:
	std::list<Sheep::Base*> *Get_list(){ return &m_List; };
	std::list<FatSheep*> *GetFatList(){ return &m_FatList; }

	SheepManager();
	~SheepManager();

	void Update();
	void Render();
	void RenderFat();

	void Set_pointers(StageManager *sp, DataManager *dmp){ this->sp = sp, this->dmp = dmp; };

	void Reset(){ for (auto it : m_List){ delete it; }m_List.clear(); }

	float FatSheepAccel(){ return m_TextParam.FatSheepAccel; }// 太ってる羊の加速度

	// 牧草食ったら呼び出す
	void CreateFatSheep(Sheep::Base *sheep);

	// ゲッタ
	int GetOpenFloor(){ return g_CreateSheepFloor; }

	//	タイトルで使う用
	void SetDownLane() { g_CreateSheepFloor = m_NextChangeFloor = 2; }
	SheepTextParam *GetSheepTextParam() { return &m_TextParam; }

private:
	//tdn2DObj *Getfile(int num){ return files[num]; }
	tdn2DObj *m_pFatSheepImages[(int)SHEEP_TYPE::MAX];
	tdn2DObj *m_pBoneImage;
	int m_CurveRange;		// 犬に対して、曲がれ命令出す範囲。

	std::list<Sheep::Base*> m_List;		// 普通の羊リスト
	std::list<FatSheep*> m_FatList;	// 太った羊リスト
	void create(int floor);

	int m_NextChangeFloor;				// 変わった先のレーン
	int MakeNextFloor(int current);		// 前回レーンではないレーンをランダムに作成する関数

	// 今経過してる時間(フレーム)
	int m_CreateFrame;

	// テキストからの情報
	SheepTextParam m_TextParam;

	StageManager *sp;
	DataManager *dmp;
};

// 実体
extern SheepManager* g_pSheepMgr;
extern int g_CreateSheepFloor;