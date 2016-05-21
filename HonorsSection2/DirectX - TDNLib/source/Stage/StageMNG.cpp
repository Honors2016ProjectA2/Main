//------------ include --------------
#include "StageMNG.h"
#include "TDNLIB.h"
#include "Data/DataMNG.h"
#include "MousePointer.h"
#include "../Collision/Collision.h"
#include "../Enemy/watchman.h"
#include "../Sound/SoundManager.h"

//**************************************************
//    StageManager class
//**************************************************

//========================================
// グローバル領域
int STAGE_POS_Y[3] =
{
	0, 0, 0
};
int LANE_WIDTH = 0;


//-------- static field ---------

//------- constructor,destructor ---------

StageManager::StageManager()
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		stage[i] = nullptr;
	}
	floor = 0;

	// image load


	m_pDogImage = new tdn2DObj("DATA/CHR/dog.png");

	// テキストから情報取得
	std::ifstream ifs("DATA/Text/Param/stage.txt");

	char skip[64];	// 読み飛ばし用

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

	// 犬ストック個数読み込み
	ifs >> skip;
	ifs >> m_CPStock;


	// 犬設置ポイント読み込み
	FOR(STAGE_MAX) m_CPlists[i].clear();
	ifs >> skip;
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

		m_CPlists[floor].push_back(new CurvePoint(m_pDogImage, Vector2((float)posX, (float)STAGE_POS_Y[floor] - (LANE_WIDTH / 4)), dir));
	}

	// 本来はスコアでレーンを追加していくが、今回は最初から3レーン
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)
			stage[i] = new Stage;
		stage[i]->Init(Vector2((float)0, (float)STAGE_POS_Y[i]), Stage::StageState::FALL, &m_CPlists[i]);
	}
}

StageManager::~StageManager()
{
	delete m_pDogImage;

	for (int i = 0; i < STAGE_MAX; i++)
	{
		SAFE_DELETE(stage[i]);

		// 犬リストの開放
		for (auto it : m_CPlists[i]) delete it;
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
	// マウス座標
	Vector2 mPos = tdnMouse::GetPos();

	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)continue;
		stage[i]->Update();

		// 犬リスト更新
		for (auto it : m_CPlists[i])
		{
			it->Update();

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
					m_CPStock++;	// ストック回復
				}

				// 犬設置
				else
				{
					// ストック残ってたら
					if (m_CPStock > 0)
					{
						m_CPStock--;
					}
					else return;	// 残ってなかったら出ていけぇ！！
				}

				it->Change();	// 犬のON_OFF
			}
		}
	}
}

void StageManager::Render()
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		static const int col[] = { 0x40ff0000, 0x4000ff00, 0x400000ff };
		// ステージ幅
		tdnPolygon::Rect(0, STAGE_POS_Y[i], 1280, LANE_WIDTH, RS::COPY, col[i]);

		stage[i]->Render();

		for (auto it : m_CPlists[i]) it->Render();

		// 入ったら加算されるスコア
		tdnText::Draw(1200, STAGE_POS_Y[i] + 120, 0xffffffff, "%d", m_AddScore[i]);
	}
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
		if (floor < STAGE_MAX - 1)
		{
			// score check
			if (data->GetScore() >= APPEND_STAGE_BORDER[floor])
			{
				floor++;
				//if (stage[floor] == nullptr)
				//	stage[floor] = new Stage;
				//stage[floor]->Init(stageImage[floor], shutterImage[floor], lockImage, Vector2((float)0, (float)STAGE_POS_Y[floor]), Stage::StageState::FALL);
			}
		}
	}
}

bool StageManager::IsOpen(int floorIdx)
{
	if (stage[floorIdx] == nullptr)return false;
	return (stage[floorIdx]->state == CurvePoint::CurvePointState::OPEN);
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

void Stage::Init(Vector2 pos, Stage::StageState state, std::vector<CurvePoint*> *CPlist)
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

	this->m_CPlist = CPlist;	// ポインタ参照
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