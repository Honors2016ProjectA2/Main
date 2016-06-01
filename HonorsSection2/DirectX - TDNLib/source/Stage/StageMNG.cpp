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

//int g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MAX];			// モードが変わっていく時間

//**************************************************
//    StageManager class
//**************************************************

//========================================
// グローバル領域
int STAGE_POS_Y[3] =
{
	0, 190, 520// ★ここいじったろ
};
int LANE_WIDTH = 0;
Vector2 YAKINIKU_AREA(0, 0);

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

StageManager::StageManager() :m_pDogImage(new tdn2DObj("DATA/CHR/dog.png")), m_pFireImage(new tdn2DObj("DATA/巻き/炎の種.png")), m_FireSelect(false), m_FireAnimFrame(0), m_FireAnimPanel(0)
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

	ifs >> skip;

	FOR(3)
	{
		// レーンスコア数
		ifs >> m_AddScore[i];
	}

	//// 火の変化時間読み込み
	//ifs >> skip;
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MAKI];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOEHAJIME];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOETEKITA];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOETA];
	//g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::ENABLE];				// 完全燃焼モード
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::HITED];

	m_FireStock = 1;

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
	Reset();
}

void StageManager::Reset()
{
	// stage all clear
	//for (int i = 0; i < STAGE_MAX; i++)
	//{
	//	SAFE_DELETE(stage[i]);
	//}

	// stage set
	//floor = 0;
	//stage[floor] = new Stage;
	//stage[floor]->Init(stageImage[floor], Vector2((float)0, (float)STAGE_POS_Y[floor]), Stage::StageState::NONE);
}

void StageManager::Update()
{
	// ホイール
	//if (tdnMouse::GetWheelFlag() != WHEEL_FLAG::NONE) m_FireSelect = !m_FireSelect;	// 反転

	// マウス座標
	Vector2 mPos = tdnMouse::GetPos();

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

				// 左クリック！！
				if (tdnMouse::GetLeft() == 3 && it->m_bCursorIn)
				{
					// 犬回収
					if (it->IsOpening())
					{
						se->Play("犬", it->GetPos());
						m_DogStock++;	// ストック回復
						it->Change();	// 犬のON_OFF
						return;
					}

					// 犬設置
					else
					{
						// ストック残ってたら
						if (m_DogStock > 0 && !m_FireSelect)
						{
							EffectMgr.AddEffect((int)it->GetPos().x+64, (int)it->GetPos().y+64,EFFECT_TYPE::DOG_EFFECT);
							se->Play("犬", it->GetPos());
							m_DogStock--;
							it->Change();	// 犬のON_OFF
						}
					}

				}
			}
			//else it->m_bCursorIn = false;

		}	// 犬リスト

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

void StageManager::RenderBack()
{
	// 背景描画
	m_pStageImages[StageImage::BACK]->Render(0,0);

	// 家の後ろ描画
	m_pStageImages[StageImage::HOUSE_BACK]->Render(0, 0);

	// 柵描画
	m_pStageImages[StageImage::SAKU]->Render(0, 0);

	// いけにえ棒描画
	m_pStageImages[StageImage::IKENIE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);
}

void StageManager::Render()
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		//static const int col[] = { 0x40ff0000, 0x4000ff00, 0x400000ff };
		//// ステージ幅
		//tdnPolygon::Rect(0, STAGE_POS_Y[i], 1280, LANE_WIDTH, RS::COPY, col[i]);

		stage[i]->Render();

		// 犬と炎の描画
		for (auto it : m_Doglists[i]) it->Render();
		//for (auto it : m_Firelists[i]) it->Render();

		// 入ったら加算されるスコア
		tdnText::Draw(1200, STAGE_POS_Y[i] + 120, 0xffffffff, "%d", m_AddScore[i]);
	}
}

void StageManager::RenderFront()
{
	// 家の前描画
	m_pStageImages[StageImage::HOUSE_FRONT]->Render(0, 0);

	// 家のドア描画

	// 草描画
	m_pStageImages[StageImage::KUSA]->Render(0, 0);

	// 炎描画
	m_pStageImages[StageImage::FIRE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);
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
START_Y(-240), SPEED_Y(32), SHUTTER_X(541), SHUTTER_Y(18)
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
	if (image != nullptr)
	{
		image->Render((int)pos.x, (int)pos.y - offsetY);
	}

	//for(auto it : *m_CPlist)it->Render();
}

int Stage::GetWidth()
{
	return W;
}