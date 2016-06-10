//------------ include --------------
#include "StageMNG.h"
#include "TDNLIB.h"
#include "Data/DataMNG.h"
#include "MousePointer.h"
#include "../Collision/Collision.h"
#include "../Enemy/watchman.h"
#include "../Sound/SoundManager.h"
#include "../Niku/Niku.h"
#include "Effect\EffectManager.h"
#include "../Sheep/Sheep.h"

//int g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MAX];			// モードが変わっていく時間

//**************************************************
//    StageManager class
//**************************************************

//========================================
// グローバル領域
int STAGE_POS_Y[3] =
{
	165, 520, 520// ★ここいじったろ
};
int LANE_WIDTH = 0;
Vector2 YAKINIKU_AREA(0, 0);
bool g_bDogSetFrame = false;		// 犬を設置したフレームかどうか

int FindFloor(float posY)
{
	// 最短フロア検索
	float dist(640);
	int floor(0);
	for (int i = 0; i < STAGE_MAX; i++)
	{
		float vy = posY - STAGE_POS_Y[i];
		vy = (vy < 0) ? vy * -1 : vy;	// 絶対値

		if (vy < dist)
		{
			floor = i;
			dist = vy;
		}
	}
	return floor;
}

//-------- static field ---------

//------- constructor,destructor ---------

StageManager::StageManager() :m_pDogImage(new tdn2DObj("DATA/CHR/dog.png")), m_pFireImage(new tdn2DObj("DATA/巻き/炎の種.png")),
m_FireAnimFrame(0), m_FireAnimPanel(0), m_ChangeScoreTime(0)
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		stage[i] = nullptr;
	}

	// image load

	// テキストから情報取得
	std::ifstream ifs("DATA/Text/Param/stage.txt");

	char skip[64];	// 読み飛ばし用

	// ステージ画像読み込み
	ifs >> skip;
	FOR(StageImage::MAX)
	{
		char path[MAX_PATH];
		ifs >> path;
		m_pStageImages[i] = new tdn2DObj(path);
	}

	// いけにえ座標読み込み
	ifs >> skip;
	ifs >> YAKINIKU_AREA.x;
	ifs >> YAKINIKU_AREA.y;

	// レーン幅読み込み
	ifs >> skip;
	ifs >> LANE_WIDTH;

	ifs >> skip;
	FOR(3)
	{
		static const int OFFSET = 120;
		STAGE_POS_Y[i] = OFFSET + LANE_WIDTH * i;

		// レーン解放羊数
		ifs >> APPEND_STAGE_BORDER[i];
	}

	// 小屋のリキャスト
	ifs >> skip;
	ifs >> m_RECAST_TIME;

	// レーンスコア数
	ifs >> skip;
	FOR(3)
	{
		ifs >> m_AddScore[i];
	}

	// レーンスコア変える時間
	ifs >> skip;
	ifs >> m_CHANGE_SCORE_TIME;

	//// 火の変化時間読み込み
	//ifs >> skip;
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MAKI];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOEHAJIME];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOETEKITA];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOETA];
	//g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::ENABLE];				// 完全燃焼モード
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::HITED];

	// 犬ストック個数読み込み
	ifs >> skip;
	ifs >> m_DogStock;


	// 犬設置ポイント読み込み
	FOR(STAGE_MAX)
	{
		m_Doglists[i].clear();
		//m_Firelists[i].clear();
	}
	ifs >> skip;

	int id[STAGE_MAX] = {0};
	while (!ifs.eof())
	{
		int posX, floor;
		ifs >> floor;
		ifs >> posX;

		char cDir[5];
		DIR dir;

		ifs >> cDir;

		if (strcmp(cDir, "UP") == 0) dir = DIR::UP;
		else if (strcmp(cDir, "DOWN") == 0) dir = DIR::DOWN;
		else assert(0);	// 例外処理

		m_Doglists[floor].push_back(new CurvePoint::Dog(this, m_pDogImage, Vector2((float)posX, (float)STAGE_POS_Y[floor] + 20), dir, id[floor], floor));
		//if(id[floor] == 1)m_Firelists[floor].push_back(new CurvePoint::Fire(this, m_pFireImage, Vector2((float)posX, (float)STAGE_POS_Y[floor] + 20), dir, id[floor], floor));
		id[floor]++;
	}

	// 本来はスコアでレーンを追加していくが、今回は最初から3レーン
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)
			stage[i] = new Stage;
		stage[i]->Init(Vector2((float)0, (float)STAGE_POS_Y[i]), Stage::StageState::FALL);
	}
}

StageManager::~StageManager()
{
	FOR(StageImage::MAX)delete m_pStageImages[i];
	delete m_pDogImage;
	delete m_pFireImage;

	FOR(STAGE_MAX)
	{
		SAFE_DELETE(stage[i]);

		// 犬リストの開放
		for (auto it : m_Doglists[i]) delete it;

		// 炎リストの開放
		//for (auto it : m_Firelists[i]) delete it;
	}

	// タイトル用
	STAGE_POS_Y[0] = 165;
	STAGE_POS_Y[1] = 520;
	STAGE_POS_Y[2] = 520;
}

//--------- public method -----------

void StageManager::Init()
{
	// image delete
	//for (int i = 0; i < STAGE_MAX; i++)
	//{
	//	SAFE_DELETE(stageImage[i]);
	//}
	//
	//// image load
	//stageImage[0]   = new tdn2DObj("DATA/Stage/Game Stage01.png");
	//stageImage[1]   = new tdn2DObj("DATA/Stage/Game Stage02.png");
	//stageImage[2]   = new tdn2DObj("DATA/Stage/Game Stage03.png");
	//Reset();
}

void StageManager::Reset()
{
	// 羊生成するフロア
	SetDogFloor(g_CreateSheepFloor);
}

void StageManager::Update()
{
	// ホイール
	//if (tdnMouse::GetWheelFlag() != WHEEL_FLAG::NONE) m_FireSelect = !m_FireSelect;	// 反転

	// マウス座標
	Vector2 mPos = tdnMouse::GetPos();

	// マウス左クリックしたかどうか
	bool bLeftClick = (tdnMouse::GetLeft() == 3);

	// 羊生成の小屋を変える処理
	if (bLeftClick)
	{
		// マウスが左端
		if (mPos.x < 150)
		{
			int floor = -1;
			for (int i = 0; i < STAGE_MAX; i++)
			{
				int y = (int)mPos.y;
				if (y > STAGE_POS_Y[i] && y < STAGE_POS_Y[i] + LANE_WIDTH)
				{
					floor = i;
					break;
				}
			}

			if (floor != -1)
			{
				// リキャストOK
				if (stage[floor]->GetRecastTime() <= 0)
				{
					// 羊生成フロア変える
					g_CreateSheepFloor = floor;

					// リキャスト設定
					stage[floor]->SetRecastTime(m_RECAST_TIME);

					// SEの再生
					se->Play("ドア", Vector2(128, (float)STAGE_POS_Y[floor] + LANE_WIDTH / 2));

					// 犬配置リセット
					m_DogStock = 2;

					// 犬レーン変更
					SetDogFloor(floor);
				}
			}
		}
	}

	// 時間経過でスコアを変える処理
	if (++m_ChangeScoreTime > m_CHANGE_SCORE_TIME)
	{
		// レーン変える処理
		m_ChangeScoreTime = 0;
		ChangeScoreLane();
	}

	// 犬設置したフレームかどうか
	g_bDogSetFrame = false;

	enum class DOG
	{
		NONE,		// 何もしてない
		SET,		// 配置
		RECOVER,	// 回収
	}DogFlag = DOG::NONE;


	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)continue;
		stage[i]->Update();

		// 犬リスト更新
		for (auto it : m_Doglists[i])
		{
			it->Update();
			//bool bFireOpen(false);
			//if (it->GetID() == 1)bFireOpen = m_Firelists[i][0]->IsOpening();

			// 炎選んでなかったら
			//if (!bFireOpen)
			{
				// マウスの中に入ってるかどうかの判定
				if (mPos.x >= it->GetPos().x && mPos.x <= it->GetPos().x + it->GetWidth() &&
					mPos.y >= it->GetPos().y && mPos.y <= it->GetPos().y + it->GetWidth())
				{
					// 初回入ったフレーム
					if (!it->m_bCursorIn)
					{
						it->m_bCursorIn = true;
					}
				}
				else it->m_bCursorIn = false;

				// 左クリック！！かつカーソル範囲内かつ有効状態かつ肉持ってなかったらなら
				if (bLeftClick && it->m_bCursorIn && it->bEnable && !NikuMgr->isNikuHold())
				{
					// 犬回収
					if (it->IsOpening())
					{
						se->Play("犬", it->GetPos());
						m_DogStock++;	// ストック回復
						it->Change();	// 犬のON_OFF
						// 犬設置したフレームかどうか
						g_bDogSetFrame = true;

						// 犬回収フラグ
						DogFlag = DOG::RECOVER;
						break;
					}

					// 犬設置
					else
					{
						// ストック残ってたら
						if (m_DogStock > 0)
						{
							EffectMgr.AddEffect((int)it->GetPos().x+64, (int)it->GetPos().y+64,EFFECT_TYPE::DOG_EFFECT);
							se->Play("犬", it->GetPos());
							m_DogStock--;
							it->Change();	// 犬のON_OFF
							// 犬設置したフレームかどうか
							g_bDogSetFrame = true;

							// 犬設置フラグ
							DogFlag = DOG::SET;
							break;
						}
					}

				}
			}
			//else it->m_bCursorIn = false;

		}	// 犬リスト

		// 犬設置してたら
		if (DogFlag == DOG::SET)
		{
			// 2体とも設置してたら、
			if (m_DogStock == 0)
			{
				// それ以外全部無効化
				FOR(STAGE_MAX) for (auto it : m_Doglists[i])
				{
					// 設置されてない位置を無効化
					if (!it->IsOpening()) it->bEnable = false;
				}
			}

			// 1体だけ設置なら、必要箇所を無効化
			else
			{
				// 上レーン左
				if (m_Doglists[0][0]->IsOpening())
				{
					// 真ん中の右以外無効化
					m_Doglists[0][1]->bEnable = false;
					m_Doglists[1][0]->bEnable = false;
					m_Doglists[1][1]->bEnable = true;
					m_Doglists[2][0]->bEnable = false;
					m_Doglists[2][1]->bEnable = false;
				}

				// 上レーン右
				else if (m_Doglists[0][1]->IsOpening())
				{
					// 真ん中の右以外無効化
					m_Doglists[0][0]->bEnable = false;
					m_Doglists[1][0]->bEnable = false;
					m_Doglists[1][1]->bEnable = true;
					m_Doglists[2][0]->bEnable = false;
					m_Doglists[2][1]->bEnable = false;
				}

				// 真ん中レーン左
				else if (m_Doglists[1][0]->IsOpening())
				{
					// 上の右以外無効化
					m_Doglists[0][0]->bEnable = false;
					m_Doglists[0][1]->bEnable = true;
					m_Doglists[1][1]->bEnable = false;
					m_Doglists[2][0]->bEnable = false;
					m_Doglists[2][1]->bEnable = false;
				}

				// 真ん中レーン右
				else if (m_Doglists[1][1]->IsOpening())
				{
					// それ以外無効化
					m_Doglists[0][0]->bEnable = false;
					m_Doglists[0][1]->bEnable = false;
					m_Doglists[1][0]->bEnable = false;
					m_Doglists[2][0]->bEnable = false;
					m_Doglists[2][1]->bEnable = false;
				}

				// 下レーン左
				else if (m_Doglists[2][0]->IsOpening())
				{
					// 真ん中レーン以外無効化
					m_Doglists[0][0]->bEnable = false;
					m_Doglists[0][1]->bEnable = false;
					m_Doglists[1][0]->bEnable = true;
					m_Doglists[1][1]->bEnable = true;
					m_Doglists[2][1]->bEnable = false;
				}

				// 下レーン右
				else if (m_Doglists[2][1]->IsOpening())
				{
					// それ以外無効化
					m_Doglists[0][0]->bEnable = false;
					m_Doglists[0][1]->bEnable = false;
					m_Doglists[1][0]->bEnable = false;
					m_Doglists[1][1]->bEnable = false;
					m_Doglists[2][0]->bEnable = false;
				}
			}
		}

		// 犬回収してたら
		else if (DogFlag == DOG::RECOVER)
		{
			// まだ1体設置してる状態だったら
			if (m_DogStock == 1)
			{
				// 上レーン左
				if (m_Doglists[0][0]->IsOpening())
				{
					// 真ん中の右を有効化
					m_Doglists[1][1]->bEnable = true;
				}

				// 上レーン右
				else if (m_Doglists[0][1]->IsOpening())
				{
					// 真ん中の右を有効化
					m_Doglists[1][1]->bEnable = true;
				}

				// 真ん中レーン右
				else if (m_Doglists[1][1]->IsOpening())
				{
					// 上レーンと下の左有効化
					if (g_CreateSheepFloor == 0)
					{
						m_Doglists[0][0]->bEnable = true;
						m_Doglists[0][1]->bEnable = true;
					}
					else if(g_CreateSheepFloor == 2) m_Doglists[2][0]->bEnable = true;
				}

				// 下レーン左
				else if (m_Doglists[2][0]->IsOpening())
				{
					// 真ん中レーンを有効化
					m_Doglists[1][0]->bEnable = true;
					m_Doglists[1][1]->bEnable = true;
				}
			}

			// 全員回収してる状態だったら
			else
			{
				SetDogFloor(g_pSheepMgr->GetOpenFloor());
			}
		}

		// 火リスト更新
		//for (auto it : m_Firelists[i])
		//{
		//	it->Update();
		//
		//	// 炎選んでたら
		//	if (m_FireSelect && !m_Doglists[i][it->GetID()]->IsOpening())
		//	{
		//		// マウスの中に入ってるかどうかの判定
		//		if (mPos.x >= it->GetPos().x && mPos.x <= it->GetPos().x + it->GetWidth() &&
		//			mPos.y >= it->GetPos().y && mPos.y <= it->GetPos().y + it->GetWidth())
		//		{
		//			// 初回入ったフレーム
		//			if (!it->m_bCursorIn)
		//			{
		//				it->m_bCursorIn = true;
		//			}
		//		}
		//		else it->m_bCursorIn = false;
		//
		//		// 左クリック！！
		//		if (tdnMouse::GetLeft() == 3 && it->m_bCursorIn)
		//		{
		//			// 炎設置
		//			if (!it->IsOpening() && m_FireStock > 0)
		//			{
		//		
		//				se->Play("まき", it->GetPos());		// SEの再生
		//				m_FireStock--;	// ストック減少
		//
		//				it->Change();	// ON
		//			}
		//		}
		//	}
		//	else it->m_bCursorIn = false;
		//
		//}	// 炎リスト

	}	// レーンfor

	// ステージのアニメーション
	if (++m_FireAnimFrame > 4)
	{
		m_FireAnimFrame = 0;
		if (++m_FireAnimPanel > 2)m_FireAnimPanel = 0;
	}
}

void StageManager::ChangeScoreLane()
{
	// 交換
	auto Swap = [](int *a, int *b)
	{
		int t = *a;
		*a = *b;
		*b = t;
	};

	// ランダムにレーンを入れ替える
	Swap(&m_AddScore[0], &m_AddScore[1]);
	Swap(&m_AddScore[2], (tdnRandom::Get(0, 1)) ? &m_AddScore[0] : &m_AddScore[1]);
}

void StageManager::SetDogFloor(int floor)
{
	FOR(STAGE_MAX)for (auto &it : m_Doglists[i])
	{
		if (it->IsOpening())it->Change();
	}

	if (floor == 0)
	{
		m_Doglists[0][0]->bEnable = true;
		m_Doglists[0][1]->bEnable = true;
		m_Doglists[1][0]->bEnable = false;
		m_Doglists[1][1]->bEnable = false;
		m_Doglists[2][0]->bEnable = false;
		m_Doglists[2][1]->bEnable = false;
	}
	else if (floor == 1)
	{
		m_Doglists[0][0]->bEnable = false;
		m_Doglists[0][1]->bEnable = false;
		m_Doglists[1][0]->bEnable = true;
		m_Doglists[1][1]->bEnable = true;
		m_Doglists[2][0]->bEnable = false;
		m_Doglists[2][1]->bEnable = false;
	}
	else if (floor == 2)
	{
		m_Doglists[0][0]->bEnable = false;
		m_Doglists[0][1]->bEnable = false;
		m_Doglists[1][0]->bEnable = false;
		m_Doglists[1][1]->bEnable = false;
		m_Doglists[2][0]->bEnable = true;
		m_Doglists[2][1]->bEnable = true;
	}
}

void StageManager::RenderBack()
{
	// 背景描画
	m_pStageImages[StageImage::BACK]->Render(0,0);

	// 家の後ろ描画
	m_pStageImages[StageImage::HOUSE_BACK]->Render(0, 0);

	// 家のドア描画
	m_pStageImages[(g_CreateSheepFloor == 0) ? StageImage::DOOR1_OPEN : StageImage::DOOR1_CLOSE]->Render(0, 0);
	m_pStageImages[(g_CreateSheepFloor == 1) ? StageImage::DOOR2_OPEN : StageImage::DOOR2_CLOSE]->Render(0, 0);
	m_pStageImages[(g_CreateSheepFloor == 2) ? StageImage::DOOR3_OPEN : StageImage::DOOR3_CLOSE]->Render(0, 0);

	// 柵描画
	m_pStageImages[StageImage::SAKU]->Render(0, 0);

	// いけにえ棒描画
	m_pStageImages[StageImage::IKENIE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);

	// 消えた描画
	if (NikuMgr->GetNiku())m_pStageImages[StageImage::KIETA_FIRE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, 0, 0, 256, 256);
}

void StageManager::Render()
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		//static const int col[] = { 0x40ff0000, 0x4000ff00, 0x400000ff };
		//// ステージ幅
		//tdnPolygon::Rect(0, STAGE_POS_Y[i], 1280, LANE_WIDTH, RS::COPY, col[i]);

		stage[i]->Render();

		//tdnPolygon::Rect(0, STAGE_POS_Y[i], 150, LANE_WIDTH, RS::COPY, 0x80ffffff);

		// リキャスト
		tdnText::Draw(64, STAGE_POS_Y[i] + 120, 0xffffffff, "%d", stage[i]->GetRecastTime());
	}
}

void StageManager::RenderFront()
{
	// 犬描画
	FOR(STAGE_MAX) for (auto it : m_Doglists[i]) it->Render();

	// 家の前描画
	m_pStageImages[StageImage::HOUSE_FRONT]->Render(0, 0);

	// 草描画
	m_pStageImages[StageImage::KUSA]->Render(0, 0);

	// 炎描画
	if (!NikuMgr->GetNiku()) m_pStageImages[StageImage::FIRE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);

	// 数字描画
	// 入ったら加算されるスコア
	//tdnText::Draw(1200, STAGE_POS_Y[0] + 0, 0xffffffff, "%d", m_AddScore[0]);
	//tdnText::Draw(1200, STAGE_POS_Y[1] + 0, 0xffffffff, "%d", m_AddScore[1]);
	//tdnText::Draw(1200, STAGE_POS_Y[2] + 270, 0xffffffff, "%d", m_AddScore[2]);
}

void StageManager::Reflection(DataManager* data, MousePointer* mouse)
{
	// shutter check
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)continue;
	}

	// stage apper
	if (data != nullptr)
	{
		data->SetpAddScore(&m_AddScore[0], &m_AddScore[1], &m_AddScore[2]);
	}
}

bool StageManager::IsOpen(int floorIdx)
{
	if (stage[floorIdx] == nullptr)return false;
	return (stage[floorIdx]->state == CurvePoint::Base::CurvePointState::OPEN);
}

Vector2 StageManager::GetArrowPos(int floorIdx)
{
	if (stage[floorIdx] == nullptr)return Vector2(0, 0);
	return Vector2((float)(stage[floorIdx]->GetWidth()/2), (float)(STAGE_POS_Y[floorIdx] + 80));
}

Vector2 StageManager::GetWarningPos(int floorIdx, bool IsRight)
{
	return Vector2((float)(IsRight ? 1200 : 80), (float)(STAGE_POS_Y[floorIdx] + 80));
}

Vector2 StageManager::GetPopupPos(int floorIdx, bool IsRight)
{
	return Vector2((float)(IsRight ? 1200 : 80), (float)(STAGE_POS_Y[floorIdx] + 120));
}

Vector2 StageManager::GetBalloonPos(int floorIdx)
{
	if (stage[floorIdx] == nullptr)return Vector2(0, 0);
	return Vector2((float)(stage[floorIdx]->GetWidth() / 2), (float)(STAGE_POS_Y[floorIdx] + 60));
}

//**************************************************
//    Stage class
//**************************************************

//------- constructor,destructor ---------

Stage::Stage() :
W(1280), H(240),
START_Y(-240), SPEED_Y(32), SHUTTER_X(541), SHUTTER_Y(18), m_RecastTime(0)
{
	image = nullptr;
	pos = Vector2(0, 0);
	state = StageState::NONE;
	offsetY = 0;
}

Stage::~Stage()
{
	// マネージャーのを参照していてるだけなので、削除はマネージャーがする
	//for (auto it : m_CPlist) delete it;
	//m_CPlist.clear();
}

//---------- public method ------------

void Stage::Init(Vector2 pos, Stage::StageState state)
{
	this->state = state;
	this->pos = pos;
	switch (state)
	{
		case StageState::NONE:
			offsetY = 0;
			break;
		case StageState::FALL:
			offsetY = (int)pos.y - START_Y;
			break;
	}
}

void Stage::Update()
{
	// リキャスト減らす処理
	if (m_RecastTime > 0) m_RecastTime--;

	switch (state)
	{
		case StageState::NONE:
			break;
		case StageState::FALL:
			offsetY -= SPEED_Y;
			if (offsetY <= 0)
			{
				offsetY = 0;
				state = StageState::NONE;
			}
			break;
	}
}

void Stage::Render()
{
	//for(auto it : *m_CPlist)it->Render();
}

int Stage::GetWidth()
{
	return W;
}