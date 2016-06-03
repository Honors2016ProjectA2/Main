#include "IEX_Expansion.h"
#include "../CharaBase/CharaBase.h"
#include "../Stage/StageMNG.h"
#include "../Sheep/Sheep.h"
#include "watchman.h"
#include "../Data/DataMNG.h"
#include "../Effect/EffectManager.h"
#include "../Niku/Niku.h"
#include "../Sound/SoundManager.h"
#include "../particle_2d/particle_2d.h"

//**************************************************
//    ���
//**************************************************
void Enemy::Base::MoveUpdate()
{
	m_pos += m_MoveVec * m_speed;
}

//**************************************************
//    �T
//**************************************************
Enemy::Wolf::Wolf(tdn2DObj *image, tdn2DObj *pniku, tdn2DObj *pHone, int floor, float speed, int nikustopTime) : Base(image, floor, speed), m_pNikukutteru(pniku), m_EAT_NIKU_TIMER(nikustopTime), m_OrgSpeed(speed), m_pHoneImage(pHone)
{
	// �C�j�V�����C�U�����Ȃ�
	W = 120;
	H = 120;

	// ���W������
	m_pos.x = 1280;
	m_pos.y = (float)STAGE_POS_Y[floor] + LANE_WIDTH / 4;

	// ���[�h�|�C���^������k
	ModeFunk[(int)MODE::RUN] = &Wolf::Run;
	ModeFunk[(int)MODE::NIKU] = &Wolf::Niku;
	ModeFunk[(int)MODE::DEAD] = &Wolf::Dead;

	// �ŏ��͂���
	ChangeMode(MODE::RUN);
}

void Enemy::Wolf::Update()
{
	// ���[�h�֐����s
	(this->*ModeFunk[(int)m_mode])();

	// ���W�X�V
	Enemy::Base::MoveUpdate();
}

void Enemy::Wolf::Run()
{
	// �A�j���X�V
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 4) m_AnimePanel = 0;
	}
}


void Enemy::Wolf::Niku()
{
	// �A�j���X�V
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 4) m_AnimePanel = 0;
	}

	// ������
	if (++m_EatNikuTimer > m_EAT_NIKU_TIMER)
	{
		EffectMgr.AddEffect((int)m_pos.x + 50, (int)m_pos.y + 30, EFFECT_TYPE::ClEAR);
		EffectMgr.AddEffect((int)m_pos.x - 80, (int)m_pos.y + 60, EFFECT_TYPE::PUSH);

		// ��
		Particle2dManager::Effect_FatSmoke(GetCenterPos());

		// ����
		EnemyMgr->CreateFatWolf(this, m_type, m_SheepType);
	}
}

void Enemy::Wolf::Dead()
{
	if (m_AnimePanel < 5)
	{
		if (++m_AnimeFrame > 4)
		{
			m_AnimeFrame = 0;
			m_AnimePanel++;
		}
	}
	else
	{
		// �������Ă���
		m_alpha -= 2;
		if (m_alpha < 2)
			m_bErase = true;	// ����
	}
}


void Enemy::Wolf::Render()
{
	tdn2DObj *pImage;
	switch (m_mode)
	{
	case Enemy::Wolf::MODE::RUN:
		pImage = m_pImage;
		break;
	case Enemy::Wolf::MODE::DEAD:
		pImage = m_pHoneImage;
		break;
	case Enemy::Wolf::MODE::NIKU:
		pImage = m_pNikukutteru;
		break;
	}
	pImage->SetARGB(m_alpha, (BYTE)255, (BYTE)255, (BYTE)255);
	pImage->Render((int)m_pos.x, (int)m_pos.y, W, H, (m_AnimePanel % 4) * W, (m_AnimePanel / 4) * H, W, H);
}

//**************************************************
//    ���H���đ������T
//**************************************************
FatWolf::FatWolf(tdn2DObj *image, const Vector2 &pos, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType) :DebuBase(image, pos), m_type(type), m_SheepType(SheepType)
{
	switch (type)
	{
	case FAT_WOLF_TYPE::SMALL:
		m_radius = 90;
		break;
	case FAT_WOLF_TYPE::MIDDLE:
		m_radius = 90;
		break;

	case FAT_WOLF_TYPE::LARGE:
		m_radius = 120;
		break;
	}
}

FatWolf::~FatWolf()
{
	// ���[�v���Ă�SE���~�߂�
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("�������r����", m_ReceiveSE);
}

void FatWolf::Update()
{
	// ���N���X�̍X�V
	DebuBase::Update();

	if (m_SheepType == SHEEP_TYPE::GOLD)
	{
		static int KiraKiraCoolTime = 0;
		// ���炫��p�[�e�B�N��
		if (++KiraKiraCoolTime > 4)
		{
			KiraKiraCoolTime = 0;
			Particle2dManager::Effect_KiraKira(GetCenterPos(), Vector2(m_radius, m_radius), 25.0f, 10.0f);
		}
	}
}

void FatWolf::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, 0, (int)m_type * 240, 240, 240);
}



//**************************************************

// manager

//**************************************************

EnemyManager::EnemyManager() :m_CreateTimer(0)
{
	m_list.clear();
	m_FatList.clear();

	// �e�L�X�g����f�[�^�ǂݍ���
	std::fstream ifs("DATA/Text/Param/tekidata.txt");
	assert(ifs);	// �t�@�C�����Ȃ��I

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	// �G�쐬���ԓǂݍ���
	ifs >> skip;
	ifs >> m_CREATETIME;

	// �T�̑��x�ǂݍ���
	ifs >> skip;
	ifs >> m_EnemySpeed[(int)ENEMY_TYPE::WOLF];

	// �ɂ�
	ifs >> skip;
	ifs >> m_NikuTime;

	m_NextFloor = tdnRandom::Get(0, 2);
}

void EnemyManager::Initialize()
{
	// �������ԏ�����
	m_CreateTimer = 0;

	// �G�摜�̓ǂݍ���
	m_pImages[(int)ENEMY_TYPE::WOLF] = new tdn2DObj("DATA/CHR/�u�I�v���ړ�.png");
	m_pNikukutteru = new tdn2DObj("DATA/CHR/kuruma back.png");
	m_pHoneImage = new tdn2DObj("DATA/CHR/hone_motion.png");
	m_pFatWolfImages[(int)SHEEP_TYPE::NOMAL] = new tdn2DObj("DATA/CHR/sinnnyou tubureru.png");
	m_pFatWolfImages[(int)SHEEP_TYPE::GOLD] = new tdn2DObj("DATA/CHR/sinnnyou hajike.png");
	m_pFatWolfImages[(int)SHEEP_TYPE::REAL] = new tdn2DObj("DATA/CHR/sinnnyou detekuru.png");
}

void EnemyManager::Release()
{
	FOR ((int)ENEMY_TYPE::MAX)
	{
		delete m_pImages[i];
	}
	Clear();
	delete m_pNikukutteru;
	delete m_pHoneImage;
	FOR((int)SHEEP_TYPE::MAX) delete m_pFatWolfImages[i];
}

//**************************************************

void EnemyManager::Create(int floor, ENEMY_TYPE type)
{
	Enemy::Wolf *set = nullptr;

	switch (type)
	{
	case ENEMY_TYPE::WOLF:
		set = new Enemy::Wolf(m_pImages[(int)ENEMY_TYPE::WOLF], m_pNikukutteru, m_pHoneImage, floor, m_EnemySpeed[(int)ENEMY_TYPE::WOLF], m_NikuTime);
		break;
	default:
		assert(0);	// ��O����
		break;
	}
	m_list.push_back(set);
}

void EnemyManager::Update()
{
	// �G�����^�C�}�[
	if (++m_CreateTimer > m_CREATETIME)
	{
		m_CreateTimer = 0;
		Create(m_NextFloor, ENEMY_TYPE::WOLF);

		// ���̃t���A�쐬
		m_NextFloor = tdnRandom::Get(0, 2);
	}
	else if (m_CREATETIME - m_CreateTimer == 80)
	{
		// ���Z���T�[
		Niku *pNiku = NikuMgr->GetNiku();
		if (pNiku)
		{
			if(pNiku->isSeted())m_NextFloor = pNiku->GetFloor();
		}
		
		// ���Ȃ������玟�̓f�u�r�t���A
		else if (!g_pSheepMgr->GetFatList()->empty())
		{
			m_NextFloor = (*g_pSheepMgr->GetFatList()->begin())->GetFloor();
		}

		// �|�b�v�A�b�v
		EffectMgr.AddEffect(1100, STAGE_POS_Y[m_NextFloor] + LANE_WIDTH / 2, EFFECT_TYPE::NOTICE);
	}
	for (auto it = m_list.begin(); it != m_list.end();)
	{
		// �G�����X�V
		(*it)->Update();

		// �����t���O�����Ă���
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_list.erase(it); 
		}
		else it++;
	}

	for (auto it = m_FatList.begin(); it != m_FatList.end();)
	{
		// �G�����X�V
		(*it)->Update();

		// �����t���O�����Ă���
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_FatList.erase(it);
		}
		else it++;
	}
}

void EnemyManager::Render()
{
	for (auto it: m_list)
		it->Render();
	for (auto it : m_FatList)
		it->Render();
}

void EnemyManager::Clear()
{
	// �G�f�[�^�S����
	for (auto it : m_list) delete it;
	m_list.clear();
	for (auto it : m_FatList)delete it;
	m_FatList.clear();
}

void EnemyManager::CreateFatWolf(Enemy::Wolf *wolf, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType)
{
	wolf->Erase();	// ���̗r������
	FatWolf *set = new FatWolf(m_pFatWolfImages[(int)SheepType], Vector2(wolf->GetPos().x - 60, (float)STAGE_POS_Y[wolf->GetFloor()] - 30), type, SheepType);// �������T����
	set->SetFloor(wolf->GetFloor());	// �t���A�ݒ�
	m_FatList.push_back(set);			// ���X�g�ɓ˂�����
}