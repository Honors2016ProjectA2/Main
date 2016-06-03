#include "IEX_Expansion.h"
#include "../CharaBase/CharaBase.h"
#include "../Stage/StageMNG.h"
#include "../Sheep/Sheep.h"
#include "watchman.h"
#include "../Data/DataMNG.h"
#include "../Effect/EffectManager.h"
#include "../Niku/Niku.h"
#include "../Sound/SoundManager.h"
#include "../particle_2d/particle_2d.h"

//**************************************************
//    基底
//**************************************************
void Enemy::Base::MoveUpdate()
{
	m_pos += m_MoveVec * m_speed;
}

//**************************************************
//    狼
//**************************************************
Enemy::Wolf::Wolf(tdn2DObj *image, tdn2DObj *pniku, tdn2DObj *pHone, int floor, float speed, int nikustopTime) : Base(image, floor, speed), m_pNikukutteru(pniku), m_EAT_NIKU_TIMER(nikustopTime), m_OrgSpeed(speed), m_pHoneImage(pHone)
{
	// イニシャライザ書けない
	W = 120;
	H = 120;

	// 座標初期化
	m_pos.x = 1280;
	m_pos.y = (float)STAGE_POS_Y[floor] + LANE_WIDTH / 4;

	// モードポインタ初期亜k
	ModeFunk[(int)MODE::RUN] = &Wolf::Run;
	ModeFunk[(int)MODE::NIKU] = &Wolf::Niku;
	ModeFunk[(int)MODE::DEAD] = &Wolf::Dead;

	// 最初はしる
	ChangeMode(MODE::RUN);
}

void Enemy::Wolf::Update()
{
	// モード関数実行
	(this->*ModeFunk[(int)m_mode])();

	// 座標更新
	Enemy::Base::MoveUpdate();
}

void Enemy::Wolf::Run()
{
	// アニメ更新
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 4) m_AnimePanel = 0;
	}
}


void Enemy::Wolf::Niku()
{
	// アニメ更新
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 4) m_AnimePanel = 0;
	}

	// 肉時間
	if (++m_EatNikuTimer > m_EAT_NIKU_TIMER)
	{
		EffectMgr.AddEffect((int)m_pos.x + 50, (int)m_pos.y + 30, EFFECT_TYPE::ClEAR);
		EffectMgr.AddEffect((int)m_pos.x - 80, (int)m_pos.y + 60, EFFECT_TYPE::PUSH);

		// 煙
		Particle2dManager::Effect_FatSmoke(GetCenterPos());

		// 太る
		EnemyMgr->CreateFatWolf(this, m_type, m_SheepType);
	}
}

void Enemy::Wolf::Dead()
{
	if (m_AnimePanel < 5)
	{
		if (++m_AnimeFrame > 4)
		{
			m_AnimeFrame = 0;
			m_AnimePanel++;
		}
	}
	else
	{
		// α下げていく
		m_alpha -= 2;
		if (m_alpha < 2)
			m_bErase = true;	// 死ぬ
	}
}


void Enemy::Wolf::Render()
{
	tdn2DObj *pImage;
	switch (m_mode)
	{
	case Enemy::Wolf::MODE::RUN:
		pImage = m_pImage;
		break;
	case Enemy::Wolf::MODE::DEAD:
		pImage = m_pHoneImage;
		break;
	case Enemy::Wolf::MODE::NIKU:
		pImage = m_pNikukutteru;
		break;
	}
	pImage->SetARGB(m_alpha, (BYTE)255, (BYTE)255, (BYTE)255);
	pImage->Render((int)m_pos.x, (int)m_pos.y, W, H, (m_AnimePanel % 4) * W, (m_AnimePanel / 4) * H, W, H);
}

//**************************************************
//    肉食って太った狼
//**************************************************
FatWolf::FatWolf(tdn2DObj *image, const Vector2 &pos, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType) :DebuBase(image, pos), m_type(type), m_SheepType(SheepType)
{
	switch (type)
	{
	case FAT_WOLF_TYPE::SMALL:
		m_radius = 90;
		break;
	case FAT_WOLF_TYPE::MIDDLE:
		m_radius = 90;
		break;

	case FAT_WOLF_TYPE::LARGE:
		m_radius = 120;
		break;
	}
}

FatWolf::~FatWolf()
{
	// ループしてるSEを止める
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("太った羊押す", m_ReceiveSE);
}

void FatWolf::Update()
{
	// 基底クラスの更新
	DebuBase::Update();

	if (m_SheepType == SHEEP_TYPE::GOLD)
	{
		static int KiraKiraCoolTime = 0;
		// きらきらパーティクル
		if (++KiraKiraCoolTime > 4)
		{
			KiraKiraCoolTime = 0;
			Particle2dManager::Effect_KiraKira(GetCenterPos(), Vector2(m_radius, m_radius), 25.0f, 10.0f);
		}
	}
}

void FatWolf::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, 0, (int)m_type * 240, 240, 240);
}



//**************************************************

// manager

//**************************************************

EnemyManager::EnemyManager() :m_CreateTimer(0)
{
	m_list.clear();
	m_FatList.clear();

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

	// にく
	ifs >> skip;
	ifs >> m_NikuTime;

	m_NextFloor = tdnRandom::Get(0, 2);
}

void EnemyManager::Initialize()
{
	// 生成時間初期化
	m_CreateTimer = 0;

	// 敵画像の読み込み
	m_pImages[(int)ENEMY_TYPE::WOLF] = new tdn2DObj("DATA/CHR/「！」左移動.png");
	m_pNikukutteru = new tdn2DObj("DATA/CHR/kuruma back.png");
	m_pHoneImage = new tdn2DObj("DATA/CHR/hone_motion.png");
	m_pFatWolfImages[(int)SHEEP_TYPE::NOMAL] = new tdn2DObj("DATA/CHR/sinnnyou tubureru.png");
	m_pFatWolfImages[(int)SHEEP_TYPE::GOLD] = new tdn2DObj("DATA/CHR/sinnnyou hajike.png");
	m_pFatWolfImages[(int)SHEEP_TYPE::REAL] = new tdn2DObj("DATA/CHR/sinnnyou detekuru.png");
}

void EnemyManager::Release()
{
	FOR ((int)ENEMY_TYPE::MAX)
	{
		delete m_pImages[i];
	}
	Clear();
	delete m_pNikukutteru;
	delete m_pHoneImage;
	FOR((int)SHEEP_TYPE::MAX) delete m_pFatWolfImages[i];
}

//**************************************************

void EnemyManager::Create(int floor, ENEMY_TYPE type)
{
	Enemy::Wolf *set = nullptr;

	switch (type)
	{
	case ENEMY_TYPE::WOLF:
		set = new Enemy::Wolf(m_pImages[(int)ENEMY_TYPE::WOLF], m_pNikukutteru, m_pHoneImage, floor, m_EnemySpeed[(int)ENEMY_TYPE::WOLF], m_NikuTime);
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
		Create(m_NextFloor, ENEMY_TYPE::WOLF);

		// 次のフロア作成
		m_NextFloor = tdnRandom::Get(0, 2);
	}
	else if (m_CREATETIME - m_CreateTimer == 80)
	{
		// 肉センサー
		Niku *pNiku = NikuMgr->GetNiku();
		if (pNiku)
		{
			if(pNiku->isSeted())m_NextFloor = pNiku->GetFloor();
		}
		
		// 肉なかったら次はデブ羊フロア
		else if (!g_pSheepMgr->GetFatList()->empty())
		{
			m_NextFloor = (*g_pSheepMgr->GetFatList()->begin())->GetFloor();
		}

		// ポップアップ
		EffectMgr.AddEffect(1100, STAGE_POS_Y[m_NextFloor] + LANE_WIDTH / 2, EFFECT_TYPE::NOTICE);
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

	for (auto it = m_FatList.begin(); it != m_FatList.end();)
	{
		// 敵たち更新
		(*it)->Update();

		// 消去フラグ立ってたら
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_FatList.erase(it);
		}
		else it++;
	}
}

void EnemyManager::Render()
{
	for (auto it: m_list)
		it->Render();
	for (auto it : m_FatList)
		it->Render();
}

void EnemyManager::Clear()
{
	// 敵データ全消去
	for (auto it : m_list) delete it;
	m_list.clear();
	for (auto it : m_FatList)delete it;
	m_FatList.clear();
}

void EnemyManager::CreateFatWolf(Enemy::Wolf *wolf, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType)
{
	wolf->Erase();	// 元の羊を消去
	FatWolf *set = new FatWolf(m_pFatWolfImages[(int)SheepType], Vector2(wolf->GetPos().x - 60, (float)STAGE_POS_Y[wolf->GetFloor()] - 30), type, SheepType);// 太った狼生成
	set->SetFloor(wolf->GetFloor());	// フロア設定
	m_FatList.push_back(set);			// リストに突っ込む
}