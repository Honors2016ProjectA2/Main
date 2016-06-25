#include "TDNLIB.h"
#include "Bokusou.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIManager.h"
#include "Effect\EffectManager.h"
#include "../particle_2d/particle_2d.h"
#include "../pie_graph/pie_graph.h"

// �O���[�o���ϐ�
int g_ModeChangeTime[(int)BOKUSOU_MODE::MAX];			// ���[�h���ς���Ă�������

// �ÓI�����o�ϐ�
BokusouManager *BokusouManager::pInstance = nullptr;	// 1�����Ȃ�����

//**************************************************
//    �q���Ǘ��N���X
//**************************************************
BokusouManager::BokusouManager():m_PrevPoint(0)
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

	m_pBokusouTarget = new BokusouTarget;
	m_pBokusouTarget->ChangePos(m_CreatePosList[m_NextPoint].pos);
}

BokusouManager::~BokusouManager()
{
	for (auto it : m_list) delete it;
	m_list.clear();
	delete m_pBokusouTarget;
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
		//EffectMgr.AddEffect((int)m_CreatePosList[m_NextPoint].pos.x+64, (int)m_CreatePosList[m_NextPoint].pos.y+64, EFFECT_TYPE::PUT);
		UIMNG.GraphAction();//  �A�N�V����

		m_CreateTimer = 0;

		// ����
		//Bokusou *set = new Bokusou(m_CreatePosList[m_NextPoint].pos, m_NextPoint);	// ���W���X�g���烉���_����
		//set->SetFloor(m_CreatePosList[m_NextPoint].floor);
		//m_list.push_back(set);

		m_PrevPoint = m_NextPoint;

		//m_CreatePosList[m_NextPoint].pos
		UIMNG.AddSendPower("Data/Power.png", Vector3(40, 40, 0), Vector3(0, 400, 0), Vector3(100, 600, 0),
			Vector3(m_CreatePosList[m_NextPoint].pos.x + 64, m_CreatePosList[m_NextPoint].pos.y + 64, 0), 48, 114514, true);

		// ���̐������W
		while (true)
		{
			m_NextPoint = tdnRandom::Get(0, m_CreatePosList.size() - 1);
			if (m_NextPoint == m_PrevPoint) continue;

			// ���Ԃ��ĂȂ��t���A����
			bool find(true);
			for (auto &it : m_list)
			{
				if (it->GetPoint() == m_NextPoint)
				{
					find = false;
					break;
				}
			}

			if (find) break;
		}

		// �^�[�Q�b�g�̈ʒu�ύX
		//m_pBokusouTarget->ChangePos(m_CreatePosList[m_NextPoint].pos);
	}

	m_pBokusouTarget->Update(this);

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

	m_pBokusouTarget->Render();
}

void BokusouManager::RenderFront()
{
	for (auto it : m_list) it->RenderFront();
	m_pBokusouTarget->RenderFront();

	//if (KeyBoard(KB_1))
	//{
	//	for (UINT i = 0; i < m_CreatePosList.size(); i++)
	//	{
	//		tdnPolygon::Rect(m_CreatePosList[i].pos.x, m_CreatePosList[i].pos.y, 128, 128, RS::COPY, 0xffffffff);
	//	}
	//}
}

void BokusouManager::CreateByBazier()
{
	// �G�t�F�N�g
	EffectMgr.AddEffect((int)m_CreatePosList[m_PrevPoint].pos.x + 64, (int)m_CreatePosList[m_PrevPoint].pos.y + 64, EFFECT_TYPE::PUT);

	// ����
	Bokusou *set = new Bokusou(m_CreatePosList[m_PrevPoint].pos, m_PrevPoint);
	set->SetFloor(m_CreatePosList[m_PrevPoint].floor);
	m_list.push_back(set);

	// �^�[�Q�b�g�̈ʒu�ύX
	m_pBokusouTarget->ChangePos(m_CreatePosList[m_NextPoint].pos);
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

void BokusouMode::Base::RenderFront()
{
	m_pImage->SetARGB(0x80ffffff);
	m_pImage->Render(pBokusou->GetPosX(), pBokusou->GetPosY(), 128, 128, m_srcX, 0, 128, 128);
	m_pImage->SetARGB(0xffffffff);
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
	//se->Play("�q���萶����", me->GetPos());
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
	//se->Play("�q���萶����", me->GetPos());
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
	//se->Play("�q���萶����", me->GetPos());

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
	se->Play("�q���萶����", me->GetPos());

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
Bokusou::Bokusou(const Vector2 &pos, int point) :m_point(point), m_pMode(nullptr), m_bErase(false), m_pos(pos), m_pBokusouFlower(new tdn2DObj("DATA/�q��/�q���̎�.png")), m_pBokusouRoll(new tdn2DObj("DATA/�q��/�q��.png"))
{
	// ���[�h������(�q������J�n)
	this->ChangeMode(BOKUSOU_MODE::BORN);
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

BokusouTarget::BokusouTarget() :m_pKusa(new tdn2DObj("DATA/�q��/�q���̎�.png")), m_pos(Vector2(0, 0)), m_angle(0), m_alpha(0), m_AnimFrame(0),
ANIM_FRAME(30)
{
	// �O���t
	m_pGraph = new Pie_graph("DATA/UI/Circle/SpiritCircle_gage2.png");

	m_pTargetAnim = new tdn2DAnim("DATA/traget.png");
	m_pTargetAnim->OrderJump(ANIM_FRAME, .65f, .2f);
	m_pTargetAnim->Action();
}

BokusouTarget::~BokusouTarget()
{
	delete m_pKusa;
	delete m_pTargetAnim;
	delete m_pGraph;
}

void BokusouTarget::Update(BokusouManager *pMgr)
{
	// ���������Z����
	m_alpha = min(m_alpha + 8, 128);
	m_pKusa->SetARGB(m_alpha, (BYTE)255, (BYTE)255, (BYTE)255);

	// �q������ڂ��Ȃ�΍�
	if(m_srcX < 3 * 128)m_srcX = ((int)(pMgr->GetGaugePercentage() * 100) / 25) * 128;

	// �Q�[�W����
	m_pGraph->Set_percent(pMgr->GetGaugePercentage());

	// �^�[�Q�b�g�̃A�j���[�V����
	m_pTargetAnim->Update();
	if (++m_AnimFrame >= ANIM_FRAME * 2)
	{
		m_AnimFrame = 0;
		m_pTargetAnim->Action();
	}
}

void BokusouTarget::Render()
{
	// �O���t
	//m_pGraph->Render((int)m_pos.x, (int)m_pos.y, 128, 128, 0, 0, 128, 128);

	// ��
	m_pKusa->Render((int)m_pos.x, (int)m_pos.y, 128, 128, m_srcX, 0, 128, 128);
	
	// �^�[�Q�b�g
	//m_pTargetAnim->Render((int)m_pos.x - 6, (int)m_pos.y);
}

void BokusouTarget::RenderFront()
{
	// ��
	m_pKusa->SetARGB(0x80ffffff);
	m_pKusa->Render((int)m_pos.x, (int)m_pos.y, 128, 128, m_srcX, 0, 128, 128);
	m_pKusa->SetARGB(0xffffffff);

	// �^�[�Q�b�g
	//m_pTargetAnim->SetARGB(0x80ffffff);
	//m_pTargetAnim->Render((int)m_pos.x - 6, (int)m_pos.y);
	//m_pTargetAnim->SetARGB(0xffffffff);
}

void BokusouTarget::ChangePos(const Vector2 &pos)
{
	m_pos = pos;
	m_alpha = 0;
	m_angle = 0;
	m_srcX = 0;
}