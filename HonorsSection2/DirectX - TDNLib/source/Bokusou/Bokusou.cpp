#include "TDNLIB.h"
#include "Bokusou.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIManager.h"

// グローバル変数
int g_ModeChangeTime[(int)BOKUSOU_MODE::MAX];			// モードが変わっていく時間

// 静的メンバ変数
BokusouManager *BokusouManager::pInstance = nullptr;	// 1つしかない実体

//**************************************************
//    牧草管理クラス
//**************************************************
BokusouManager::BokusouManager()
{
	// テキストからデータ読み込み
	std::ifstream ifs("DATA/Text/Param/bokusou.txt");
	assert(ifs);	//　データない

	char skip[64];	// 読み飛ばし用

	ifs >> skip;

	// 牧草花の変化時間読み込み
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::HUTABA];
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::HONBA];
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::TSUBOMI];
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::SAITA];
	g_ModeChangeTime[(int)BOKUSOU_MODE::BORN];				// 牧草状態は時間の必要なし

	// 牧草種生成時間読みこみ
	ifs >> skip;
	ifs >> m_CREATETIME;

	// 牧草種生成座標読み込み
	m_CreatePosList.clear();
	ifs >> skip;
	while (!ifs.eof())
	{
		float x;
		int floor;
		ifs >> x;
		ifs >> floor;

		m_CreatePosList.push_back(CreatePos(Vector2(x, (float)STAGE_POS_Y[floor] + LANE_WIDTH / 4), floor));
	}
}

void BokusouManager::Initialize()
{
	m_CreateTimer = 0;
	m_list.clear();
}

BokusouManager::~BokusouManager()
{
	for (auto it : m_list) delete it;
	m_list.clear();
}

void BokusouManager::Release()
{
	SAFE_DELETE(pInstance);
}

void BokusouManager::Update()
{
	// 牧草の種ゲージ増加
	UIMNG.SetGraph((float)m_CreateTimer / m_CREATETIME);
	if (++m_CreateTimer > m_CREATETIME)
	{
		//A列車 牧草生成した瞬間

		// 牧草生成！！
		m_CreateTimer = 0;

		int r = rand() % m_CreatePosList.size();	// ランダム値
		Bokusou *set = new Bokusou(m_CreatePosList[r].pos);	// 座標リストからランダムに
		set->SetFloor(m_CreatePosList[r].floor);
		m_list.push_back(set);
	}

	for (auto it = m_list.begin(); it != m_list.end();)
	{
		// 牧草更新
		(*it)->Update();

		// 消去チェック
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_list.erase(it);
		}
		else it++;
	}
}

void BokusouManager::Render()
{
	for (auto it : m_list) it->Render();
}


//**************************************************
//    牧草委譲クラス
//**************************************************
void BokusouMode::Base::Update()
{
	// 設定時間過ぎたら
	if (++m_timer > m_limit)
	{
		NextMode();	// 次のモードへ
	}
}

void BokusouMode::Base::Render()
{
	m_pImage->Render(pBokusou->GetPosX(), pBokusou->GetPosY(), 128, 128, m_srcX, 0, 128, 128);
}

void BokusouMode::Base::NextMode()
{
	// 次のモードに変更
	pBokusou->ChangeMode(m_NextMode);
}

//===========================================================
//		双葉杏
//===========================================================
BokusouMode::Hutaba::Hutaba(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// 本葉に行く
	m_NextMode = BOKUSOU_MODE::HONBA;

	// 画像座標
	m_srcX = 0;

	// SEの再生
	se->Play("牧草芽生えた", me->GetPos());
}
//void BokusouMode::Hutaba::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		本葉未央
//===========================================================
BokusouMode::Honba::Honba(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// つぼみに行く
	m_NextMode = BOKUSOU_MODE::TSUBOMI;

	// 画像座標
	m_srcX = 128;

	// SEの再生
	se->Play("牧草成長", me->GetPos());
}
//void BokusouMode::Honba::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		つぼみ
//===========================================================
BokusouMode::Tsubomi::Tsubomi(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// 咲いたに行く
	m_NextMode = BOKUSOU_MODE::SAITA;

	// 画像座標
	m_srcX = 256;

	// SEの再生
	se->Play("牧草成長", me->GetPos());
}
//void BokusouMode::Tsubomi::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}


//===========================================================
//		さいた―
//===========================================================
BokusouMode::Saita::Saita(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// そして牧場が誕生した
	m_NextMode = BOKUSOU_MODE::BORN;

	// 画像座標
	m_srcX = 128 * 3;

	// SEの再生
	se->Play("牧草成長", me->GetPos());
}
//void BokusouMode::Saita::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		牧草誕生
//===========================================================
BokusouMode::Born::Born(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// 画像座標
	m_srcX = 0;

	// SEの再生
	se->Play("牧草成長", me->GetPos());
}
void BokusouMode::Born::Update()
{

}

//**************************************************
//    牧草クラス
//**************************************************

//===========================================================
//		初期化・解放
//===========================================================
Bokusou::Bokusou(const Vector2 &pos) :m_pMode(nullptr), m_bErase(false), m_pos(pos)
{
	// 画像初期化
	m_pBokusouFlower = new tdn2DObj("DATA/牧草/牧草の種.png");
	m_pBokusouRoll = new tdn2DObj("DATA/牧草/牧草.png");

	// モード初期化(双葉杏から開始)
	this->ChangeMode(BOKUSOU_MODE::HUTABA);
}

Bokusou::~Bokusou()
{
	delete m_pMode;
	delete m_pBokusouFlower;
	delete m_pBokusouRoll;
}


//===========================================================
//		更新
//===========================================================
void Bokusou::Update()
{
	m_pMode->Update();
}



//===========================================================
//		描画
//===========================================================
void Bokusou::Render()
{
	m_pMode->Render();
}

void Bokusou::ChangeMode(BOKUSOU_MODE m)
{
	if (m_pMode)delete m_pMode;

	switch (m)
	{
	case BOKUSOU_MODE::HUTABA:
		m_pMode = new BokusouMode::Hutaba(this, m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::HONBA:
		m_pMode = new BokusouMode::Honba(this, m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::TSUBOMI:
		m_pMode = new BokusouMode::Tsubomi(this, m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::SAITA:
		m_pMode = new BokusouMode::Saita(this, m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::BORN:
		m_pMode = new BokusouMode::Born(this, m_pBokusouRoll);
		break;
	}

	// モードが変わる時間設定
	m_pMode->SetChangeModeTime(g_ModeChangeTime[(int)m]);
}