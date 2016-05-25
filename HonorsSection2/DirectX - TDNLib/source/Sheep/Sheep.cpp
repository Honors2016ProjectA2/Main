#include "IEX_Expansion.h"
#include "../Sound/SoundManager.h"
#include "MousePointer.h"
#include "../CurvePoint/CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "Sheep.h"
#include "../Data/DataMNG.h"
#include "../system/system.h"

Sheep::Base::Base(const SheepData &data, int floor, const SheepTextParam &textparam) :
animepos(0, 0),
floor(floor),
process(GETOUT),
m_AnimePanel(0), m_bErase(false)
{
	Move[0] = &Sheep::Base::Get_out;
	Move[1] = &Sheep::Base::Walk;
	Move[2] = &Sheep::Base::Curve;
	Move[3] = &Sheep::Base::Caught;
	Move[4] = &Sheep::Base::Crushed;
	Move[5] = &Sheep::Base::Ran_over;

	alpha = 0xffffffff;
	frame = 0, animeframe = 0;
	//move = 5;/* speed */

	this->ase.animex = 0;

	memcpy_s(&m_data, sizeof(SheepData), &data, sizeof(SheepData));
	m_data.Image = data.Image;

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
	animeframe++;
	if (animeframe > m_data.Animkankaku)
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
	animeframe++;
	if (animeframe > m_data.Animkankaku)
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
			floor = i;
			break;
		}
	}
}

void Sheep::Base::Caught()
{
	if (animeframe < 7)
	{
		if (frame > 1)
			animeframe++, frame = 0;
		frame++;
	}
	else
	{
		alpha -= 0x0f000000;
		if (alpha <= 0x00ffffff)
			m_bErase = true;
	}
}
void Sheep::Base::Crushed()
{
	if (animeframe < 11)
	{
		if (frame > 2)
			animeframe++, frame = 0;
		frame++;
	}
	else
	{
		m_bErase = true;;
	}
}
void Sheep::Base::Ran_over()
{
	if (animeframe < 9)
	{
		if (frame > 1)
			animeframe++, frame = 0;

		if (animeframe > 3)
		{
			alpha -= 0x0f000000;
		}
		frame++;
	}
	else
		m_bErase = true;
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
	Get_frame_pos(animepos, m_AnimePanel);

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

	m_data.Image->SetARGB(alpha);
	m_data.Image->Render((int)pos.x, (int)pos.y, m_data.SIZE, m_data.SIZE,
		(int)animepos.x, (int)animepos.y, (move.x >= 0) ? -m_data.SIZE : m_data.SIZE, m_data.SIZE,
		RS::COPY);
}

void Sheep::Base::Be_caught(int type)
{
	if (col_check) return;
	switch (type)
	{
	case 0:
		process = CAUGHT;
		//obj = file[CAUGHT];
		break;

	case 1:
		Be_crushed();
		break;

	case 2:
		Be_ran_over();
		break;

	default:
		break;
	}
	animeframe = frame = 0;
}
void Sheep::Base::Be_crushed()
{
	process = CRUSHED; 
	//obj = file[CRUSHED];
}
void Sheep::Base::Be_ran_over()
{ 
	process = RAN_OVER; 
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
	se->Stop("きらめく羊さん", m_seID);
}
void Sheep::Gold::Update()
{
	// 基底クラスのアップデート
	Sheep::Base::Update();

	// 音の座標設定
	se->SetPos("きらめく羊さん", m_seID, pos);
}


//**************************************************
//	牧草食って太った羊
//**************************************************
FatSheep::FatSheep(tdn2DObj *image, const Vector2 &pos) :m_image(image), m_pos(pos), m_angle(0), m_bErase(false), m_moveX(0), m_ReceiveSE(TDNSOUND_PLAY_NONE)
{

}

FatSheep::~FatSheep()
{
	// ループしてるSEを止める
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("太った羊押す", m_ReceiveSE);
}

void FatSheep::Update()
{
	// SE鳴らしてないときの状態
	if (m_ReceiveSE == TDNSOUND_PLAY_NONE)
	{
		// 動いてたら
		if (m_moveX != 0) m_ReceiveSE = se->Play("太った羊押す", m_pos, Vector2(0, 0), true);	// SE再生(デストラクタで止める)
	}
	else
	{
		// SEの座標を設定
		se->SetPos("太った羊押す", m_ReceiveSE, m_pos);
	}
	// 移動値を足していく
	m_pos.x += m_moveX;
	// 移動値に応じて羊を回転させる
	m_angle -= m_moveX * 0.01f;
}

void FatSheep::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, 0, 0, 240, 240);
}


//**************************************************

// manager

//**************************************************

SheepManager::SheepManager() :
m_ChangeLaneTime(0), sp(0)
{
	m_pFatSheepImage = new tdn2DObj("DATA/CHR/fat_sheep.png");

	m_CreateFloor = 0;	// 初期羊生成フロア
	// 次のランダムフロア決定
	m_NextChangeFloor = MakeNextFloor(m_CreateFloor);

	// テキストからパラメータ読み込み
	std::fstream ifs("DATA/Text/Param/sheep.txt");

	char skip[64];	// 読み飛ばし用変数

	// 羊レーンが変わる時間読み込み
	ifs >> skip;
	ifs >> CHANGE_LANE_TIME;

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
	// タイマー設定
	m_CurrentTime = clock();
}

SheepManager::~SheepManager()
{
	for (int i = 0; i < (int)SHEEP_TYPE::MAX; i++)
	{
		SAFE_DELETE(m_TextParam.data[i].Image);
	}
	delete m_pFatSheepImage;

	for (auto it : m_List) delete it;
	m_List.clear();
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
		if (KeyBoardTRG(KB_DOT))
		{
			m_ChangeLaneTime = 0;
			m_CreateFloor = 0;
		}
		else if (KeyBoardTRG(KB_SLASH))
		{
			m_ChangeLaneTime = 0;
			m_CreateFloor = 1;
		}
		else if (KeyBoardTRG(KB_UNDER_BAR))
		{
			m_ChangeLaneTime = 0;
			m_CreateFloor = 2;
		}
	}

	// レーン変更時間
	if (++m_ChangeLaneTime > CHANGE_LANE_TIME)
	{
		// 時間リセット
		m_ChangeLaneTime = 0;

		// レーンを変える
		m_CreateFloor = m_NextChangeFloor;

		// 次のランダムフロア決定
		m_NextChangeFloor = MakeNextFloor(m_CreateFloor);
	}
	else if (m_ChangeLaneTime == CHANGE_LANE_TIME / 4)
	{
		// ★UIマネージャにポップアップをお願いする
	}

	// 前回の時間と今の時間の差分
	UINT delta = clock() - m_CurrentTime;
	bool bCreate(false);

	// 前回からの経過時間と出現間隔に応じて、羊を生成
	for (int j = delta; j > m_TextParam.AppTime; j -= m_TextParam.AppTime)
	{
		create(m_CreateFloor);
		bCreate = true;
	}

	// 現在時刻設定
	if (bCreate)m_CurrentTime = clock();

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

//**************************************************

void SheepManager::Render()
{
	// 羊描画
	for (auto it : m_List)
	{
		it->Render();
	}

	// 草食って太った羊
	for (auto it : m_FatList) it->Render();
}

void SheepManager::CreateFatSheep(Sheep::Base *sheep)
{
	sheep->Erase();	// 元の羊を消去
	FatSheep *set = new FatSheep(m_pFatSheepImage, *sheep->Get_pos() + Vector2(-64, -64));// 太った羊生成
	set->SetFloor(sheep->Get_floor());	// フロア設定
	m_FatList.push_back(set);			// リストに突っ込む
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