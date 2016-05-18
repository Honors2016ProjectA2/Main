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

//-------- static field ---------

//------- constructor,destructor ---------

StageManager::StageManager()
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		stageImage[i] = nullptr;
		stage[i] = nullptr;
	}
	floor = 0;

	// image load
	stageImage[0] = new tdn2DObj("DATA/Stage/Game Stage01.png");
	stageImage[1] = new tdn2DObj("DATA/Stage/Game Stage02.png");
	stageImage[2] = new tdn2DObj("DATA/Stage/Game Stage03.png");

	m_pDogImage = new tdn2DObj("DATA/CHR/dog.png");

	// テキストから情報取得
	std::ifstream ifs("DATA/Text/Param/stage.txt");

	char skip[64];	// 読み飛ばし用
	ifs >> skip;
	FOR(3)
	{
		// レーン解放羊数
		ifs >> APPEND_STAGE_BORDER[i];
	}

	ifs >> skip;

	FOR(3)
	{
		// レーンスコア数
		ifs >> m_AddScore[i];
	}

	ifs >> skip;

	// 犬設置ポイント読み込み
	FOR(STAGE_MAX) m_CPlists[i].clear();
	while (!ifs.eof())
	{
		int posX, floor;
		ifs >> floor;
		ifs >> posX;

		// 犬リスト格納
		CurvePointRadio *set = new CurvePointRadio;
		set->p[0] = new CurvePoint(m_pDogImage, Vector2((float)posX, (float)STAGE_POS_Y[floor] - 25), DIR::UP);
		set->p[1] = new CurvePoint(m_pDogImage, Vector2((float)posX, (float)STAGE_POS_Y[floor + 1] - 25), DIR::DOWN);

		m_CPlists[floor].push_back(set->p[0]);
		m_CPlists[floor + 1].push_back(set->p[1]);

		m_CPRadios.push_back(set);
	}

	// 本来はスコアでレーンを追加していくが、今回は最初から3レーン
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)
			stage[i] = new Stage;
		stage[i]->Init(stageImage[i], Vector2((float)0, (float)STAGE_POS_Y[i]), Stage::StageState::FALL, &m_CPRadios);
	}
}

StageManager::~StageManager()
{
	delete m_pDogImage;

	for (int i = 0; i < STAGE_MAX; i++)
	{
		SAFE_DELETE(stageImage[i]);
		SAFE_DELETE(stage[i]);
	}
	// 犬リストの開放
	for (auto it : m_CPRadios)
	{
		delete it;
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
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)continue;
		stage[i]->Update();
		for (auto it : m_CPlists[i]) it->Update();
	}
}

void StageManager::Render()
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)continue;
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

void Stage::Init(tdn2DObj* stage, Vector2 pos, Stage::StageState state, std::vector<CurvePointRadio*> *CPlist)
{
	image = stage;
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
	// マウス座標と自分の座標で判定をとる
	Vector2 mPos = tdnMouse::GetPos();

	for (auto it : *m_CPlist)
	{
		for (int i = 0; i < 2; i++)
		{
			static const int AIKATA[2] = { 1, 0 };
			// マウスの中に入ってるかどうかの判定
			if (mPos.x >= it->p[i]->GetPos().x && mPos.x <= it->p[i]->GetPos().x + it->p[i]->GetWidth() &&
				mPos.y >= it->p[i]->GetPos().y && mPos.y <= it->p[i]->GetPos().y + it->p[i]->GetWidth())
			{
				// 初回入ったフレーム
				if (!it->p[i]->m_bCursorIn)
				{
					it->p[i]->m_bCursorIn = true;
				}
			}
			else it->p[i]->m_bCursorIn = false;

			// 左クリック！！
			if (tdnMouse::GetLeft() == 3 && it->p[i]->m_bCursorIn)
			{
				it->p[i]->Change();	// 犬のON_OFF
				// 排他処理
				if (it->p[AIKATA[i]]->IsOpening()) it->p[AIKATA[i]]->Change();
			}
		}
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