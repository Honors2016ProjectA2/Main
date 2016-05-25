#include "TDNLIB.h"
#include "IEX_Expansion.h"
#include "../Stage/StageMNG.h"
#include "watchman.h"
#include "../Data/DataMNG.h"


//**************************************************
//    基底
//**************************************************
void Enemy::Base::MoveUpdate()
{
	m_pos += m_MoveVec*m_speed;
}

//**************************************************
//    狼
//**************************************************
Enemy::Wolf::Wolf(tdn2DObj *image, int floor, float speed) :Base(image, floor, speed)
{
	// イニシャライザ書けない
	W = 120;
	H = 120;
	// 移動の向き(左)
	m_MoveVec = Vector2(-1, 0);

	// 座標初期化
	m_pos.x = 1280;
	m_pos.y = (float)STAGE_POS_Y[floor] + LANE_WIDTH / 4;
}

void Enemy::Wolf::Update()
{
	// アニメ更新
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 3) m_AnimePanel = 0;
	}

	// 座標更新
	Enemy::Base::MoveUpdate();
}

void Enemy::Wolf::Render()
{
	m_pImage->Render((int)m_pos.x, (int)m_pos.y, W, H, (m_AnimePanel % 4) * W, (m_AnimePanel / 4) * H, W, H);
}

//**************************************************
//    メテオ
//**************************************************
Enemy::Meteo::Meteo(tdn2DObj *image, int floor, float speed) :Base(image, floor, speed)
{
	// イニシャライザ書けない
	W = 120;
	H = 120;
	// 移動の向き(左)
	m_MoveVec = Vector2(-1, 0);
}

void Enemy::Meteo::Update()
{
	// 座標更新
	Enemy::Base::MoveUpdate();
}

void Enemy::Meteo::Render()
{
	m_pImage->Render((int)m_pos.x, (int)m_pos.y);
}

//**************************************************

// manager

//**************************************************

EnemyManager::EnemyManager() :m_CreateTimer(0)
{
	m_list.clear();

	// テキストからデータ読み込み
	std::fstream ifs("DATA/Text/Param/tekidata.txt");
	assert(ifs);	// ファイルがない！

	char skip[64];	// 読み飛ばし用変数

	// 敵作成時間読み込み
	ifs >> skip;
	ifs >> m_CREATETIME;

	// 狼の速度読み込み
	ifs >> skip;
	ifs >> m_EnemySpeed[(int)ENEMY_TYPE::WOLF];
}

void EnemyManager::Initialize()
{
	// 生成時間初期化
	m_CreateTimer = 0;

	// 敵画像の読み込み
	m_pImages[(int)ENEMY_TYPE::WOLF] = new tdn2DObj("DATA/CHR/「！」左移動.png");
	m_pImages[(int)ENEMY_TYPE::METEO] = new tdn2DObj("DATA/CHR/「！」左移動.png");
}

void EnemyManager::Release()
{
	FOR ((int)ENEMY_TYPE::MAX)
	{
		delete m_pImages[i];
	}
	Clear();
}

//**************************************************

void EnemyManager::Create(int floor, ENEMY_TYPE type)
{
	Enemy::Base *set = nullptr;

	switch (type)
	{
	case ENEMY_TYPE::WOLF:
		set = new Enemy::Wolf(m_pImages[(int)ENEMY_TYPE::WOLF], floor, m_EnemySpeed[(int)ENEMY_TYPE::WOLF]);
		break;
	case ENEMY_TYPE::METEO:
		set = new Enemy::Meteo(m_pImages[(int)ENEMY_TYPE::METEO], floor, m_EnemySpeed[(int)ENEMY_TYPE::METEO]);
		break;
	default:
		assert(0);	// 例外処理
		break;
	}
	m_list.push_back(set);
}

void EnemyManager::Update()
{
	// 敵生成タイマー
	if (++m_CreateTimer > m_CREATETIME)
	{
		m_CreateTimer = 0;
		Create(rand() % 3, ENEMY_TYPE::WOLF);
	}
	for (auto it = m_list.begin(); it != m_list.end();)
	{
		// 敵たち更新
		(*it)->Update();

		// 消去フラグ立ってたら
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_list.erase(it); 
		}
		else it++;
	}
}

void EnemyManager::Render()
{
	for (auto it: m_list) it->Render();
}

void EnemyManager::Clear()
{
	// 敵データ全消去
	for (auto it : m_list) delete it;
	m_list.clear();
}
