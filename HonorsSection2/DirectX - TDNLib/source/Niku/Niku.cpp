#include "TDNLIB.h"
#include "Niku.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "Effect\EffectManager.h"
#include "../particle_2d/particle_2d.h"

// �ÓI�����o�ϐ�
NikuManager *NikuManager::pInstance = nullptr;	// 1�����Ȃ�����

int g_YakiNikuModeChangeTime[(int)YAKINIKU_MODE::MAX];			// ���[�h���ς���Ă�������

//**************************************************
//    �q���Ǘ��N���X
//**************************************************
NikuManager::NikuManager() :m_pNiku(nullptr), m_pYakiniku(nullptr), m_pNikuImage(new tdn2DObj("DATA/CHR/!-kai2.png")), m_pIkenieImage(new tdn2DObj("DATA/CHR/kuruma2.png"))
{
	// �e�L�X�g����f�[�^�ǂݍ���
	std::ifstream ifs("DATA/Text/Param/niku.txt");
	assert(ifs);

	char skip[64];	// �ǂݔ�΂��p
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
		// �܂����ݒu���ĂȂ�������
		if (!m_pNiku->isSeted())
		{
			// ���͂�ł���
			if (m_bHoldNiku)
			{
				Vector2 mPos = tdnMouse::GetPos() + Vector2(-64, -64);
				// ���W��ݒ�
				m_pNiku->SetPos(mPos);

				// �}�E�X��������
				if (tdnMouse::GetLeft() == 2)
				{
					// �͂݃t���OOFF
					m_bHoldNiku = false;

					// �ŒZ�t���A�v�Z
					int floor = FindFloor(mPos.y);
					m_pNiku->SetFloor(floor);

					// Y���W�␳
					m_pNiku->SetPosY((float)STAGE_POS_Y[floor] + (float)(LANE_WIDTH / 8));

					// ���ݒu�I�I
					m_pNiku->Set();
				}
			}

			// ���͂�łȂ����
			else
			{
				// �}�E�X��������
				if (tdnMouse::GetLeft() == 3)
				{
					// ���u��Ƃ̋����͈͓�
					if ((tdnMouse::GetPos() - m_pNiku->GetCenterPos()).LengthSq() < 64 * 64)
					{
						// ���͂݃t���OON
						m_bHoldNiku = true;
					}
				}
			}
		}

		m_pNiku->Update();

		// �����`�F�b�N
		if (m_pNiku->EraseOK())
		{
			delete m_pNiku, m_pNiku = nullptr;
		}
	}

	else if (m_pYakiniku)
	{
		m_pYakiniku->Update();

		// �}�E�X���N���b�N�œ��쐬
		if (tdnMouse::GetLeft() == 3)
		{
			// �ē��G���A���Ȃ�
			if ((tdnMouse::GetPos() - (YAKINIKU_AREA + Vector2(128, 128))).LengthSq() < 128 * 128)
			{
				// �����
				CreateNiku();
			}
		}

		// �����`�F�b�Nt
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

	// SE�̍Đ�
	se->Play("�Ă���", YAKINIKU_AREA);
	se->Play("�ߖ�", YAKINIKU_AREA);
}

void NikuManager::CreateNiku()
{
	if (m_pYakiniku)
	{
		// �ē������ē����
		m_pNiku = new Niku(YAKINIKU_AREA + Vector2(128, 90), m_pYakiniku->GetMode(), m_pNikuImage);
		m_pYakiniku->Erase();
	}
}


//**************************************************
//    �Ă����N���X
//**************************************************
//**************************************************
//    �q���Ϗ��N���X
//**************************************************
void YakinikuMode::Base::Update()
{
	// �ݒ莞�ԉ߂�����
	if (++m_timer > m_limit)
	{
		NextMode();	// ���̃��[�h��
	}
}

void YakinikuMode::Base::Render()
{
	m_pYakiniku->pImage->Render(m_pYakiniku->GetPosX(), m_pYakiniku->GetPosY(), 256, 256, m_srcX, 0, 256, 256);
}

void YakinikuMode::Base::NextMode()
{
	// ���̃��[�h�ɕύX
	m_pYakiniku->ChangeMode(m_NextMode);
}

//===========================================================
//		��
//===========================================================
YakinikuMode::Nama::Nama(Yakiniku *me) :Base(me)
{
	// �{�t�ɍs��
	m_NextMode = YAKINIKU_MODE::RARE;

	// �摜���W
	m_srcX = 0;

	// SE�̍Đ�
	//se->Play("�q���萶����", me->GetPos());
}
//void YakinikuMode::Nama::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		���A
//===========================================================
YakinikuMode::Rare::Rare(Yakiniku *me) :Base(me)
{
	// �ڂ݂ɍs��
	m_NextMode = YAKINIKU_MODE::MEDIAM;

	// �摜���W
	m_srcX = 256;

	// SE�̍Đ�
	//se->Play("�q������", me->GetPos());
}
//void YakinikuMode::Rare::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		�~�f�B�A��
//===========================================================
YakinikuMode::Mediam::Mediam(Yakiniku *me) :Base(me)
{
	// �炢���ɍs��
	m_NextMode = YAKINIKU_MODE::KOGETA;

	// �摜���W
	m_srcX = 256 * 2;

	// SE�̍Đ�
	//se->Play("�q������", me->GetPos());
}
//void YakinikuMode::Mediam::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}


//===========================================================
//		�E�F���_��
//===========================================================
YakinikuMode::Weldan::Weldan(Yakiniku *me) :Base(me)
{
	// �����Ėq�ꂪ�a������
	m_NextMode = YAKINIKU_MODE::KOGETA;

	// �摜���W
	m_srcX = 256 * 3;

	// SE�̍Đ�
	//se->Play("�q������", me->GetPos());

}
//void YakinikuMode::Weldan::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		�ł���
//===========================================================
YakinikuMode::Kogeta::Kogeta(Yakiniku *me) :Base(me), m_KiraKiraCoolTime(0)
{
	// �摜���W
	m_srcX = 256 * 3;

	// SE�̍Đ�
	se->Play("�q������", me->GetPos());

	// �G�t�F�N�g����
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PLUS);
}
void YakinikuMode::Kogeta::Update()
{
	// ���炫��p�[�e�B�N��
	//if (++m_KiraKiraCoolTime > 4)
	//{
	//	m_KiraKiraCoolTime = 0;
	//	Particle2dManager::Effect_KiraKira(m_pYakiniku->GetCenterPos(), Vector2(56, 56), 15.0f, 10.0f);
	//}
}

//**************************************************
//    �q���N���X
//**************************************************

//===========================================================
//		�������E���
//===========================================================
Yakiniku::Yakiniku(tdn2DObj *image) :m_pMode(nullptr), m_bErase(false), m_pos(YAKINIKU_AREA), pImage(image)
{
	// ���[�h������(�o�t�ǂ���J�n)
	this->ChangeMode(YAKINIKU_MODE::NAMA);
}

Yakiniku::~Yakiniku()
{
	delete m_pMode;
}


//===========================================================
//		�X�V
//===========================================================
void Yakiniku::Update()
{
	m_pMode->Update();
}



//===========================================================
//		�`��
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

	// ���[�h���ς�鎞�Ԑݒ�
	m_pMode->SetChangeModeTime(g_YakiNikuModeChangeTime[(int)m]);
}


//**************************************************
//    ���N���X
//**************************************************

//===========================================================
//		�������E���
//===========================================================
Niku::Niku(const Vector2 &pos, YAKINIKU_MODE type, tdn2DObj *image) :m_pos(pos), m_bErase(false), m_orgY(pos.y),
m_BoundPow(Vector2(6, -8)), m_gravity(1.0f), m_pImage(image), m_type(type), m_floor(FindFloor(pos.y)), m_bSet(false)
{
	m_move = m_BoundPow;
	// Y���W�␳
	//m_pos.y = (float)STAGE_POS_Y[m_floor] + (float)(LANE_WIDTH / 8);
}


//===========================================================
//		�X�V
//===========================================================
void Niku::Update()
{
	if (m_BoundPow.y != 0)
	{
		// �d�͏���
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
//		�`��
//===========================================================
void Niku::Render()
{
	m_pImage->Render((int)m_pos.x, (int)m_pos.y);
}