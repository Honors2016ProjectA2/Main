#include "TDNLIB.h"
#include "Bokusou.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIManager.h"

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

		m_CreatePosList.push_back(Vector2(x, (float)STAGE_POS_Y[floor] + LANE_WIDTH / 4));
	}
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
	//UIMNG.
	if (++m_CreateTimer > m_CREATETIME)
	{
		// �q�������I�I
		m_CreateTimer = 0;
		m_list.push_back(new Bokusou(m_CreatePosList[rand() % m_CreatePosList.size()]));	// ���W���X�g���烉���_����
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
void BokusouMode::Base::Update(Bokusou *pBokusou)
{
	// �ݒ莞�ԉ߂�����
	if (++m_timer > m_limit)
	{
		NextMode(pBokusou);	// ���̃��[�h��
	}
}

void BokusouMode::Base::Render(Bokusou *pBokusou)
{
	m_pImage->Render(pBokusou->GetPosX(), pBokusou->GetPosY(), 128, 128, m_srcX, 0, 128, 128);
}

void BokusouMode::Base::NextMode(Bokusou *pBokusou)
{
	// ���̃��[�h�ɕύX
	pBokusou->ChangeMode(m_NextMode);
}

//===========================================================
//		�o�t��
//===========================================================
BokusouMode::Hutaba::Hutaba(tdn2DObj *image) :Base(image)
{
	// �{�t�ɍs��
	m_NextMode = BOKUSOU_MODE::HONBA;

	// �摜���W
	m_srcX = 0;
}
//void BokusouMode::Hutaba::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		�{�t����
//===========================================================
BokusouMode::Honba::Honba(tdn2DObj *image) :Base(image)
{
	// �ڂ݂ɍs��
	m_NextMode = BOKUSOU_MODE::TSUBOMI;

	// �摜���W
	m_srcX = 128;
}
//void BokusouMode::Honba::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		�ڂ�
//===========================================================
BokusouMode::Tsubomi::Tsubomi(tdn2DObj *image) :Base(image)
{
	// �炢���ɍs��
	m_NextMode = BOKUSOU_MODE::SAITA;

	// �摜���W
	m_srcX = 256;
}
//void BokusouMode::Tsubomi::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}


//===========================================================
//		�������\
//===========================================================
BokusouMode::Saita::Saita(tdn2DObj *image) :Base(image)
{
	// �����Ėq�ꂪ�a������
	m_NextMode = BOKUSOU_MODE::BORN;

	// �摜���W
	m_srcX = 128 * 3;
}
//void BokusouMode::Saita::Update(Bokusou *pBokusou)
//{
//	NextMode(pBokusou);
//}

//===========================================================
//		�q���a��
//===========================================================
BokusouMode::Born::Born(tdn2DObj *image) :Base(image)
{
	// �摜���W
	m_srcX = 0;
}
void BokusouMode::Born::Update(Bokusou *pBokusou)
{

}

//**************************************************
//    �q���N���X
//**************************************************

//===========================================================
//		�������E���
//===========================================================
Bokusou::Bokusou(const Vector2 &pos) :m_pMode(nullptr), m_bErase(false), m_pos(pos)
{
	// �摜������
	m_pBokusouFlower = new tdn2DObj("DATA/�q��/�q���̎�.png");
	m_pBokusouRoll = new tdn2DObj("DATA/�q��/�q��.png");

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
	m_pMode->Update(this);
}



//===========================================================
//		�`��
//===========================================================
void Bokusou::Render()
{
	m_pMode->Render(this);
}

void Bokusou::ChangeMode(BOKUSOU_MODE m)
{
	if (m_pMode)delete m_pMode;

	switch (m)
	{
	case BOKUSOU_MODE::HUTABA:
		m_pMode = new BokusouMode::Hutaba(m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::HONBA:
		m_pMode = new BokusouMode::Honba(m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::TSUBOMI:
		m_pMode = new BokusouMode::Tsubomi(m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::SAITA:
		m_pMode = new BokusouMode::Saita(m_pBokusouFlower);
		break;

	case BOKUSOU_MODE::BORN:
		m_pMode = new BokusouMode::Born(m_pBokusouRoll);
		break;
	}

	// ���[�h���ς�鎞�Ԑݒ�
	m_pMode->SetChangeModeTime(g_ModeChangeTime[(int)m]);
}