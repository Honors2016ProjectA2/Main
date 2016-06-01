#include "TDNLIB.h"
#include "Niku.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "Effect\EffectManager.h"
#include "../particle_2d/particle_2d.h"

// 静的メンバ変数
NikuManager *NikuManager::pInstance = nullptr;	// 1つしかない実体

int g_YakiNikuModeChangeTime[(int)YAKINIKU_MODE::MAX];			// モードが変わっていく時間

//**************************************************
//    牧草管理クラス
//**************************************************
NikuManager::NikuManager() :m_pNiku(nullptr), m_pYakiniku(nullptr), m_pNikuImage(new tdn2DObj("DATA/CHR/!-kai2.png")), m_pIkenieImage(new tdn2DObj("DATA/CHR/kuruma2.png"))
{
	// テキストからデータ読み込み
	std::ifstream ifs("DATA/Text/Param/niku.txt");
	assert(ifs);

	char skip[64];	// 読み飛ばし用
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
	SAFE_DELETE(m_pNikuImage);
	SAFE_DELETE(m_pYakiniku);
	SAFE_DELETE(m_pIkenieImage);
}

void NikuManager::Release()
{
	SAFE_DELETE(pInstance);
}

void NikuManager::Update()
{
	if (m_pNiku)
	{
		// まだ肉設置してなかったら
		if (!m_pNiku->isSeted())
		{
			// 肉掴んでたら
			if (m_bHoldNiku)
			{
				Vector2 mPos = tdnMouse::GetPos() + Vector2(-64, -64);
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
						// 肉掴みフラグON
						m_bHoldNiku = true;
					}
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
		m_pYakiniku->Update();

		// マウス左クリックで肉作成
		if (tdnMouse::GetLeft() == 3)
		{
			// 焼肉エリア内なら
			if ((tdnMouse::GetPos() - (YAKINIKU_AREA + Vector2(128, 128))).LengthSq() < 128 * 128)
			{
				// 肉作る
				CreateNiku();
			}
		}

		// 消去チェックt
		if (m_pYakiniku->EraseOK())
		{
			delete m_pYakiniku, m_pYakiniku = nullptr;
		}
	}
}

void NikuManager::Render()
{
	if (m_pNiku)m_pNiku->Render();
	else if (m_pYakiniku)m_pYakiniku->Render();
}


void NikuManager::StartYakiniku()
{
	m_pYakiniku = new Yakiniku(m_pIkenieImage);

	// SEの再生
	se->Play("焼けた", YAKINIKU_AREA);
	se->Play("悲鳴", YAKINIKU_AREA);
}

void NikuManager::CreateNiku()
{
	if (m_pYakiniku)
	{
		// 焼肉消して肉作る
		m_pNiku = new Niku(YAKINIKU_AREA + Vector2(128, 90), m_pYakiniku->GetMode(), m_pNikuImage);
		m_pYakiniku->Erase();
	}
}


//**************************************************
//    焼き肉クラス
//**************************************************
//**************************************************
//    牧草委譲クラス
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
	// 本葉に行く
	m_NextMode = YAKINIKU_MODE::RARE;

	// 画像座標
	m_srcX = 0;

	// SEの再生
	//se->Play("牧草芽生えた", me->GetPos());
}
//void YakinikuMode::Nama::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		レア
//===========================================================
YakinikuMode::Rare::Rare(Yakiniku *me) :Base(me)
{
	// つぼみに行く
	m_NextMode = YAKINIKU_MODE::MEDIAM;

	// 画像座標
	m_srcX = 256;

	// SEの再生
	//se->Play("牧草成長", me->GetPos());
}
//void YakinikuMode::Rare::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		ミディアム
//===========================================================
YakinikuMode::Mediam::Mediam(Yakiniku *me) :Base(me)
{
	// 咲いたに行く
	m_NextMode = YAKINIKU_MODE::KOGETA;

	// 画像座標
	m_srcX = 256 * 2;

	// SEの再生
	//se->Play("牧草成長", me->GetPos());
}
//void YakinikuMode::Mediam::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}


//===========================================================
//		ウェルダン
//===========================================================
YakinikuMode::Weldan::Weldan(Yakiniku *me) :Base(me)
{
	// そして牧場が誕生した
	m_NextMode = YAKINIKU_MODE::KOGETA;

	// 画像座標
	m_srcX = 256 * 3;

	// SEの再生
	//se->Play("牧草成長", me->GetPos());

}
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
	se->Play("牧草成長", me->GetPos());

	// エフェクト発動
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PLUS);
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
//    牧草クラス
//**************************************************

//===========================================================
//		初期化・解放
//===========================================================
Yakiniku::Yakiniku(tdn2DObj *image) :m_pMode(nullptr), m_bErase(false), m_pos(YAKINIKU_AREA), pImage(image)
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

	case YAKINIKU_MODE::WELDAN:
		m_pMode = new YakinikuMode::Weldan(this);
		break;

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
Niku::Niku(const Vector2 &pos, YAKINIKU_MODE type, tdn2DObj *image) :m_pos(pos), m_bErase(false), m_orgY(pos.y),
m_BoundPow(Vector2(6, -8)), m_gravity(1.0f), m_pImage(image), m_type(type), m_floor(FindFloor(pos.y)), m_bSet(false)
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
}



//===========================================================
//		描画
//===========================================================
void Niku::Render()
{
	m_pImage->Render((int)m_pos.x, (int)m_pos.y);
}