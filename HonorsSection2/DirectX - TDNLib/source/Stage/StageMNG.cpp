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
#include "../TipsCount/TipsCounter.h"

//int g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MAX];			// モードが変わっていく時間

// 家の座標
static const int HOUSE_POS_Y[STAGE_MAX] =
{
	116, 290, 500
};

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
Vector2 YAKINIKU_AREA(442, 362);
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

StageManager::StageManager(bool bTutorial) :m_pDogImage(new tdn2DObj("DATA/CHR/dog.png")), m_pFireImage(new tdn2DObj("DATA/巻き/炎の種.png")), m_bTutorial(bTutorial), m_bDogUpdate(true),
m_FireAnimFrame(0), m_FireAnimPanel(0), m_ChangeScoreTime(0), m_pBatuImage(new tdn2DObj("DATA/batu.png")), m_pNumber(new tdn2DObj("DATA/Number/Number.png")), m_pFireFlash(new FireFlash),
m_bHouseUpdate(true)
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
		stage[i]->Init(Vector2((float)0, (float)HOUSE_POS_Y[i]), Stage::StageState::FALL);
	}
}

StageManager::~StageManager()
{
	FOR(StageImage::MAX)delete m_pStageImages[i];
	delete m_pDogImage;
	delete m_pFireImage;
	delete m_pBatuImage;
	FOR(STAGE_MAX)
	{
		SAFE_DELETE(stage[i]);

		// 犬リストの開放
		for (auto it : m_Doglists[i]) delete it;

		// 炎リストの開放
		//for (auto it : m_Firelists[i]) delete it;
	}
	delete m_pNumber;
	delete m_pFireFlash;
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

	// 火の点滅更新
	m_pFireFlash->Update();

	// 小屋のリキャスト関連
	int floor = -1;
	for (int i = 0; i < STAGE_MAX; i++)
	{
		int y = (int)mPos.y;

		// マウスカーソル範囲内！
		if (y > STAGE_POS_Y[i] && y < STAGE_POS_Y[i] + LANE_WIDTH && mPos.x < 150)
		{
			floor = i;

			// 初回フレーム
			if (!stage[i]->m_bPoint)
			{

			}
			stage[i]->m_bPoint = true;
		}
		else
		{
			// 範囲内じゃない
			stage[i]->m_bPoint = false;
		}
	}

	if (m_bHouseUpdate)
	{
		if (tdnMouse::GetLeft() == 3)
		{
			// 羊生成の小屋を変える処理
			if (floor != -1 && floor != g_CreateSheepFloor)
			{
				// リキャストOK
				if (stage[floor]->GetRecastTime() <= 0)
				{
					// リキャスト設定
					stage[g_CreateSheepFloor]->SetRecastTime(m_RECAST_TIME);

					// 羊生成フロア変える
					g_CreateSheepFloor = floor;

					// SEの再生
					se->Play("ドア", Vector2(128, (float)STAGE_POS_Y[floor] + LANE_WIDTH / 2));

					// 開くエフェクト
					EffectMgr.AddEffect(96, STAGE_POS_Y[floor] + LANE_WIDTH / 2, EFFECT_TYPE::SMOKE);
					EffectMgr.AddEffect(96, STAGE_POS_Y[floor] + LANE_WIDTH / 2, EFFECT_TYPE::INEFFECT_MINI);

					// 犬配置リセット
					m_DogStock = 2;

					// 犬レーン変更
					SetDogFloor(floor);

					stage[floor]->m_TenmetsuFrame = 0;

					// Tipsカウント
					TipsCountMgr->m_LaneChange++;
				}
			}
		}
	}

	// 時間経過でスコアを変える処理
	//if (++m_ChangeScoreTime > m_CHANGE_SCORE_TIME)
	//{
	//	// レーン変える処理
	//	m_ChangeScoreTime = 0;
	//	ChangeScoreLane();
	//}

	// 犬関連の更新
	if(m_bDogUpdate)UpdateDogs(mPos);

	// ステージのアニメーション
	if (++m_FireAnimFrame > 4)
	{
		m_FireAnimFrame = 0;
		if (++m_FireAnimPanel > 2)m_FireAnimPanel = 0;
	}
}

void StageManager::UpdateDogs(const Vector2 &mPos)
{
	static int MouseLeftFrame = 0;
	// マウスのフレーム
	MouseLeftFrame = (tdnMouse::GetLeft() == 0) ? 0 : MouseLeftFrame + 1;


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

				// 左マウス押して20フレーム以内に離したら！！かつカーソル範囲内かつ有効状態かつ肉持ってなかったらなら
				if ((tdnMouse::GetLeft() == 2 && MouseLeftFrame <= 12) && it->m_bCursorIn && it->bEnable && !NikuMgr->isNikuHold())
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
							EffectMgr.AddEffect((int)it->GetPos().x + 64, (int)it->GetPos().y + 64, EFFECT_TYPE::DOG_EFFECT);
							se->Play("犬", it->GetPos());
							m_DogStock--;
							it->Change();	// 犬のON_OFF
							// 犬設置したフレームかどうか
							g_bDogSetFrame = true;

							// 犬設置フラグ
							DogFlag = DOG::SET;

							// Tipsカウント
							TipsCountMgr->m_DogUse++;
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
					else if (g_CreateSheepFloor == 2) m_Doglists[2][0]->bEnable = true;
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

	// 家描画
	FOR(STAGE_MAX)
	{
		// 羊出てる家
		if (i == g_CreateSheepFloor)
		{
			m_pStageImages[StageImage::DOOR_OPEN3]->Render(0, HOUSE_POS_Y[i]);
			m_pStageImages[StageImage::DOOR_OPEN2]->Render(0, HOUSE_POS_Y[i]);
			m_pStageImages[StageImage::DOOR_OPEN1]->Render(0, HOUSE_POS_Y[i]);
		}
		// 閉まってる家
		else
		{
			// 下地
			m_pStageImages[StageImage::DOOR_CLOSE]->SetARGB(0x80101010);
			m_pStageImages[StageImage::DOOR_CLOSE]->SetScale(1.0f);
			m_pStageImages[StageImage::DOOR_CLOSE]->Render(0, HOUSE_POS_Y[i]);

			// ゲージ部分
			const float GaugePercent = (max(stage[i]->GetRecastTime()-60,0) / (float)m_RECAST_TIME);
			// ゲージMAXif文
			if (GaugePercent <= .0f)
			{
				m_pStageImages[StageImage::DOOR_CLOSE]->SetScale(stage[i]->m_bPoint ? 1.05f : 1.0f);
			}
			m_pStageImages[StageImage::DOOR_CLOSE]->SetARGB(0xffffffff);
			m_pStageImages[StageImage::DOOR_CLOSE]->Render(0, HOUSE_POS_Y[i] + (int)(180 * GaugePercent), 180, (int)((1 - GaugePercent) * 180), 0, (int)(180 * GaugePercent), 180, (int)((1 - GaugePercent) * 180));

			// 点滅加算合成
			if (GaugePercent <= .0f)
			{
				m_pStageImages[StageImage::DOOR_CLOSE]->SetARGB((BYTE)(160 * ((float)stage[i]->m_TenmetsuFrame / stage[i]->TENMETSU)), (BYTE)255, (BYTE)255, (BYTE)255);
				m_pStageImages[StageImage::DOOR_CLOSE]->Render(0, HOUSE_POS_Y[i], RS::ADD);
			}
			else
			{
				if (stage[i]->m_bPoint)
				{
					// ×描画
					static bool up = true;
					static float scale = .7f;
					static const float add = .01f;
					if (up)
					{
						if ((scale += add) > 1.0f) up = false;
					}
					else
					{
						if ((scale -= add) < .7f) up = true;
					}
					m_pBatuImage->SetScale(scale);
					m_pBatuImage->Render(32, HOUSE_POS_Y[i] + 16);

					// リキャストタイム
					m_pNumber->Render(64, HOUSE_POS_Y[i] + 54, 64, 64, stage[i]->GetRecastTime() / 60 * 64, 0, 64, 64);
				}
			}
		}
	}

	// 柵描画
	m_pStageImages[StageImage::SAKU]->Render(0, 0);

	// いけにえ棒描画
	m_pStageImages[StageImage::IKENIE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);
	// 羊掴んだら点滅
	if (g_pSheepMgr->isHoldSheep() && !NikuMgr->GetNiku() && !NikuMgr->GetYakiniku())
	{
		m_pStageImages[StageImage::IKENIE]->SetARGB((0x00ffffff | ((BYTE)(255 * ((float)m_pFireFlash->frame / m_pFireFlash->TURN)) << 24)));	// α処理
		m_pStageImages[StageImage::IKENIE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256, RS::ADD);
		m_pStageImages[StageImage::IKENIE]->SetARGB(0xffffffff);
	}

	// 消えた描画
	if (NikuMgr->GetNiku())m_pStageImages[StageImage::KIETA_FIRE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, 0, 0, 256, 256);
}

void StageManager::Render()
{
	//FOR(STAGE_MAX)
	//{
		//static const int col[] = { 0x40ff0000, 0x4000ff00, 0x400000ff };
		//// ステージ幅
		//tdnPolygon::Rect(0, STAGE_POS_Y[i], 1280, LANE_WIDTH, RS::COPY, col[i]);

		//stage[i]->Render();

		//tdnPolygon::Rect(0, STAGE_POS_Y[i], 150, LANE_WIDTH, RS::COPY, 0x80ffffff);

		// リキャスト
		//tdnText::Draw(64, STAGE_POS_Y[i] + 120, 0xffffffff, "%d", stage[i]->GetRecastTime());
	//}
}

void StageManager::RenderFront()
{
	// 犬描画
	FOR(STAGE_MAX)
	{
		for (auto it : m_Doglists[i]) it->Render();

		// 家りっぷる
		stage[i]->Render();
	}

	// 右家の前描画
	m_pStageImages[StageImage::HOUSE_FRONT]->Render(0, 0);

	// 炎描画
	if (!NikuMgr->GetNiku())
	{
		m_pStageImages[StageImage::FIRE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);

		// 羊掴んだら点滅
		if (g_pSheepMgr->isHoldSheep() && !NikuMgr->GetNiku() && !NikuMgr->GetYakiniku())
		{
			m_pStageImages[StageImage::FIRE]->SetARGB((0x00ffffff | ((BYTE)(255 * ((float)m_pFireFlash->frame / m_pFireFlash->TURN)) << 24)));	// α処理
			m_pStageImages[StageImage::FIRE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256, RS::ADD);	// α処理
			m_pStageImages[StageImage::FIRE]->SetARGB(0xffffffff);
		}

	}

	// 数字描画
	// 入ったら加算されるスコア
	//tdnText::Draw(1200, STAGE_POS_Y[0] + 0, 0xffffffff, "%d", m_AddScore[0]);
	//tdnText::Draw(1200, STAGE_POS_Y[1] + 0, 0xffffffff, "%d", m_AddScore[1]);
	//tdnText::Draw(1200, STAGE_POS_Y[2] + 270, 0xffffffff, "%d", m_AddScore[2]);
}

void StageManager::RenderFront2()
{
	FOR(STAGE_MAX)for (auto& it : m_Doglists[i]) it->RenderFront();
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
W(1280), H(240), m_bPoint(false),
START_Y(-240), SPEED_Y(32), SHUTTER_X(541), SHUTTER_Y(18), m_RecastTime(0), m_TenmetsuFrame(0), TENMETSU(60), m_bTenmtsuUp(true)
{
	m_pHouseRipple = new tdn2DAnim("DATA/Stage/doa_simaruRipple.png");
	m_pHouseRipple->OrderRipple(16, 1.0f, .05f);

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
	delete m_pHouseRipple;
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
	if (m_RecastTime > 0)
	{
		// リキャスト溜まった瞬間！！
		if (--m_RecastTime <= 60)
		{
			m_RecastTime = 0;
			// エフェクト
			//EffectMgr.AddEffect((int)pos.x + 68, (int)pos.y + 72, EFFECT_TYPE::PUT);

			// SE
			se->Play("リキャスト", pos);

			m_pHouseRipple->Action();
		}
	}
	else
	{
		// 小屋の点滅
		if (m_bTenmtsuUp)
		{
			if ((m_TenmetsuFrame += m_bPoint ? 4 : 1) >= TENMETSU)
			{
				m_bTenmtsuUp = !m_bTenmtsuUp;
				m_TenmetsuFrame = TENMETSU;
			}
		}
		else
		{
			if ((m_TenmetsuFrame -= m_bPoint ? 4 : 1) <= 0)
			{
				m_bTenmtsuUp = !m_bTenmtsuUp;
				m_TenmetsuFrame = 0;
			}
		}
	}

	m_pHouseRipple->Update();

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
	m_pHouseRipple->Render(0, (int)pos.y, RS::ADD);
	m_pHouseRipple->Render(0, (int)pos.y, RS::ADD);
}

int Stage::GetWidth()
{
	return W;
}