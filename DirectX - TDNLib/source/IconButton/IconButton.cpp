#include "TDNLIB.h"
#include "IconButton.h"
#include "../system/System.h"
#include "../Sound/SoundManager.h"

//*****************************************************************************************************************************
//
//		�{�^���Ǘ�����
//
//*****************************************************************************************************************************
//******************************************************************
//		�f�[�^�ǉ�
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
		int ID, dstX, dstY;					// �摜�̈ʒu���
		char TexPath[MAX_PATH];				// �摜�̃t�@�C���p�X
		char se_ID[64];						// SE��炷ID
		BYTE InAction;						// �������Ƃ��̋����̃t���O

		char skip[128];	// �ǂݔ�΂��p
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

			// �l�p����
			if (strcmp(work, "�l�p") == 0)
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

				// �摜�̒��S�擾�p��2D�̎��̂����
				tdn2DObj *lpImage = new tdn2DObj(TexPath);
				Vector2 CenterPos(lpImage->GetWidth() *.5f + dstX, lpImage->GetHeight() *.5f + dstY);
				delete lpImage;

				// �]�T���������炱�̕ӂ̏����͉��P������
				Vector2 v = MinPos - CenterPos;	// ���S������
				float len = v.Length()*scale;
				v.Normalize();
				v *= len;
				MinPos = CenterPos + v;

				v = MaxPos - CenterPos;
				v.Normalize();
				v *= len;
				MaxPos = CenterPos + v;

				// ����Ϗ��̎��̂����̒��ō쐬���Ă���
				set->SetCollisionSqure(MinPos, MaxPos);
			}

			// �~����
			else if (strcmp(work, "�~") == 0)
			{
				float radius;
				ifs >> skip;
				ifs >> radius;

				// �摜�̒��S�擾�p��2D�̎��̂����
				tdn2DObj *lpImage = new tdn2DObj(TexPath);
				Vector2 CenterPos(lpImage->GetWidth() *.5f + dstX, lpImage->GetHeight() *.5f + dstY);
				delete lpImage;

				// ����Ϗ��̎��̂����̒��ō쐬���Ă���
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
//		�������E���
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
//		�X�V�E�`��
//******************************************************************
void IconButtonManager::Update(const Vector2 &CursorPos)
{
	// ���X�g�̒��g�X�V
	for (auto it : m_List)
	{
		it->Update(CursorPos);	// �X�V
	}
}

void IconButtonManager::Render()
{
	// ���X�g�̒��g�`��
	for (auto it : m_List)
	{
		it->Render();		// �`��
	}
}


int IconButtonManager::GetInButtonNo()
{
	int ButtonNo(NOT_IN_BUTTON);	// �����l�͉��������Ă��Ȃ����Ƃ��Ӗ�����-1���w��

	for (UINT i = 0; i < m_List.size(); i++)
	{
		// �J�[�\���͈͓̔��ɓ����Ă�摜
		if (m_List[i]->isIn())
		{
			// ���̉摜��ID��Ԃ�
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
//		�{�^���P��
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

	// SE��ID�������Ă���ݒ�A�����ĂȂ�������""��ݒ�
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
	// �ғ���Ԃ��ǂ���
	//if (m_EnDisType != EN_DIS_TYPE::ENABLE)return;
	if (m_EnDisType == EN_DIS_TYPE::DISABLE_VANISH) return;

	// �A�C�R���͈͓���
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
					char *b = "�J�[�\��";
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
	// �{�^���L�����
	if (m_EnDisType == EN_DIS_TYPE::ENABLE)
	{
		m_pButton->SetARGB(0xffffffff);
		m_pButton->SetScale((m_InActionFlag & IN_ACTION::UP_SCALE && m_In) ?
			1.1f :	// ������Ɗg�債�ăt���OON
			1.0f	// OFF
			);
		if (m_InActionFlag & IN_ACTION::PUT_WHITE && m_In) m_pButton->Render(m_dstX, m_dstY, shader2D, "color_over_ray");
		else m_pButton->Render(m_dstX, m_dstY);
	}

	// �{�^���������(��⍕������)
	else if (m_EnDisType == EN_DIS_TYPE::DISABLE_BLACK)
	{
		m_pButton->SetARGB((BYTE)252, (BYTE)128, (BYTE)128, (BYTE)128);
		m_pButton->SetScale(1);
		m_pButton->Render(m_dstX, m_dstY);
	}

	// �{�^���������(��┒������)
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
	// �����̔���͈̔͂�`�悷��
	tdnPolygon::Rect((int)m_MinPos.x, (int)m_MinPos.y, (int)(m_MaxPos.x - m_MinPos.x), (int)(m_MaxPos.y - m_MinPos.y), RS::COPY, 0x80ff1010);
}

void IconButton::Collision2D::Circle::RenderDebug()
{

}

bool IconButton::Collision2D::Squre::Collision(const Vector2 &pos)
{
	// 2D�l�p����
	return (pos.x >= m_MinPos.x && pos.x <= m_MaxPos.x && pos.y >= m_MinPos.y && pos.y <= m_MaxPos.y);
}
bool IconButton::Collision2D::Circle::Collision(const Vector2 &pos)
{
	// 2D�~����
	return ((pos - m_CenterPos).LengthSp() < m_Radius * m_Radius);
}