#include "TDNLIB.h"
#include "IEX_Expansion.h"
#include "../Stage/StageMNG.h"
#include "watchman.h"
#include "../Data/DataMNG.h"


//**************************************************
//    ���
//**************************************************
void Enemy::Base::MoveUpdate()
{
	m_pos += m_MoveVec*m_speed;
}

//**************************************************
//    �T
//**************************************************
Enemy::Wolf::Wolf(tdn2DObj *image, int floor, float speed) :Base(image, floor, speed)
{
	// �C�j�V�����C�U�����Ȃ�
	W = 120;
	H = 120;
	// �ړ��̌���(��)
	m_MoveVec = Vector2(-1, 0);

	// ���W������
	m_pos.x = 1280;
	m_pos.y = (float)STAGE_POS_Y[floor] + LANE_WIDTH / 4;
}

void Enemy::Wolf::Update()
{
	// �A�j���X�V
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 3) m_AnimePanel = 0;
	}

	// ���W�X�V
	Enemy::Base::MoveUpdate();
}

void Enemy::Wolf::Render()
{
	m_pImage->Render((int)m_pos.x, (int)m_pos.y, W, H, (m_AnimePanel % 4) * W, (m_AnimePanel / 4) * H, W, H);
}

//**************************************************
//    ���e�I
//**************************************************
Enemy::Meteo::Meteo(tdn2DObj *image, int floor, float speed) :Base(image, floor, speed)
{
	// �C�j�V�����C�U�����Ȃ�
	W = 120;
	H = 120;
	// �ړ��̌���(��)
	m_MoveVec = Vector2(-1, 0);
}

void Enemy::Meteo::Update()
{
	// ���W�X�V
	Enemy::Base::MoveUpdate();
}

void Enemy::Meteo::Render()
{
	m_pImage->Render((int)m_pos.x, (int)m_pos.y);
}

//**************************************************

// manager

//**************************************************

EnemyManager::EnemyManager() :m_CreateTimer(0)
{
	m_list.clear();

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
}

void EnemyManager::Initialize()
{
	// �������ԏ�����
	m_CreateTimer = 0;

	// �G�摜�̓ǂݍ���
	m_pImages[(int)ENEMY_TYPE::WOLF] = new tdn2DObj("DATA/CHR/�u�I�v���ړ�.png");
	m_pImages[(int)ENEMY_TYPE::METEO] = new tdn2DObj("DATA/CHR/�u�I�v���ړ�.png");
}

void EnemyManager::Release()
{
	FOR ((int)ENEMY_TYPE::MAX)
	{
		delete m_pImages[i];
	}
	Clear();
}

//**************************************************

void EnemyManager::Create(int floor, ENEMY_TYPE type)
{
	Enemy::Base *set = nullptr;

	switch (type)
	{
	case ENEMY_TYPE::WOLF:
		set = new Enemy::Wolf(m_pImages[(int)ENEMY_TYPE::WOLF], floor, m_EnemySpeed[(int)ENEMY_TYPE::WOLF]);
		break;
	case ENEMY_TYPE::METEO:
		set = new Enemy::Meteo(m_pImages[(int)ENEMY_TYPE::METEO], floor, m_EnemySpeed[(int)ENEMY_TYPE::METEO]);
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
		Create(rand() % 3, ENEMY_TYPE::WOLF);
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
}

void EnemyManager::Render()
{
	for (auto it: m_list) it->Render();
}

void EnemyManager::Clear()
{
	// �G�f�[�^�S����
	for (auto it : m_list) delete it;
	m_list.clear();
}
