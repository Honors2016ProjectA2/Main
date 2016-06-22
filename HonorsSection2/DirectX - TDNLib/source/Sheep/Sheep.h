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
	tdn2DObj *CatchImage;	// 掴まれ画像
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
		bool isDead(){ return (process == MODE::CRUSHED); }
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

	SheepManager(bool bTitle = false);
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

	// 掴み更新
	void CatchUpdate();

	// タイトル用か
	bool m_bTitle;
};

// 実体
extern SheepManager* g_pSheepMgr;
extern int g_CreateSheepFloor;










// リザルトにて、管理される個々の羊
class ResultSheep
{
public:
	enum MODE{ RUN, HAKERU };

	ResultSheep(const Vector2 &pos, float speed) :m_pos(pos), m_speed(speed), m_pMode(nullptr), m_bErase(false){ ChangeMode(MODE::RUN); };
	~ResultSheep(){ SAFE_DELETE(m_pMode); }
	void Update(){ m_pMode->Update(); }
	void Render(tdn2DObj *pImage){ m_pMode->Render(pImage); }

	// 消去関連
	bool EraseOK(){ return m_bErase; }
	void Erase(){ m_bErase = true; }

	void ChangeMode(MODE m)
	{
		if (m_pMode) delete m_pMode;
		switch (m)
		{
		case ResultSheep::RUN:
			m_pMode = new Mode::Run(this);
			break;
		case ResultSheep::HAKERU:
			m_pMode = new Mode::Hakeru(this);
			break;
		}
	}

	Vector2 &GetPos(){ return m_pos; }
private:

	Vector2 m_pos;	// 座標
	float m_speed;	// 速度
	bool m_bErase;	// 消去フラグ

	// モード分岐用
	class Mode
	{
	public:
		class Base
		{
		protected:
			ResultSheep *m_pSheep;	// 親
			int m_AnimFrame, m_AnimPanel;	// アニメ関係
			float m_angle;
		public:
			Base(ResultSheep *pSheep) :m_pSheep(pSheep), m_AnimFrame(0), m_AnimPanel(rand() % 4),m_angle(0){}
			virtual void Update()
			{
				// あにめ
				if (++m_AnimFrame > 7)
				{
					m_AnimFrame = 0;
					if (++m_AnimPanel >= 4)m_AnimPanel = 0;
				}
			}
			virtual void Render(tdn2DObj *pImage)
			{
				pImage->SetAngle(m_angle);
				pImage->Render((int)m_pSheep->m_pos.x, (int)m_pSheep->m_pos.y, 120, 120, m_AnimPanel * 120, 0, -120, 120);
			}
		};

		// 左から右に走る
		class Run:public Base
		{
		public:
			Run(ResultSheep *pSheep) :Base(pSheep){}
			void Update();
			//void Render(tdn2DObj *pImage);
		};

		// 掃ける
		class Hakeru:public Base
		{
		private:
			Vector2 m_vec;
		public:
			Hakeru(ResultSheep *pSheep);
			void Update();
			//void Render(tdn2DObj *pImage);
		};
	};

	// モードポインタ
	Mode::Base *m_pMode;
};

class Number;

// リザルト用羊管理
class ResultSheepManager
{
public:
	ResultSheepManager(float ScoreWakuY, float SohotaWakuY, float startX, float speed);
	~ResultSheepManager();

	void Update();
	void Render();

	// スコアの羊スタート
	void StartScoreSheep();
	// スコア項目の羊が掃ける
	void HakeruScoreSheep(){ for (auto& it : m_ScoreSheepList)it->ChangeMode(ResultSheep::MODE::HAKERU); }

	// その他項目の羊スタート
	void StartOtherSheep();
	// その他項目の羊が掃ける
	void HakeruOtherSheep(){ for (auto& it : m_OtherSheepList)it->ChangeMode(ResultSheep::MODE::HAKERU); }

	bool isScoreEnd(){ return m_waku[0].isSet; }
	bool isOtherEnd(){ return m_waku[1].isSet; }

	void Reset()
	{
		for (auto it = m_ScoreSheepList.begin(); it != m_ScoreSheepList.end();)
		{
			delete (*it);
			it = m_ScoreSheepList.erase(it);
		}
		for (auto it = m_OtherSheepList.begin(); it != m_OtherSheepList.end();)
		{
			delete (*it);
			it = m_OtherSheepList.erase(it);
		}
		m_waku[0].pos.x = m_waku[1].pos.x = -1280;
		m_waku[0].isSet = m_waku[1].isSet = false;
	}
private:
	// スコア枠用の羊
	std::list <ResultSheep*> m_ScoreSheepList;

	// その他枠用の羊
	std::list <ResultSheep*> m_OtherSheepList;

	// スコアのナンバー
	enum NUMBER_TYPE{ SCORE, COMBO, WOLFKILL, SHEEPKILL, MAX };
	Number *m_pNumbers[NUMBER_TYPE::MAX];

	// 羊画像
	tdn2DObj *m_pSheepImage;

	// 枠画像
	struct Waku
	{
		tdn2DObj *pImage;
		Vector2 pos;
		bool isSet;
		Waku() :pos(-1280, 0), isSet(false){}
	}m_waku[2];	// [0]:スコア枠, [1]:その他枠

	// 枠羊の開始地点Y
	const float m_StartScoreY, m_StartOtherY;
	// 開始座標X
	const float m_StartX;
	// 羊の速度
	const float m_speed;

	// 枠の幅
	static const int WAKU_WIDTH = 756;
};