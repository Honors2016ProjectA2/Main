#include "TDNLIB.h"
#include "IconButton.h"
#include "../system/System.h"
#include "../Sound/SoundManager.h"

//*****************************************************************************************************************************
//
//		ボタン管理さん
//
//*****************************************************************************************************************************
//******************************************************************
//		データ追加
//******************************************************************
//int IconButtonManager::Push(char *TexPath, int dstX, int dstY,  BYTE InAction, char* se_ID)
//{
//	IconButton *set = new IconButton;
//	set->Initialize(m_Number, TexPath, dstX, dstY, InAction, se_ID);
//
//	m_List.push_back(set);
//
//	return (m_Number) += 1;
//}

void IconButtonManager::TextLoad(char *filename)
{
	std::ifstream ifs(filename);
	assert(ifs);

	while (!ifs.eof())
	{
		IconButton *set = new IconButton;
		int ID, dstX, dstY;					// 画像の位置情報
		char TexPath[MAX_PATH];				// 画像のファイルパス
		char se_ID[64];						// SEを鳴らすID
		BYTE InAction;						// 入ったときの挙動のフラグ

		char skip[128];	// 読み飛ばし用
		ifs >> skip;
		ifs >> ID;
		ifs >> skip;
		ifs >> TexPath;
		ifs >> skip;
		ifs >> dstX;
		ifs >> dstY;
		ifs >> skip;

		{
			char work[16];
			ifs >> work;

			// 四角判定
			if (strcmp(work, "四角") == 0)
			{
				Vector2 dstWH, MinPos, MaxPos;
				float scale;
				ifs >> skip;
				ifs >> MinPos.x;
				ifs >> MinPos.y;
				ifs >> skip;
				ifs >> dstWH.x;
				ifs >> dstWH.y;
				MaxPos = MinPos + dstWH;

				ifs >> skip;
				ifs >> scale;

				// 画像の中心取得用に2Dの実体を作る
				tdn2DObj *lpImage = new tdn2DObj(TexPath);
				Vector2 CenterPos(lpImage->GetWidth() *.5f + dstX, lpImage->GetHeight() *.5f + dstY);
				delete lpImage;

				// 余裕があったらこの辺の処理は改善したい
				Vector2 v = MinPos - CenterPos;	// 中心→左上
				float len = v.Length()*scale;
				v.Normalize();
				v *= len;
				MinPos = CenterPos + v;

				v = MaxPos - CenterPos;
				v.Normalize();
				v *= len;
				MaxPos = CenterPos + v;

				// 判定委譲の実体をこの中で作成している
				set->SetCollisionSqure(MinPos, MaxPos);
			}

			// 円判定
			else if (strcmp(work, "円") == 0)
			{
				float radius;
				ifs >> skip;
				ifs >> radius;

				// 画像の中心取得用に2Dの実体を作る
				tdn2DObj *lpImage = new tdn2DObj(TexPath);
				Vector2 CenterPos(lpImage->GetWidth() *.5f + dstX, lpImage->GetHeight() *.5f + dstY);
				delete lpImage;

				// 判定委譲の実体をこの中で作成している
				set->SetCollisionCircle(CenterPos, radius);
			}
			else assert(0);
		}
		ifs >> skip;
		ifs >> InAction;
		ifs >> skip;
		ifs >> se_ID;

		char *pSE_ID = nullptr;
		if (strcmp(se_ID, "NO") != 0) pSE_ID = se_ID;

		set->Initialize(ID, TexPath, dstX, dstY, InAction, pSE_ID);
		m_List.push_back(set);
	}
}

//******************************************************************
//		初期化・解放
//******************************************************************
IconButtonManager::IconButtonManager()
{
	m_List.clear();
}

IconButtonManager::~IconButtonManager()
{
	Clear();
}

void IconButtonManager::Clear()
{
	for (auto it : m_List) delete it;
	m_List.clear();
}

//******************************************************************
//		更新・描画
//******************************************************************
void IconButtonManager::Update(const Vector2 &CursorPos)
{
	// リストの中身更新
	for (auto it : m_List)
	{
		it->Update(CursorPos);	// 更新
	}
}

void IconButtonManager::Render()
{
	// リストの中身描画
	for (auto it : m_List)
	{
		it->Render();		// 描画
	}
}


int IconButtonManager::GetInButtonNo()
{
	int ButtonNo(NOT_IN_BUTTON);	// 初期値は何も入っていないことを意味する-1を指定

	for (UINT i = 0; i < m_List.size(); i++)
	{
		// カーソルの範囲内に入ってる画像
		if (m_List[i]->isIn())
		{
			// その画像のIDを返す
			ButtonNo = m_List[i]->GetID();
			break;
		}
	}
	return ButtonNo;
}

void IconButtonManager::SetEnDis(UINT ID, EN_DIS_TYPE type)
{
	assert(ID < m_List.size());
	for (auto it : m_List)
	{
		if (it->GetID() == ID)
		{
			it->SetEnable(type);
			break;
		}
	}
}


//*****************************************************************************************************************************
//
//		ボタン単体
//
//*****************************************************************************************************************************

void IconButton::Initialize(int ID, char *TexPath, int dstX, int dstY, BYTE InAction, char* se_ID)
{
	m_ID = ID;
	m_dstX = dstX;
	m_dstY = dstY;
	//m_dstW = dstW;
	//m_dstH = dstH;
	//m_srcW = srcW;
	//m_srcH = srcH;
	m_pButton = new tdn2DObj(TexPath);
	m_InActionFlag = InAction;

	// SEのIDが入ってたら設定、入ってなかったら""を設定
	if (se_ID)
		sprintf_s(m_SE_ID, _countof(m_SE_ID), "%s", se_ID);
	else memset(m_SE_ID, '\0', _countof(m_SE_ID));
}

IconButton::~IconButton()
{
	delete m_pButton;
	SAFE_DELETE(m_pCollision);
}

void IconButton::Update(const Vector2 &CursorPos)
{
	// 稼働状態かどうか
	//if (m_EnDisType != EN_DIS_TYPE::ENABLE)return;
	if (m_EnDisType == EN_DIS_TYPE::DISABLE_VANISH) return;

	// アイコン範囲内時
	if (
		//CursorPos.x >= m_dstX && CursorPos.x <= m_dstX + m_dstW && CursorPos.y >= m_dstY && CursorPos.y <= m_dstY + m_dstH
		m_pCollision->Collision(CursorPos)
		)
	{
		if (!m_In)
		{
			if (strcmp(m_SE_ID, "\0") != 0)
			{
				if (m_SEReceive == TDNSOUND_PLAY_NONE)
				{
					char *a = m_SE_ID;
					char *b = "カーソル";
					m_SEReceive = se->Play(b);
				}
			}
		}
		m_In = true;
	}
	else
	{
		m_SEReceive = TDNSOUND_PLAY_NONE;
		m_In = false;
	}
}

void IconButton::Render()
{
	// ボタン有効状態
	if (m_EnDisType == EN_DIS_TYPE::ENABLE)
	{
		m_pButton->SetARGB(0xffffffff);
		m_pButton->SetScale((m_InActionFlag & IN_ACTION::UP_SCALE && m_In) ?
			1.1f :	// ちょっと拡大してフラグON
			1.0f	// OFF
			);
		if (m_InActionFlag & IN_ACTION::PUT_WHITE && m_In) m_pButton->Render(m_dstX, m_dstY, shader2D, "color_over_ray");
		else m_pButton->Render(m_dstX, m_dstY);
	}

	// ボタン無効状態(やや黒くする)
	else if (m_EnDisType == EN_DIS_TYPE::DISABLE_BLACK)
	{
		m_pButton->SetARGB((BYTE)252, (BYTE)128, (BYTE)128, (BYTE)128);
		m_pButton->SetScale(1);
		m_pButton->Render(m_dstX, m_dstY);
	}

	// ボタン無効状態(やや白くする)
	else if (m_EnDisType == EN_DIS_TYPE::DISABLE_WHITE)
	{
		m_pButton->SetARGB(0xffffffff);
		m_pButton->SetScale(1);
		m_pButton->Render(m_dstX, m_dstY, shader2D, "color_over_ray");
	}

#ifdef _DEBUG
	//m_pCollision->RenderDebug();
#endif
}


void IconButton::Collision2D::Squre::RenderDebug()
{
	// 自分の判定の範囲を描画する
	tdnPolygon::Rect((int)m_MinPos.x, (int)m_MinPos.y, (int)(m_MaxPos.x - m_MinPos.x), (int)(m_MaxPos.y - m_MinPos.y), RS::COPY, 0x80ff1010);
}

void IconButton::Collision2D::Circle::RenderDebug()
{

}

bool IconButton::Collision2D::Squre::Collision(const Vector2 &pos)
{
	// 2D四角判定
	return (pos.x >= m_MinPos.x && pos.x <= m_MaxPos.x && pos.y >= m_MinPos.y && pos.y <= m_MaxPos.y);
}
bool IconButton::Collision2D::Circle::Collision(const Vector2 &pos)
{
	// 2D円判定
	return ((pos - m_CenterPos).LengthSp() < m_Radius * m_Radius);
}