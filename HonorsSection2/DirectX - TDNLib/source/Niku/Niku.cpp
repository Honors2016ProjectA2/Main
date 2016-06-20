#include "TDNLIB.h"
#include "../Sheep/Sheep.h"
#include "Niku.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "Effect\EffectManager.h"
#include "../particle_2d/particle_2d.h"
#include "../TipsCount/TipsCounter.h"

// �ÓI�����o�ϐ�
NikuManager *NikuManager::pInstance = nullptr;	// 1�����Ȃ�����

int g_YakiNikuModeChangeTime[(int)YAKINIKU_MODE::MAX];			// ���[�h���ς���Ă�������

//**************************************************
//    �q���Ǘ��N���X
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

	// �e�L�X�g����f�[�^�ǂݍ���
	std::ifstream ifs("DATA/Text/Param/niku.txt");
	assert(ifs);

	char skip[64];	// �ǂݔ�΂��p

	// �摜�p�X�ǂݍ���
	FOR((int)SHEEP_TYPE::MAX)
	{
		char path[MAX_PATH];

		// �����ɂ��摜
		ifs >> skip;
		ifs >> path;
		m_pIkenieImages[i] = new tdn2DObj(path);

		// ���摜
		ifs >> path;
		m_pNikuImages[i] = new tdn2DObj(path);
	}
	
	// ���Ă�����
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
	// ���x�W�G
	if (m_pNikuBazier)
	{
		m_pNikuBazier->Update();
		static float fireAngle = 0.0f;
		fireAngle -= 0.5f;
		m_pNikuBazier->GetPic()->SetAngle(fireAngle);
		// �x�W�G�I�������
		if(m_pNikuBazier->IsEnd())
		{
			// �x�W�G������
			delete m_pNikuBazier; m_pNikuBazier = nullptr;
			// ������(��������΂����悤�ɂȂ��Ă���)
			delete m_pNiku, m_pNiku = nullptr;

			// �΃G�t�F�N�g
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 128, (int)YAKINIKU_AREA.y + 138, EFFECT_TYPE::BURN);

			// SE
			se->Play("�_��", YAKINIKU_AREA);
		}
	}
	// ��
	if (m_pNiku)
	{
		// �����Ă���X���[
		if (m_pNiku->isVisible()) return;

		// ���͂�ł���
		if (m_bHoldNiku)
		{
			Vector2 mPos = tdnMouse::GetPos() + Vector2(-64, -64);

			// �����[�ɍs�������Ȃ��悤�ɕ␳
			mPos.x = Math::Clamp(mPos.x, 120, 1000);

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
					// ���̈ړ��ʏ���
					m_pNiku->MoveOff();

					// ���͂݃t���OON
					m_bHoldNiku = true;
					se->Play("�q������", m_pNiku->GetCenterPos());

					// �ݒu���Ȃ�
					m_pNiku->UnSet();
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
		static int HinokoFrame = 0;
		// �΂̕��p�[�e�B�N��
		if (++HinokoFrame > 1)
		{
			HinokoFrame = 0;
			Particle2dManager::Effect_Hinoko(YAKINIKU_AREA + Vector2(128, 128));
		}

		m_pYakiniku->Update();

		// �}�E�X���N���b�N�œ��쐬
		if (tdnMouse::GetLeft() == 3)
		{
			Vector2 mPos = tdnMouse::GetPos();
			// �ē��G���A���Ȃ�
			if (mPos.x > YAKINIKU_AREA.x + 30 && mPos.x < YAKINIKU_AREA.x + 30 + 190 && mPos.y > YAKINIKU_AREA.y + 96 && mPos.y < YAKINIKU_AREA.y + 96 + 100)
			{
				// �����
				CreateNiku();
			}
		}

		//else if (m_pYakiniku->GetMode() == YAKINIKU_MODE::KOGETA)
		//{
		//	// �ł�����������
		//	CreateNiku();
		//}

		// �����`�F�b�Nt
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

	// SE�̍Đ�
	se->Play("�Ă���", YAKINIKU_AREA);
	se->Play("�ߖ�", YAKINIKU_AREA);
}

void NikuManager::StartNikuBazier()
{
	// ������
	MyAssert(m_pNiku, "����͂��̓����Ȃ�");

	// ������(���̂͏������A�\������)
	m_pNiku->SetVisible();

	// �J�n�ƏI�����W
	Vector3 start(m_pNiku->GetCenterPos().x, m_pNiku->GetCenterPos().y, 0), end(YAKINIKU_AREA.x + 128, YAKINIKU_AREA.y + 128, 0);
	
	// �J�n���W�̈ʒu�ɉ����āA���W���Z�o
	Vector3 center, center2;

	float PowerLR = 420, PowerUD = 150;

	// �^�񒆃��[������Ȃ�������
	if (m_pNiku->GetFloor() != 1)
	{								/*	��		�E	*/
		PowerLR = (start.x < 640) ? PowerLR : -PowerLR;
									/*	��		��	*/
		PowerUD = (start.y < 320) ? PowerUD : -PowerUD;

		center = Vector3(start.x + PowerLR, start.y - PowerUD, 0);
		center2 = Vector3(center.x + PowerLR * 1.85f, center.y + PowerUD, 0);
	}
	// �^�񒆃��[��
	else
	{
		PowerLR = (start.x < 640) ? 260.0f : -260.0f;
		PowerUD = start.y;

		center = Vector3(start.x + PowerLR, start.y - PowerUD, 0);
		center2 = Vector3(center.x + PowerLR * 2.6f, PowerUD - 100, 0);
	}


	// ���x�W�G����
	m_pNikuBazier = new NikuBazier("DATA/powerF.png", start, center, center2, end, 46);
	m_pNikuBazier->Action();
}

// ���������𐶐�����u��
void NikuManager::CreateNiku()
{
	if (m_pYakiniku)
	{
		// �ē������ē����
		m_pNiku = new Niku(YAKINIKU_AREA + Vector2(128, 90), m_pYakiniku->GetMode(), m_pNikuImages[(int)m_pYakiniku->GetSheepType()], m_pYakiniku->GetSheepType());
		

		// �^�C�v�ɉ����ăG�t�F�N�g�����I
		switch (m_pYakiniku->GetMode())
		{
		case YAKINIKU_MODE::NAMA:
		case YAKINIKU_MODE::KOGETA:
			// BAD�G�t�F�N�g
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 128, (int)YAKINIKU_AREA.y + 64, EFFECT_TYPE::BAD);
			// SE�̍Đ�
			se->Play("BAD", YAKINIKU_AREA);
			break;
		case YAKINIKU_MODE::RARE:
			// GREAT�G�t�F�N�g
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 64, (int)YAKINIKU_AREA.y + 24, EFFECT_TYPE::GREAT);
			// SE�̍Đ�
			se->Play("GREAT", YAKINIKU_AREA);
			break;
		case YAKINIKU_MODE::MEDIAM:
			// PERFECT�G�t�F�N�g
			EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 64, (int)YAKINIKU_AREA.y + 32, EFFECT_TYPE::PERFECT);
			// SE�̍Đ�
			se->Play("PERFECT", YAKINIKU_AREA);
			break;
		}
		
		// ���̏Ă�����Tips�J�E���g
		TipsCountMgr->m_YakinikuSteps[(int)m_pYakiniku->GetMode()]++;
		// ���̗r�̓���Tips�J�E���g
		TipsCountMgr->m_NikuSheepTypes[(int)m_pYakiniku->GetSheepType()]++;

		m_pYakiniku->Erase();
	}
}


//**************************************************
//    �Ă����N���X
//**************************************************
//**************************************************
//    �ē��Ϗ��N���X
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

void YakinikuMode::Base::RenderFront()
{
	m_pYakiniku->pImage->SetAlpha(64);
	m_pYakiniku->pImage->Render(m_pYakiniku->GetPosX(), m_pYakiniku->GetPosY(), 256, 256, m_srcX, 0, 256, 256);
	m_pYakiniku->pImage->SetAlpha(255);
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
	// ���A�ɍs��
	m_NextMode = YAKINIKU_MODE::RARE;

	// �摜���W
	m_srcX = 0;

	// �G�t�F�N�g����
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::BAD);

	// SE�̍Đ�
	//se->Play("�q���萶����", me->GetPos());
}
//void YakinikuMode::Nama::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		���A ���@���������̓�
//===========================================================
YakinikuMode::Rare::Rare(Yakiniku *me) :Base(me)
{
	// �~�f�B�A���ɍs��
	m_NextMode = YAKINIKU_MODE::MEDIAM;

	// �摜���W
	m_srcX = 256;

	// �G�t�F�N�g����
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::GREAT);

	// SE�̍Đ�
	//se->Play("�q������", me->GetPos());
}
//void YakinikuMode::Rare::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}

//===========================================================
//		�~�f�B�A�� ���i�����Z���̓��j
//===========================================================
YakinikuMode::Mediam::Mediam(Yakiniku *me) :Base(me)
{
	// �������ɍs��
	m_NextMode = YAKINIKU_MODE::KOGETA;

	// �摜���W
	m_srcX = 256 * 2;

	// �G�t�F�N�g����
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PERFECT);

	// SE�̍Đ�
	se->Play("���ł�", me->GetPos());

	EffectMgr.AddEffect(me->GetPosX() + 128, me->GetPosY() + 120, EFFECT_TYPE::PUT);

}
//void YakinikuMode::Mediam::Update(Yakiniku *pYakiniku)
//{
//	NextMode(pYakiniku);
//}


//===========================================================
//		�E�F���_��
//===========================================================
//YakinikuMode::Weldan::Weldan(Yakiniku *me) :Base(me)
//{
//	// �����Ėq�ꂪ�a������
//	m_NextMode = YAKINIKU_MODE::KOGETA;
//
//	// �摜���W
//	m_srcX = 256 * 3;
//
//	// SE�̍Đ�
//	//se->Play("�q������", me->GetPos());
//
//}
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
	//->Play("�q������", me->GetPos());

	// �G�t�F�N�g����
	//EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::BAD);
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
//    �ē��N���X
//**************************************************

//===========================================================
//		�������E���
//===========================================================
Yakiniku::Yakiniku(tdn2DObj *image, SHEEP_TYPE SheepType) :m_SheepType(SheepType), m_pMode(nullptr), m_bErase(false), m_pos(YAKINIKU_AREA), pImage(image)
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

	// ���[�h���ς�鎞�Ԑݒ�
	m_pMode->SetChangeModeTime(g_YakiNikuModeChangeTime[(int)m]);
}


//**************************************************
//    ���N���X
//**************************************************

//===========================================================
//		�������E���
//===========================================================
Niku::Niku(const Vector2 &pos, YAKINIKU_MODE type, tdn2DObj *image, SHEEP_TYPE SheepType) :m_SheepType(SheepType), m_pos(pos), m_bErase(false), m_orgY(pos.y),
m_BoundPow(Vector2(6, -8)), m_gravity(1.0f), m_pImage(image), m_type(type), m_floor(FindFloor(pos.y)), m_bSet(false), m_bVisible(false)
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

	// �����Z���̓��Ȃ�
	if (m_type == YAKINIKU_MODE::MEDIAM)
	{
		// ���C�p�[�e�B�N��
		Particle2dManager::Effect_NikuSmoke(m_pos + Vector2(60, 60));
	}
}



//===========================================================
//		�`��
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
	// ���[�g�𑫂��Ă���
	m_percent = min(m_percent + m_percentRate, 1.0f);

	// �ρ[�Ă�����
	Particle2dManager::Effect_KiraKira(Vector2(m_pos.x, m_pos.y), Vector2(16, 16), 15, 10, 1, 70);// �ρ[�Ă�����
	FOR(4)Particle2dManager::Effect_Hinoko(Vector2(m_pos.x, m_pos.y));
}