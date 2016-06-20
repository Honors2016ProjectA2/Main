#include "TDNLIB.h"
#include "../Sheep/Sheep.h"
#include "Niku.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "Effect\EffectManager.h"
#include "../particle_2d/particle_2d.h"
#include "../TipsCount/TipsCounter.h"

// 静的メンバ変数
NikuManager *NikuManager::pInstance = nullptr;	// 1つしかない実体

int g_YakiNikuModeChangeTime[(int)YAKINIKU_MODE::MAX];			// モードが変わっていく時間

//**************************************************
//    牧草管理クラス
//**************************************************
NikuManager *NikuManager::GetInstance()
{
	if (!pInstance) pInstance = new NikuManager;
	return pInstance;
}

NikuManager::NikuManager() :m_pNiku(nullptr), m_pYakiniku(nullptr), m_pNikuBazier(nullptr)
{
	m_pIkenieImages = new tdn2DObj*[(int)SHEEP_TYPE::MAX];
	m_pNikuImages = new tdn2DObj*[(int)SHEEP_TYPE::MAX];

	// テキストからデータ読み込み
	std::ifstream ifs("DATA/Text/Param/niku.txt");
	assert(ifs);

	char skip[64];	// 読み飛ばし用

	// 画像パス読み込み
	FOR((int)SHEEP_TYPE::MAX)
	{
		char path[MAX_PATH];

		// いけにえ画像
		ifs >> skip;
		ifs >> path;
		m_pIkenieImages[i] = new tdn2DObj(path);

		// 肉画像
		ifs >> path;
		m_pNikuImages[i] = new tdn2DObj(path);
	}
	
	// 肉焼け時間
	ifs >> skip;
	FOR((int)YAKINIKU_MODE::KOGETA)
	{
		ifs >> skip;
		ifs >> g_YakiNikuModeChangeTime[i];
	}
}

void NikuManager::Initialize()
{
	m_bHoldNiku = false;
}

NikuManager::~NikuManager()
{
	SAFE_DELETE(m_pNiku);
	SAFE_DELETE(m_pYakiniku);
	FOR((int)SHEEP_TYPE::MAX)
	{
		SAFE_DELETE(m_pNikuImages[i]);
		SAFE_DELETE(m_pIkenieImages[i]);
	}
	delete[] m_pNikuImages;
	delete[] m_pIkenieImages;
}

void NikuManager::Release()
{
	SAFE_DELETE(pInstance);
}

void NikuManager::Update()
{
	// 肉ベジエ
	if (m_pNikuBazier)
	{
		m_pNikuBazier->Update();
		static float fireAngle = 0.0f;
		fireAngle -= 0.5f;
		m_pNikuBazier->GetPic()->SetAngle(fireAngle);
		// ベジエ終わったら
		if(m_pNikuBazier->IsEnd())
		{
			// ベジエ消して
			delete m_pNikuBazier; m_pNikuBazier = nullptr;
			// 肉消す(消えたら火がつくようになっている)
			delete m_pNiku, m_pNiku = nullptr;

			// 火エフェクト
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 128, (int)YAKINIKU_AREA.y + 138, EFFECT_TYPE::BURN);

			// SE
			se->Play("点火", YAKINIKU_AREA);
		}
	}
	// 肉
	if (m_pNiku)
	{
		// 消えてたらスルー
		if (m_pNiku->isVisible()) return;

		// 肉掴んでたら
		if (m_bHoldNiku)
		{
			Vector2 mPos = tdnMouse::GetPos() + Vector2(-64, -64);

			// 肉が端に行きすぎないように補正
			mPos.x = Math::Clamp(mPos.x, 120, 1000);

			// 座標を設定
			m_pNiku->SetPos(mPos);

			// マウス離したら
			if (tdnMouse::GetLeft() == 2)
			{
				// 掴みフラグOFF
				m_bHoldNiku = false;

				// 最短フロア計算
				int floor = FindFloor(mPos.y);
				m_pNiku->SetFloor(floor);

				// Y座標補正
				m_pNiku->SetPosY((float)STAGE_POS_Y[floor] + (float)(LANE_WIDTH / 8));

				// 肉設置！！
				m_pNiku->Set();
			}
		}

		// 肉掴んでない状態
		else
		{
			// マウス押したら
			if (tdnMouse::GetLeft() == 3)
			{
				// 肉置場との距離範囲内
				if ((tdnMouse::GetPos() - m_pNiku->GetCenterPos()).LengthSq() < 64 * 64)
				{
					// 肉の移動量消す
					m_pNiku->MoveOff();

					// 肉掴みフラグON
					m_bHoldNiku = true;
					se->Play("牧草成長", m_pNiku->GetCenterPos());

					// 設置しない
					m_pNiku->UnSet();
				}
			}
		}

		m_pNiku->Update();

		// 消去チェック
		if (m_pNiku->EraseOK())
		{
			delete m_pNiku, m_pNiku = nullptr;
		}
	}

	else if (m_pYakiniku)
	{
		static int HinokoFrame = 0;
		// 火の粉パーティクル
		if (++HinokoFrame > 1)
		{
			HinokoFrame = 0;
			Particle2dManager::Effect_Hinoko(YAKINIKU_AREA + Vector2(128, 128));
		}

		m_pYakiniku->Update();

		// マウス左クリックで肉作成
		if (tdnMouse::GetLeft() == 3)
		{
			Vector2 mPos = tdnMouse::GetPos();
			// 焼肉エリア内なら
			if (mPos.x > YAKINIKU_AREA.x + 30 && mPos.x < YAKINIKU_AREA.x + 30 + 190 && mPos.y > YAKINIKU_AREA.y + 96 && mPos.y < YAKINIKU_AREA.y + 96 + 100)
			{
				// 肉作る
				CreateNiku();
			}
		}

		//else if (m_pYakiniku->GetMode() == YAKINIKU_MODE::KOGETA)
		//{
		//	// 焦げ肉自動生成
		//	CreateNiku();
		//}

		// 消去チェックt
		if (m_pYakiniku->EraseOK())
		{
			delete m_pYakiniku, m_pYakiniku = nullptr;
		}
	}
}

void NikuManager::RenderYakiniku()
{
	if (m_pYakiniku)m_pYakiniku->Render();
}

void NikuManager::RenderNiku()
{
	if (m_pNiku)m_pNiku->Render();
}

void NikuManager::RenderFront()
{
	if (m_pYakiniku)m_pYakiniku->RenderFront();
	if (m_pNiku)m_pNiku->RenderFront();
	if (m_pNikuBazier)m_pNikuBazier->Render();
}


void NikuManager::StartYakiniku(SHEEP_TYPE type)
{
	m_pYakiniku = new Yakiniku(m_pIkenieImages[(int)type], type);

	// SEの再生
	se->Play("焼けた", YAKINIKU_AREA);
	se->Play("悲鳴", YAKINIKU_AREA);
}

void NikuManager::StartNikuBazier()
{
	// 肉無い
	MyAssert(m_pNiku, "あるはずの肉がない");

	// 肉消す(実体は消さず、表示だけ)
	m_pNiku->SetVisible();

	// 開始と終了座標
	Vector3 start(m_pNiku->GetCenterPos().x, m_pNiku->GetCenterPos().y, 0), end(YAKINIKU_AREA.x + 128, YAKINIKU_AREA.y + 128, 0);
	
	// 開始座標の位置に応じて、座標を算出
	Vector3 center, center2;

	float PowerLR = 420, PowerUD = 150;

	// 真ん中レーンじゃなかったら
	if (m_pNiku->GetFloor() != 1)
	{								/*	左		右	*/
		PowerLR = (start.x < 640) ? PowerLR : -PowerLR;
									/*	下		上	*/
		PowerUD = (start.y < 320) ? PowerUD : -PowerUD;

		center = Vector3(start.x + PowerLR, start.y - PowerUD, 0);
		center2 = Vector3(center.x + PowerLR * 1.85f, center.y + PowerUD, 0);
	}
	// 真ん中レーン
	else
	{
		PowerLR = (start.x < 640) ? 260.0f : -260.0f;
		PowerUD = start.y;

		center = Vector3(start.x + PowerLR, start.y - PowerUD, 0);
		center2 = Vector3(center.x + PowerLR * 2.6f, PowerUD - 100, 0);
	}


	// 肉ベジエ生成
	m_pNikuBazier = new NikuBazier("DATA/powerF.png", start, center, center2, end, 46);
	m_pNikuBazier->Action();
}

// ここが肉を生成する瞬間
void NikuManager::CreateNiku()
{
	if (m_pYakiniku)
	{
		// 焼肉消して肉作る
		m_pNiku = new Niku(YAKINIKU_AREA + Vector2(128, 90), m_pYakiniku->GetMode(), m_pNikuImages[(int)m_pYakiniku->GetSheepType()], m_pYakiniku->GetSheepType());
		

		// タイプに応じてエフェクト発動！
		switch (m_pYakiniku->GetMode())
		{
		case YAKINIKU_MODE::NAMA:
		case YAKINIKU_MODE::KOGETA:
			// BADエフェクト
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 128, (int)YAKINIKU_AREA.y + 64, EFFECT_TYPE::BAD);
			// SEの再生
			se->Play("BAD", YAKINIKU_AREA);
			break;
		case YAKINIKU_MODE::RARE:
			// GREATエフェクト
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 64, (int)YAKINIKU_AREA.y + 24, EFFECT_TYPE::GREAT);
			// SEの再生
			se->Play("GREAT", YAKINIKU_AREA);
			break;
		case YAKINIKU_MODE::MEDIAM:
			// PERFECTエフェクト
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 64, (int)YAKINIKU_AREA.y + 32, EFFECT_TYPE::PERFECT);
			// SEの再生
			se->Play("PERFECT", YAKINIKU_AREA);
			break;
		}
		
		// 肉の焼き加減Tipsカウント
		TipsCountMgr->m_YakinikuSteps[(int)m_pYakiniku->GetMode()]++;
		// 何の羊の肉かTipsカウント
		TipsCountMgr->m_NikuSheepTypes[(int)m_pYakiniku->GetSheepType()]++;

		m_pYakiniku->Erase();
	}
}


//**************************************************
//    焼き肉クラス
//**************************************************
//**************************************************
//    焼肉委譲クラス
//**************************************************
void YakinikuMode::Base::Update()
{
	// 設定時間過ぎたら
	if (++m_timer > m_limit)
	{
		NextMode();	// 次のモードへ
	}
}

void YakinikuMode::Base::Render()
{
	m_pYakiniku->pImage->Render(m_pYakiniku->GetPosX(), m_pYakiniku->GetPosY(), 256, 256, m_srcX, 0, 256, 256);
}

void YakinikuMode::Base::RenderFront()
{
	m_pYakiniku->pImage->SetAlpha(64);
	m_pYakiniku->pImage->Render(m_pYakiniku->GetPosX(), m_pYakiniku->GetPosY(), 256, 256, m_srcX, 0, 256, 256);
	m_pYakiniku->pImage->SetAlpha(255);
}

void YakinikuMode::Base::NextMode()
{
	// 次のモードに変更
	m_pYakiniku->ChangeMode(m_NextMode);
}

//===========================================================
//		生
//===========================================================
YakinikuMode::Nama::Nama(Yakiniku *me) :Base(me)
{
	// レアに行く
	m_NextMode = YAKINIKU_MODE::RARE;

	// 画像座標
	m_srcX = 0;

	// エフェクト発動
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::BAD);

	// SEの再生
	//se->Play("牧草芽生えた", me->GetPos());
}
//void YakinikuMode::Nama::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		レア ←　いい感じの肉
//===========================================================
YakinikuMode::Rare::Rare(Yakiniku *me) :Base(me)
{
	// ミディアムに行く
	m_NextMode = YAKINIKU_MODE::MEDIAM;

	// 画像座標
	m_srcX = 256;

	// エフェクト発動
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::GREAT);

	// SEの再生
	//se->Play("牧草成長", me->GetPos());
}
//void YakinikuMode::Rare::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		ミディアム ←（超高校級の肉）
//===========================================================
YakinikuMode::Mediam::Mediam(Yakiniku *me) :Base(me)
{
	// こげたに行く
	m_NextMode = YAKINIKU_MODE::KOGETA;

	// 画像座標
	m_srcX = 256 * 2;

	// エフェクト発動
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PERFECT);

	// SEの再生
	se->Play("今です", me->GetPos());

	EffectMgr.AddEffect(me->GetPosX() + 128, me->GetPosY() + 120, EFFECT_TYPE::PUT);

}
//void YakinikuMode::Mediam::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}


//===========================================================
//		ウェルダン
//===========================================================
//YakinikuMode::Weldan::Weldan(Yakiniku *me) :Base(me)
//{
//	// そして牧場が誕生した
//	m_NextMode = YAKINIKU_MODE::KOGETA;
//
//	// 画像座標
//	m_srcX = 256 * 3;
//
//	// SEの再生
//	//se->Play("牧草成長", me->GetPos());
//
//}
//void YakinikuMode::Weldan::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		焦げた
//===========================================================
YakinikuMode::Kogeta::Kogeta(Yakiniku *me) :Base(me), m_KiraKiraCoolTime(0)
{
	// 画像座標
	m_srcX = 256 * 3;

	// SEの再生
	//->Play("牧草成長", me->GetPos());

	// エフェクト発動
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::BAD);
}
void YakinikuMode::Kogeta::Update()
{
	// きらきらパーティクル
	//if (++m_KiraKiraCoolTime > 4)
	//{
	//	m_KiraKiraCoolTime = 0;
	//	Particle2dManager::Effect_KiraKira(m_pYakiniku->GetCenterPos(), Vector2(56, 56), 15.0f, 10.0f);
	//}
}

//**************************************************
//    焼肉クラス
//**************************************************

//===========================================================
//		初期化・解放
//===========================================================
Yakiniku::Yakiniku(tdn2DObj *image, SHEEP_TYPE SheepType) :m_SheepType(SheepType), m_pMode(nullptr), m_bErase(false), m_pos(YAKINIKU_AREA), pImage(image)
{
	// モード初期化(双葉杏から開始)
	this->ChangeMode(YAKINIKU_MODE::NAMA);
}

Yakiniku::~Yakiniku()
{
	delete m_pMode;
}


//===========================================================
//		更新
//===========================================================
void Yakiniku::Update()
{
	m_pMode->Update();
}



//===========================================================
//		描画
//===========================================================
void Yakiniku::Render()
{
	m_pMode->Render();
}

void Yakiniku::RenderFront()
{
	m_pMode->RenderFront();
}

void Yakiniku::ChangeMode(YAKINIKU_MODE m)
{
	if (m_pMode)delete m_pMode;

	switch (m)
	{
	case YAKINIKU_MODE::NAMA:
		m_pMode = new YakinikuMode::Nama(this);
		break;

	case YAKINIKU_MODE::RARE:
		m_pMode = new YakinikuMode::Rare(this);
		break;

	case YAKINIKU_MODE::MEDIAM:
		m_pMode = new YakinikuMode::Mediam(this);
		break;

	//case YAKINIKU_MODE::WELDAN:
	//	m_pMode = new YakinikuMode::Weldan(this);
	//	break;

	case YAKINIKU_MODE::KOGETA:
		m_pMode = new YakinikuMode::Kogeta(this);
		break;
	}

	// モードが変わる時間設定
	m_pMode->SetChangeModeTime(g_YakiNikuModeChangeTime[(int)m]);
}


//**************************************************
//    肉クラス
//**************************************************

//===========================================================
//		初期化・解放
//===========================================================
Niku::Niku(const Vector2 &pos, YAKINIKU_MODE type, tdn2DObj *image, SHEEP_TYPE SheepType) :m_SheepType(SheepType), m_pos(pos), m_bErase(false), m_orgY(pos.y),
m_BoundPow(Vector2(6, -8)), m_gravity(1.0f), m_pImage(image), m_type(type), m_floor(FindFloor(pos.y)), m_bSet(false), m_bVisible(false)
{
	m_move = m_BoundPow;
	// Y座標補正
	//m_pos.y = (float)STAGE_POS_Y[m_floor] + (float)(LANE_WIDTH / 8);
}


//===========================================================
//		更新
//===========================================================
void Niku::Update()
{
	if (m_BoundPow.y != 0)
	{
		// 重力処理
		m_move.y += m_gravity;

		m_pos += m_move;

		if (m_pos.y > m_orgY)
		{
			if ((m_BoundPow *= 0.75f).Length() < 0.6f)m_BoundPow = Vector2(0, 0);
			m_move = m_BoundPow;
		}
	}

	// 超高校級の肉なら
	if (m_type == YAKINIKU_MODE::MEDIAM)
	{
		// 湯気パーティクル
		Particle2dManager::Effect_NikuSmoke(m_pos + Vector2(60, 60));
	}
}



//===========================================================
//		描画
//===========================================================
void Niku::Render()
{
	if(!m_bVisible)m_pImage->Render((int)m_pos.x, (int)m_pos.y, 120, 120, (int)m_type * 120, 0, 120, 120);
}

void Niku::RenderFront()
{
	if (!m_bVisible)
	{
		m_pImage->SetARGB(0x80ffffff);
		m_pImage->Render((int)m_pos.x, (int)m_pos.y, 120, 120, (int)m_type * 120, 0, 120, 120);
		m_pImage->SetARGB(0xffffffff);
	}
}





void NikuBazier::Move()
{
	// レートを足していく
	m_percent = min(m_percent + m_percentRate, 1.0f);

	// ぱーてぃくる
	Particle2dManager::Effect_KiraKira(Vector2(m_pos.x, m_pos.y), Vector2(16, 16), 15, 10, 1, 70);// ぱーてぃくる
	FOR(4)Particle2dManager::Effect_Hinoko(Vector2(m_pos.x, m_pos.y));
}