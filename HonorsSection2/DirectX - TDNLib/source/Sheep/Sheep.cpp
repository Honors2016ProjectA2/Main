#include "IEX_Expansion.h"
#include "../Sound/SoundManager.h"
#include "MousePointer.h"
#include "../CurvePoint/CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "Sheep.h"
#include "../Data/DataMNG.h"
#include "../system/system.h"
#include "../particle_2d/particle_2d.h"
#include "../Niku/Niku.h"
#include "../Number/Number.h"
#include "../UI/UIManager.h"

// グローバル領域
int g_CreateSheepFloor;	// 羊を生成するフロア

Sheep::Base::Base(const SheepData &data, int floor, const SheepTextParam &textparam) :
animepos(0, 0),
m_floor(floor),
process(WALK),
m_AnimePanel(0), m_bErase(false)
{
	Move[MODE::WALK] = &Sheep::Base::Walk;
	Move[MODE::CURVE] = &Sheep::Base::Curve;
	Move[MODE::PUSH] = &Sheep::Base::Push;
	Move[MODE::CRUSHED] = &Sheep::Base::Crushed;
	Move[MODE::CAUGHT] = &Sheep::Base::Caught;

	alpha = 255;
	frame = 0, animeframe = 0;
	//move = 5;/* speed */

	this->ase.animex = 0;

	memcpy_s(&m_data, sizeof(SheepData), &data, sizeof(SheepData));
	m_data.Image = data.Image;
	m_data.BoneImage = data.BoneImage;

	col_check = false;

	int addY = tdnRandom::Get(-textparam.VerticalWidth, textparam.VerticalWidth);	// 上下ちょっとずらす用

	pos = Vector2((float)textparam.startX, (float)STAGE_POS_Y[floor] + (float)(LANE_WIDTH / 8) - (float)addY);
	C_MOVE.x = (float)textparam.speed;
	C_MOVE.y = 0;
}

Sheep::Base::~Base()
{}

//**************************************************

void Sheep::Base::Get_out()
{
	// Initialize的な役割なのかな
	animeframe++;
	if (animeframe > 6)
	{
		process = WALK;
		//obj = file[WALK];
		animeframe = 0;
	}
}
void Sheep::Base::Walk()
{
	// 砂煙パーティクル
	Vector2 pPos = GetCenterPos();
	pPos.y += 32;
	Particle2dManager::Effect_SandCloud(pPos);

	// あにめ
	if (++animeframe > m_data.Animkankaku)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	// 本来の移動値
	move = C_MOVE;
	pos.x += move.x;
}

void Sheep::Base::Curve()
{
	// 砂煙パーティクル
	Vector2 pPos = GetCenterPos();
	pPos.y += 32;
	Particle2dManager::Effect_SandCloud(pPos);

	// あにめ
	if (++animeframe > m_data.Animkankaku)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	const float pow = C_MOVE.Length();					// 普段自分が持ってる移動の力
	const float addAngle = 0.011f * C_MOVE.Length();		// サインカーブを曲げるのに使う

	// サインカーブ的なので羊の群れを曲げる
	move = Vector2(cosf(m_sinAngle) * pow, sinf(m_sinAngle) * pow);
	if (m_CurveDir == DIR::UP) move.y *= -1;
	pos += move;

	// 90度曲がって戻る時
	if (m_bTurned)
	{
		// 90度曲がり終わったら、直線モードに戻る
		if ((m_sinAngle -= addAngle) <= 0) process = MODE::WALK;
	}

	// 90度曲がる前
	else
	{
		// 90度曲がり終わったら、曲がったフラグON
		if ((m_sinAngle += addAngle) >= PI * 0.5f) m_bTurned = true;
	}
	// フロア計算
	for (int i = STAGE_MAX - 1; i >= 0; i--)
	{
		if (pos.y >= STAGE_POS_Y[i])
		{
			m_floor = i;
			break;
		}
	}
}

void Sheep::Base::Push()
{
	// あにめ
	if (++animeframe > m_data.Animkankaku)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	// 本来の移動値
	move = C_MOVE;
	pos.x += move.x;
}
void Sheep::Base::Crushed()
{
	if (m_AnimePanel < 5)
	{
		if (++animeframe > 4)
		{
			animeframe = 0;
			m_AnimePanel++;
		}
	}
	else
	{
		alpha -= 2;
		if (alpha < 2)
			m_bErase = true;
	}
}
void Sheep::Base::Caught()
{
	// あにめ
	if (++animeframe > m_data.Animkankaku / 2)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	// マウス座標
	pos = tdnMouse::GetPos() + Vector2(-60, -60);

	// 火の座標

	// マウス離された瞬間
	bool hanashita;
	if (m_bLeftCatch) hanashita = (tdnMouse::GetLeft() == 2);
	else hanashita = (tdnMouse::GetRight() == 2);

	if (hanashita)
	{
		// 歩くモードに戻る
		process = MODE::WALK;

		// 最短フロアにいる
		m_floor = FindFloor(pos.y);
		pos.y = (float)STAGE_POS_Y[m_floor] + (float)(LANE_WIDTH / 8);
	}
}

void Sheep::Base::Update()
{
	(this->*Move[process])();
}

void Sheep::Base::Get_frame_pos(Vector2 &pos, int animePanel)
{
	pos.x = (float)(animePanel % 4);
	pos.y = (float)(animePanel / 4);

	pos.x *= m_data.SIZE;
	pos.y *= m_data.SIZE;
}

void Sheep::Base::Render()
{
	//Get_frame_pos(animepos, m_AnimePanel);

	if (process == WALK)
	{
		/* 距離と比例して透明にする */
		//許して、美しく綺麗に纏める処理考えるの面倒だったんだ。。。動くコード is びゅーてぃふる
		if( move.x > 0 ){
			shader2D->SetValue("moveAngle", .0f);
		}else{
			shader2D->SetValue("moveAngle", 0.125f);
		}
		//byu.byu->Render((int)(pos.x + ((move.x > 0) ? -64 : 64)), (int)pos.y, SIZE, SIZE, 0, 0, 128, 128, shader2D, "byun");
//		byu.byu->Render((int)(pos.x + ((move > 0) ? -64 : 64)), (int)pos.y, SIZE, SIZE, byu.animex, 0, 128, 128);
		//ase.ase->Render((int)(pos.x + ((move.x > 0) ? -32 : 32)), (int)pos.y, SIZE, SIZE, ase.animex, 0, (move.x > 0) ? -128 : 128, 512);
	}

	// 生きてる
	if (process != CRUSHED)
	{
		//　掴まれてる
		if (process == CAUGHT)
		{
			m_data.CatchImage->SetARGB((BYTE)255, (BYTE)255, (BYTE)255, (BYTE)255);
			m_data.CatchImage->Render((int)pos.x, (int)pos.y + 32, m_data.SIZE, m_data.SIZE,
				(m_AnimePanel % 4) * m_data.SIZE, (m_AnimePanel / 4) * m_data.SIZE, -m_data.SIZE, m_data.SIZE,
				RS::COPY);
		}
		else
		{
			m_data.Image->SetARGB((BYTE)255, (BYTE)255, (BYTE)255, (BYTE)255);
			m_data.Image->Render((int)pos.x, (int)pos.y, m_data.SIZE, m_data.SIZE,
				(m_AnimePanel % 4) * m_data.SIZE, (m_AnimePanel / 4) * m_data.SIZE, -m_data.SIZE, m_data.SIZE,
				RS::COPY);
		}
	}

	// 死んでる
	else
	{
		m_data.BoneImage->SetARGB(alpha, (BYTE)255, (BYTE)255, (BYTE)255);
		m_data.BoneImage->Render((int)pos.x, (int)pos.y, m_data.SIZE, m_data.SIZE,
			m_AnimePanel * m_data.SIZE, 0, (move.x >= 0) ? -m_data.SIZE : m_data.SIZE, m_data.SIZE,
			RS::COPY);
	}
}

void Sheep::Base::Be_crushed()
{
	if (col_check) return;

	m_AnimePanel = animeframe = 0;
	process = CRUSHED; 
	//obj = file[CRUSHED];
}
void Sheep::Base::Be_caught(bool bLeft)
{ 
	m_bLeftCatch = bLeft;
	process = CAUGHT; 
	//obj = file[RAN_OVER];
}


//**************************************************
//	金羊
//**************************************************
Sheep::Gold::Gold(const SheepData &data, int floor, const SheepTextParam &textparam) :Base(data, floor, textparam)
{
	// 音鳴らす
	m_seID = se->Play("きらめく羊さん", pos, Vector2(0, 0), true);
}
Sheep::Gold::~Gold()
{
	if(m_seID != TDNSOUND_PLAY_NONE)se->Stop("きらめく羊さん", m_seID);
}
void Sheep::Gold::Update()
{
	// 基底クラスのアップデート
	Sheep::Base::Update();

	// 音の座標設定
	if(m_seID != TDNSOUND_PLAY_NONE)se->SetPos("きらめく羊さん", m_seID, pos);

	// きらきらエフェクト
	if (process != CRUSHED && process != PUSH)Particle2dManager::Effect_KiraKira(GetCenterPos(), Vector2(32, 32), 10.0f, 5.0f, 1, 90);
}


//**************************************************
//	リアル羊
//**************************************************
void Sheep::Real::Update()
{
	// 基底クラスのアップデート
	Sheep::Base::Update();

	// エフェクト
	if (process != PUSH && process != CRUSHED)Particle2dManager::Effect_RealSheep(GetCenterPos() + Vector2(130, -5));
}


//**************************************************
//	牧草食って太った羊
//**************************************************
FatSheep::FatSheep(tdn2DObj *image, const Vector2 &pos, SHEEP_TYPE type) :DebuBase(image, pos, 120), m_type(type)
{

}

FatSheep::~FatSheep()
{
	// ループしてるSEを止める
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("太った羊押す", m_ReceiveSE);
}


//**************************************************

// manager

//**************************************************

SheepManager::SheepManager() :sp(0), m_CreateFrame(0)
{
	m_pBoneImage = new tdn2DObj("DATA/CHR/hone_motion.png");

	g_CreateSheepFloor = 2;	// 初期羊生成フロア
	// 次のランダムフロア決定
	m_NextChangeFloor = MakeNextFloor(g_CreateSheepFloor);

	// テキストからパラメータ読み込み
	std::fstream ifs("DATA/Text/Param/sheep.txt");

	char skip[64];	// 読み飛ばし用変数

	// 出現座標読み込み
	ifs >> skip;
	ifs >> m_TextParam.startX;

	// 出現間隔(ミリ秒)読み込み
	ifs >> skip;
	ifs >> m_TextParam.AppTime;

	// 出現の上下幅読み込み
	ifs >> skip;
	ifs >> m_TextParam.VerticalWidth;

	// 速度読み込み
	ifs >> skip;
	ifs >> m_TextParam.speed;

	// 草くって太った羊の速度
	ifs >> skip;
	ifs >> m_TextParam.FatSheepAccel;

	// 羊情報読み込み
	for (int i = 0; i < (int)SHEEP_TYPE::MAX; i++)
	{
		ifs >> skip;
		
		// 羊画像読み込み
		ifs >> skip;
		char path[MAX_PATH];
		ifs >> path;
		m_TextParam.data[i].Image = new tdn2DObj(path);
		m_TextParam.data[i].BoneImage = m_pBoneImage;

		// でぶがぞう
		ifs >> skip;
		ifs >> path;
		m_pFatSheepImages[i] = new tdn2DObj(path);

		// 掴まれ画像
		ifs >> skip;
		ifs >> path;
		m_TextParam.data[i].CatchImage = new tdn2DObj(path);

		// 得点倍率読み込み
		ifs >> skip;
		ifs >> m_TextParam.data[i].magnification;

		// コマのサイズ
		ifs >> skip;
		ifs >> m_TextParam.data[i].SIZE;

		// アニメ枚数
		ifs >> skip;
		ifs >> m_TextParam.data[i].NumPanel;

		// コマ進めるフレーム
		ifs >> skip;
		ifs >> m_TextParam.data[i].Animkankaku;

		// 出現率
		ifs >> skip;
		ifs >> m_TextParam.data[i].percentage;
	}
	m_List.clear();
}

SheepManager::~SheepManager()
{
	for (int i = 0; i < (int)SHEEP_TYPE::MAX; i++)
	{
		SAFE_DELETE(m_TextParam.data[i].Image);
		delete m_pFatSheepImages[i];
		SAFE_DELETE(m_TextParam.data[i].CatchImage);
	}
	delete m_pBoneImage;

	for (auto it : m_List) delete it;
	for (auto it : m_FatList) delete it;
	m_List.clear();
	m_FatList.clear();
}

//**************************************************

void SheepManager::create(int floor)
{
	// ランダムに羊のタイプ
	float r = tdnRandom::Get(.0f, 100.0f);
	for (int i = (int)SHEEP_TYPE::MAX - 1; i >= 0; i--)
	{
		// ランダムに取得したパーセントが設定したpercentage範囲内だったらそいつを生成
		if (r <= m_TextParam.data[i].percentage)
		{
			switch ((SHEEP_TYPE)i)
			{
			case SHEEP_TYPE::NOMAL:
				m_List.push_back(new Sheep::Normal(m_TextParam.data[i], floor, m_TextParam));
				break;
			case SHEEP_TYPE::GOLD:
				m_List.push_back(new Sheep::Gold(m_TextParam.data[i], floor, m_TextParam));
				break;
			case SHEEP_TYPE::REAL:
				m_List.push_back(new Sheep::Real(m_TextParam.data[i], floor, m_TextParam));
				break;
			default:
				assert(0);	// 例外処理
				break;
			}

			break;
		}
		else r -= m_TextParam.data[i].percentage;
	}
}

void SheepManager::Update()
{
	// デバッグ用
	{
		//if (KeyBoardTRG(KB_DOT))
		//{
		//	//m_ChangeLaneTime = 0;
		//	g_CreateSheepFloor = 0;
		//}
		//else if (KeyBoardTRG(KB_SLASH))
		//{
		//	//m_ChangeLaneTime = 0;
		//	g_CreateSheepFloor = 1;
		//}
		//else if (KeyBoardTRG(KB_UNDER_BAR))
		//{
		//	//m_ChangeLaneTime = 0;
		//	g_CreateSheepFloor = 2;
		//}
	}

	// レーン変更時間
	//if (++m_ChangeLaneTime > CHANGE_LANE_TIME)
	//{
	//	// 時間リセット
	//	m_ChangeLaneTime = 0;
	//
	//	// レーンを変える
	//	g_CreateSheepFloor = m_NextChangeFloor;
	//
	//	// 次のランダムフロア決定
	//	m_NextChangeFloor = MakeNextFloor(g_CreateSheepFloor);
	//}
	//else if (m_ChangeLaneTime == CHANGE_LANE_TIME / 4)
	//{
	//	// ★UIマネージャにポップアップをお願いする
	//}

	// 前回の時間と今の時間の差分
	/*
	UINT delta = clock() - m_CurrentTime;
	bool bCreate(false);

	// 前回からの経過時間と出現間隔に応じて、羊を生成
	for (int j = delta; j > m_TextParam.AppTime; j -= m_TextParam.AppTime)
	{
		create(g_CreateSheepFloor);
		bCreate = true;
	}

	// 現在時刻設定
	if (bCreate)m_CurrentTime = clock();
	*/

	if (++m_CreateFrame > m_TextParam.AppTime)
	{
		m_CreateFrame = 0;
		create(g_CreateSheepFloor);
	}

	// 掴み更新
	CatchUpdate();

	// 羊更新
	for (auto it = m_List.begin(); it != m_List.end();)
	{
		(*it)->Update();
		if ((*it)->EraseOK() || (*it)->Get_pos()->x < -2048 || (*it)->Get_pos()->x > 2048)
		{
			delete (*it);
			it = m_List.erase(it);
		}
		else it++;
	}

	// 牧草食って太った羊更新
	for (auto it = m_FatList.begin(); it != m_FatList.end();)
	{
		(*it)->Update();
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_FatList.erase(it);
		}
		else it++;
	}
}

void SheepManager::CatchUpdate()
{
	auto CheckCatch = [](bool *OutbLeft, bool bNikuHold)
	{
		if (tdnMouse::GetRight() == 3)
		{
			*OutbLeft = false;
			return true;
		}
		// 5フレーム目
		else if (KeyBoard(MOUSE_LEFT) == 5 && !bNikuHold)
		{
			*OutbLeft = true;
			return true;
		}
		else return false;
	};
	// 掴む処理
	bool bLeftCatch;
	if (CheckCatch(&bLeftCatch, NikuMgr->isNikuHold()))
	{
		for (auto& it : m_List)
		{
			// 骨をつかませない！！
			if (it->isDead()) continue;

			if ((it->GetCenterPos() - tdnMouse::GetPos()).LengthSq() < 60 * 60)
			{
				// 羊捕まえるSE
				se->Play((it->GetType() == SHEEP_TYPE::REAL) ? "リアル掴む" : "羊掴む", *it->Get_pos());

				// 捕まれてるモードに変更
				it->Be_caught(bLeftCatch);
				break;
			}
		}
	}
}

//**************************************************

void SheepManager::Render()
{
	// 羊描画
	for (auto it : m_List)
	{
		it->Render();
	}
}

void SheepManager::RenderFat()
{
	// 草食って太った羊
	for (auto it : m_FatList) it->Render();
}

void SheepManager::CreateFatSheep(Sheep::Base *sheep)
{
	sheep->Erase();	// 元の羊を消去
	FatSheep *set = new FatSheep(m_pFatSheepImages[(int)sheep->GetType()], Vector2(sheep->Get_pos()->x, (float)STAGE_POS_Y[sheep->Get_floor()] - 30), sheep->GetType());// 太った羊生成
	set->SetFloor(sheep->Get_floor());	// フロア設定
	m_FatList.push_back(set);			// リストに突っ込む

	// 煙パーティクル
	Particle2dManager::Effect_FatSmoke(sheep->GetCenterPos());
}

int SheepManager::MakeNextFloor(int current)
{
	int r;	// ランダムな値を入れる変数
	do
	{
		// 同じレーンに変わらないようにループさせる
		r = rand() % STAGE_MAX;
	} while (r == current);
	return r;
}



SheepManager *g_pSheepMgr = nullptr;









// リザルト羊走ってる状態
void ResultSheep::Mode::Run::Update()
{
	// 基底クラスの更新(アニメ関連)
	Base::Update();

	// 右に移動
	m_pSheep->m_pos.x += m_pSheep->m_speed;
}

// リザルト羊掃けてる状態
ResultSheep::Mode::Hakeru::Hakeru(ResultSheep *pSheep) :Base(pSheep)
{
	// 移動ベクトルランダムに作成
	const float RandAngle = (PI * .5f)*((float)rand() / RAND_MAX) + (PI * .25f);	// 0～90

	// アングル⇒ベクトル
	m_vec.x = -sinf(RandAngle);
	m_vec.y = cosf(RandAngle);
}
void ResultSheep::Mode::Hakeru::Update()
{
	// 基底クラスの更新(アニメ関連)
	Base::Update();

	// 左上に移動
	m_pSheep->m_pos += m_vec * ((1) ? 32 : m_pSheep->m_speed * 4);

	// 回転
	m_angle += (m_vec.y < 0) ? .2f : -.2f;

	// 画面外判定(消去)
	if (m_pSheep->m_pos.x < -120 || m_pSheep->m_pos.x > tdnSystem::GetScreenSize().right + 120||
		m_pSheep->m_pos.y < -120 || m_pSheep->m_pos.y > tdnSystem::GetScreenSize().bottom + 120)
		m_pSheep->Erase();
}

// リザルト羊管理
ResultSheepManager::ResultSheepManager(float ScoreWakuY, float SonotaWakuY, float startX, float speed)
:m_pSheepImage(new tdn2DObj("DATA/CHR/sheep_animation.png")), m_StartScoreY(ScoreWakuY), m_StartOtherY(SonotaWakuY), m_StartX(startX), m_speed(speed)
{
	m_waku[0].pImage = new tdn2DObj("DATA/Result/boadScore.png");
	m_waku[1].pImage = new tdn2DObj("DATA/Result/boadsonota.png");
	m_waku[0].pos.y = ScoreWakuY;
	m_waku[1].pos.y = SonotaWakuY;
	FOR(NUMBER_TYPE::MAX)m_pNumbers[i] = new Number;
}

ResultSheepManager::~ResultSheepManager()
{
	for (auto it = m_ScoreSheepList.begin(); it != m_ScoreSheepList.end();)
	{
		delete (*it);
		it = m_ScoreSheepList.erase(it);
	}
	for (auto it = m_OtherSheepList.begin(); it != m_OtherSheepList.end();)
	{
		delete (*it);
		it = m_OtherSheepList.erase(it);
	}
	delete m_pSheepImage;
	delete m_waku[0].pImage;
	delete m_waku[1].pImage;
	FOR(NUMBER_TYPE::MAX)delete m_pNumbers[i];
}

void ResultSheepManager::Update()
{
	// スコア枠を羊が運んでいるときの状態
	if (!m_ScoreSheepList.empty() && !m_waku[0].isSet)
	{
		m_waku[0].pos.x = (*m_ScoreSheepList.begin())->GetPos().x + 100;

		// 端っこにあたったif文
		if (m_waku[0].pos.x > tdnSystem::GetScreenSize().right - WAKU_WIDTH - 8)
		{
			se->Play("リザルトダン");
			m_waku[0].isSet = true;

			// 羊掃けさせる
			HakeruScoreSheep();

			// ナンバーアクション
			m_pNumbers[NUMBER_TYPE::SCORE]->Action();
		}
	}

	// その他枠を羊が運んでいるときの状態
	if (!m_OtherSheepList.empty() && !m_waku[1].isSet)
	{
		m_waku[1].pos.x = (*m_OtherSheepList.begin())->GetPos().x + 100;

		// 端っこにあたったif文
		if (m_waku[1].pos.x > tdnSystem::GetScreenSize().right - WAKU_WIDTH - 8)
		{
			se->Play("リザルトダン");
			m_waku[1].isSet = true;

			// 羊掃けさせる
			HakeruOtherSheep();

			// ナンバーアクション
			m_pNumbers[NUMBER_TYPE::COMBO]->Action();
			m_pNumbers[NUMBER_TYPE::WOLFKILL]->Action();
			m_pNumbers[NUMBER_TYPE::SHEEPKILL]->Action();
		}
	}

	// ナンバーアップデート更新
	FOR(NUMBER_TYPE::MAX)m_pNumbers[i]->Update();

	// スコア枠羊更新
	for (auto it = m_ScoreSheepList.begin(); it != m_ScoreSheepList.end();)
	{
		(*it)->Update();
		// 消去チェック
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_ScoreSheepList.erase(it);
		}
		else it++;
	}

	// その他枠羊更新
	for (auto it = m_OtherSheepList.begin(); it != m_OtherSheepList.end();)
	{
		(*it)->Update();
		// 消去チェック
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_OtherSheepList.erase(it);
		}
		else it++;
	}
}

void ResultSheepManager::Render()
{
	// 羊
	for (auto& it : m_ScoreSheepList) it->Render(m_pSheepImage);
	for (auto& it : m_OtherSheepList) it->Render(m_pSheepImage);

	// 枠
	m_waku[0].pImage->Render((int)m_waku[0].pos.x, (int)m_waku[0].pos.y);
	m_waku[1].pImage->Render((int)m_waku[1].pos.x, (int)m_waku[1].pos.y);

	// ナンバー
	m_pNumbers[NUMBER_TYPE::SCORE]->Render((int)m_waku[0].pos.x + 630, (int)m_waku[0].pos.y + 35, UIMNG.GetScore());	// スコア


	static const int OTHER_HABA = 85;
	m_pNumbers[NUMBER_TYPE::COMBO]->Render((int)m_waku[1].pos.x + 630, (int)m_waku[1].pos.y + 35, UIMNG.GetCombo());								// コンボ
	m_pNumbers[NUMBER_TYPE::WOLFKILL]->Render((int)m_waku[1].pos.x + 630, (int)m_waku[1].pos.y + 35 + OTHER_HABA, UIMNG.GetWorfHappyCount());		// 狼討伐
	m_pNumbers[NUMBER_TYPE::SHEEPKILL]->Render((int)m_waku[1].pos.x + 630, (int)m_waku[1].pos.y + 35 + OTHER_HABA*2, UIMNG.GetSheepKillCount());	// 羊死に
}

// スコア羊スタート
void ResultSheepManager::StartScoreSheep()
{
	static const int NUM_SHEEP = 3;		// 羊の数
	static const int HUREHABA = 16;		// 横の振れ幅(縦一列に並ぶのもどうかと思うので)

	FOR(NUM_SHEEP)
	{
		// スコア押す羊格納
		m_ScoreSheepList.push_back(new ResultSheep(Vector2(m_StartX - (rand() % HUREHABA), m_StartScoreY + i * 30), m_speed));
	}
}

// その他項目羊スタート
void ResultSheepManager::StartOtherSheep()
{
	static const int NUM_SHEEP = 7;		// 羊の数
	static const int HUREHABA = 16;		// 横の振れ幅(縦一列に並ぶのもどうかと思うので)

	FOR(NUM_SHEEP)
	{
		// スコア押す羊格納
		m_OtherSheepList.push_back(new ResultSheep(Vector2(m_StartX - (rand() % HUREHABA), m_StartOtherY + i * 30), m_speed));
	}
}