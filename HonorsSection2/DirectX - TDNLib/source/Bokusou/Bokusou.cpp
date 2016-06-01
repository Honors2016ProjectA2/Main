#include "TDNLIB.h"
#include "Bokusou.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIManager.h"
#include "Effect\EffectManager.h"
#include "../particle_2d/particle_2d.h"

// �O���[�o���ϐ�
int g_ModeChangeTime[(int)BOKUSOU_MODE::MAX];			// ���[�h���ς���Ă�������

// �ÓI�����o�ϐ�
BokusouManager *BokusouManager::pInstance = nullptr;	// 1�����Ȃ�����

//**************************************************
//    �q���Ǘ��N���X
//**************************************************
BokusouManager::BokusouManager()
{
	// �e�L�X�g����f�[�^�ǂݍ���
	std::ifstream ifs("DATA/Text/Param/bokusou.txt");
	assert(ifs);	//�@�f�[�^�Ȃ�

	char skip[64];	// �ǂݔ�΂��p

	ifs >> skip;

	// �q���Ԃ̕ω����ԓǂݍ���
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::HUTABA];
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::HONBA];
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::TSUBOMI];
	ifs >> g_ModeChangeTime[(int)BOKUSOU_MODE::SAITA];
	g_ModeChangeTime[(int)BOKUSOU_MODE::BORN];				// �q����Ԃ͎��Ԃ̕K�v�Ȃ�

	// �q���퐶�����ԓǂ݂���
	ifs >> skip;
	ifs >> m_CREATETIME;

	// �q���퐶�����W�ǂݍ���
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

	// ���̐������W
	m_NextPoint = tdnRandom::Get(0, m_CreatePosList.size() - 1);
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
	// �q���̎�Q�[�W����
	UIMNG.SetGraph((float)m_CreateTimer / m_CREATETIME);
	if (++m_CreateTimer > m_CREATETIME)
	{
		// �q�����������u��
		EffectMgr.AddEffect((int)m_CreatePosList[m_NextPoint].pos.x+64, (int)m_CreatePosList[m_NextPoint].pos.y+64, EFFECT_TYPE::PUT);
		UIMNG.GraphAction();//  �A�N�V����

		// �q�������I�I
		m_CreateTimer = 0;

		Bokusou *set = new Bokusou(m_CreatePosList[m_NextPoint].pos);	// ���W���X�g���烉���_����
		set->SetFloor(m_CreatePosList[m_NextPoint].floor);
		m_list.push_back(set);

		// ���̐������W
		m_NextPoint = tdnRandom::Get(0, m_CreatePosList.size() - 1);
	}

	for (auto it = m_list.begin(); it != m_list.end();)
	{
		// �q���X�V
		(*it)->Update();

		// �����`�F�b�N
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
//    �q���Ϗ��N���X
//**************************************************
void BokusouMode::Base::Update()
{
	// �ݒ莞�ԉ߂�����
	if (++m_timer > m_limit)
	{
		NextMode();	// ���̃��[�h��
	}
}

void BokusouMode::Base::Render()
{
	m_pImage->Render(pBokusou->GetPosX(), pBokusou->GetPosY(), 128, 128, m_srcX, 0, 128, 128);
}

void BokusouMode::Base::NextMode()
{
	// ���̃��[�h�ɕύX
	pBokusou->ChangeMode(m_NextMode);
}

//===========================================================
//		�o�t��
//===========================================================
BokusouMode::Hutaba::Hutaba(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// �{�t�ɍs��
	m_NextMode = BOKUSOU_MODE::HONBA;

	// �摜���W
	m_srcX = 0;

	// SE�̍Đ�
	se->Play("�q���萶����", me->GetPos());
}
//void BokusouMode::Hutaba::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		�{�t����
//===========================================================
BokusouMode::Honba::Honba(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// �ڂ݂ɍs��
	m_NextMode = BOKUSOU_MODE::TSUBOMI;

	// �摜���W
	m_srcX = 128;

	// SE�̍Đ�
	se->Play("�q������", me->GetPos());
}
//void BokusouMode::Honba::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		�ڂ�
//===========================================================
BokusouMode::Tsubomi::Tsubomi(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// �炢���ɍs��
	m_NextMode = BOKUSOU_MODE::SAITA;

	// �摜���W
	m_srcX = 256;

	// SE�̍Đ�
	se->Play("�q������", me->GetPos());
}
//void BokusouMode::Tsubomi::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}


//===========================================================
//		�������\
//===========================================================
BokusouMode::Saita::Saita(Bokusou *me, tdn2DObj *image) :Base(me, image)
{
	// �����Ėq�ꂪ�a������
	m_NextMode = BOKUSOU_MODE::BORN;

	// �摜���W
	m_srcX = 128 * 3;

	// SE�̍Đ�
	se->Play("�q������", me->GetPos());

}
//void BokusouMode::Saita::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		�q���a��
//===========================================================
BokusouMode::Born::Born(Bokusou *me, tdn2DObj *image) :Base(me, image), m_KiraKiraCoolTime(0)
{
	// �摜���W
	m_srcX = 0;

	// SE�̍Đ�
	se->Play("�q������", me->GetPos());

	// �G�t�F�N�g����
	EffectMgr.AddEffect((int)me->GetPos().x + 64, (int)me->GetPos().y + 64, EFFECT_TYPE::PLUS);

}
void BokusouMode::Born::Update()
{
	// ���炫��p�[�e�B�N��
	if (++m_KiraKiraCoolTime > 4)
	{
		m_KiraKiraCoolTime = 0;
		Particle2dManager::Effect_KiraKira(pBokusou->GetCenterPos(), Vector2(56, 56), 15.0f, 10.0f);
	}
}

//**************************************************
//    �q���N���X
//**************************************************

//===========================================================
//		�������E���
//===========================================================
Bokusou::Bokusou(const Vector2 &pos) :m_pMode(nullptr), m_bErase(false), m_pos(pos), m_pBokusouFlower(new tdn2DObj("DATA/�q��/�q���̎�.png")), m_pBokusouRoll(new tdn2DObj("DATA/�q��/�q��.png"))
{
	// ���[�h������(�o�t�ǂ���J�n)
	this->ChangeMode(BOKUSOU_MODE::HUTABA);
}

Bokusou::~Bokusou()
{
	delete m_pMode;
	delete m_pBokusouFlower;
	delete m_pBokusouRoll;
}


//===========================================================
//		�X�V
//===========================================================
void Bokusou::Update()
{
	m_pMode->Update();
}



//===========================================================
//		�`��
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

	// ���[�h���ς�鎞�Ԑݒ�
	m_pMode->SetChangeModeTime(g_ModeChangeTime[(int)m]);
}