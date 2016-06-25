#include "TDNLIB.h"
#include "Bokusou.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIManager.h"
#include "Effect\EffectManager.h"
#include "../particle_2d/particle_2d.h"
#include "../pie_graph/pie_graph.h"

// グローバル変数
int g_ModeChangeTime[(int)BOKUSOU_MODE::MAX];			// モードが変わっていく時間

// 静的メンバ変数
BokusouManager *BokusouManager::pInstance = nullptr;	// 1つしかない実体

//**************************************************
//    牧草管理クラス
//**************************************************
BokusouManager::BokusouManager():m_PrevPoint(0)
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

	// 次の生成座標
	m_NextPoint = tdnRandom::Get(0, m_CreatePosList.size() - 1);
}

void BokusouManager::Initialize()
{
	m_CreateTimer = 0;
	m_list.clear();

	m_pBokusouTarget = new BokusouTarget;
	m_pBokusouTarget->ChangePos(m_CreatePosList[m_NextPoint].pos);
}

BokusouManager::~BokusouManager()
{
	for (auto it : m_list) delete it;
	m_list.clear();
	delete m_pBokusouTarget;
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
		// 牧草生成した瞬間
		//EffectMgr.AddEffect((int)m_CreatePosList[m_NextPoint].pos.x+64, (int)m_CreatePosList[m_NextPoint].pos.y+64, EFFECT_TYPE::PUT);
		UIMNG.GraphAction();//  アクション

		m_CreateTimer = 0;

		// 生成
		//Bokusou *set = new Bokusou(m_CreatePosList[m_NextPoint].pos, m_NextPoint);	// 座標リストからランダムに
		//set->SetFloor(m_CreatePosList[m_NextPoint].floor);
		//m_list.push_back(set);

		m_PrevPoint = m_NextPoint;

		//m_CreatePosList[m_NextPoint].pos
		UIMNG.AddSendPower("Data/Power.png", Vector3(40, 40, 0), Vector3(0, 400, 0), Vector3(100, 600, 0),
			Vector3(m_CreatePosList[m_NextPoint].pos.x + 64, m_CreatePosList[m_NextPoint].pos.y + 64, 0), 48, 114514, true);

		// 次の生成座標
		while (true)
		{
			m_NextPoint = tdnRandom::Get(0, m_CreatePosList.size() - 1);
			if (m_NextPoint == m_PrevPoint) continue;

			// かぶってないフロア検索
			bool find(true);
			for (auto &it : m_list)
			{
				if (it->GetPoint() == m_NextPoint)
				{
					find = false;
					break;
				}
			}

			if (find) break;
		}

		// ターゲットの位置変更
		//m_pBokusouTarget->ChangePos(m_CreatePosList[m_NextPoint].pos);
	}

	m_pBokusouTarget->Update(this);

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

	m_pBokusouTarget->Render();
}

void BokusouManager::RenderFront()
{
	for (auto it : m_list) it->RenderFront();
	m_pBokusouTarget->RenderFront();

	//if (KeyBoard(KB_1))
	//{
	//	for (UINT i = 0; i < m_CreatePosList.size(); i++)
	//	{
	//		tdnPolygon::Rect(m_CreatePosList[i].pos.x, m_CreatePosList[i].pos.y, 128, 128, RS::COPY, 0xffffffff);
	//	}
	//}
}

void BokusouManager::CreateByBazier()
{
	// エフェクト
	EffectMgr.AddEffect((int)m_CreatePosList[m_PrevPoint].pos.x + 64, (int)m_CreatePosList[m_PrevPoint].pos.y + 64, EFFECT_TYPE::PUT);

	// 生成
	Bokusou *set = new Bokusou(m_CreatePosList[m_PrevPoint].pos, m_PrevPoint);
	set->SetFloor(m_CreatePosList[m_PrevPoint].floor);
	m_list.push_back(set);

	// ターゲットの位置変更
	m_pBokusouTarget->ChangePos(m_CreatePosList[m_NextPoint].pos);
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

void BokusouMode::Base::RenderFront()
{
	m_pImage->SetARGB(0x80ffffff);
	m_pImage->Render(pBokusou->GetPosX(), pBokusou->GetPosY(), 128, 128, m_srcX, 0, 128, 128);
	m_pImage->SetARGB(0xffffffff);
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
	//se->Play("牧草芽生えた", me->GetPos());
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
	//se->Play("牧草芽生えた", me->GetPos());
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
	//se->Play("牧草芽生えた", me->GetPos());

}
//void BokusouMode::Saita::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		牧草誕生
//===========================================================
BokusouMode::Born::Born(Bokusou *me, tdn2DObj *image) :Base(me, image), m_KiraKiraCoolTime(0)
{
	// 画像座標
	m_srcX = 0;

	// SEの再生
	se->Play("牧草芽生えた", me->GetPos());

	// エフェクト発動
	EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PLUS);

}
void BokusouMode::Born::Update()
{
	// きらきらパーティクル
	if (++m_KiraKiraCoolTime > 4)
	{
		m_KiraKiraCoolTime = 0;
		Particle2dManager::Effect_KiraKira(pBokusou->GetCenterPos(), Vector2(56, 56), 15.0f, 10.0f);
	}
}

//**************************************************
//    牧草クラス
//**************************************************

//===========================================================
//		初期化・解放
//===========================================================
Bokusou::Bokusou(const Vector2 &pos, int point) :m_point(point), m_pMode(nullptr), m_bErase(false), m_pos(pos), m_pBokusouFlower(new tdn2DObj("DATA/牧草/牧草の種.png")), m_pBokusouRoll(new tdn2DObj("DATA/牧草/牧草.png"))
{
	// モード初期化(牧草から開始)
	this->ChangeMode(BOKUSOU_MODE::BORN);
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

BokusouTarget::BokusouTarget() :m_pKusa(new tdn2DObj("DATA/牧草/牧草の種.png")), m_pos(Vector2(0, 0)), m_angle(0), m_alpha(0), m_AnimFrame(0),
ANIM_FRAME(30)
{
	// グラフ
	m_pGraph = new Pie_graph("DATA/UI/Circle/SpiritCircle_gage2.png");

	m_pTargetAnim = new tdn2DAnim("DATA/traget.png");
	m_pTargetAnim->OrderJump(ANIM_FRAME, .65f, .2f);
	m_pTargetAnim->Action();
}

BokusouTarget::~BokusouTarget()
{
	delete m_pKusa;
	delete m_pTargetAnim;
	delete m_pGraph;
}

void BokusouTarget::Update(BokusouManager *pMgr)
{
	// くさα加算処理
	m_alpha = min(m_alpha + 8, 128);
	m_pKusa->SetARGB(m_alpha, (BYTE)255, (BYTE)255, (BYTE)255);

	// 牧草しょぼくなる対策
	if(m_srcX < 3 * 128)m_srcX = ((int)(pMgr->GetGaugePercentage() * 100) / 25) * 128;

	// ゲージ処理
	m_pGraph->Set_percent(pMgr->GetGaugePercentage());

	// ターゲットのアニメーション
	m_pTargetAnim->Update();
	if (++m_AnimFrame >= ANIM_FRAME * 2)
	{
		m_AnimFrame = 0;
		m_pTargetAnim->Action();
	}
}

void BokusouTarget::Render()
{
	// グラフ
	//m_pGraph->Render((int)m_pos.x, (int)m_pos.y, 128, 128, 0, 0, 128, 128);

	// 草
	m_pKusa->Render((int)m_pos.x, (int)m_pos.y, 128, 128, m_srcX, 0, 128, 128);
	
	// ターゲット
	//m_pTargetAnim->Render((int)m_pos.x - 6, (int)m_pos.y);
}

void BokusouTarget::RenderFront()
{
	// 草
	m_pKusa->SetARGB(0x80ffffff);
	m_pKusa->Render((int)m_pos.x, (int)m_pos.y, 128, 128, m_srcX, 0, 128, 128);
	m_pKusa->SetARGB(0xffffffff);

	// ターゲット
	//m_pTargetAnim->SetARGB(0x80ffffff);
	//m_pTargetAnim->Render((int)m_pos.x - 6, (int)m_pos.y);
	//m_pTargetAnim->SetARGB(0xffffffff);
}

void BokusouTarget::ChangePos(const Vector2 &pos)
{
	m_pos = pos;
	m_alpha = 0;
	m_angle = 0;
	m_srcX = 0;
}