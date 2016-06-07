#include "IEX_Expansion.h"
#include "../Sound/SoundManager.h"
#include "MousePointer.h"
#include "../CurvePoint/CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "Sheep.h"
#include "../Data/DataMNG.h"
#include "../system/system.h"
#include "../particle_2d/particle_2d.h"
#include "../Niku/Niku.h"

// �O���[�o���̈�
int g_CreateSheepFloor;	// �r�𐶐�����t���A

Sheep::Base::Base(const SheepData &data, int floor, const SheepTextParam &textparam) :
animepos(0, 0),
m_floor(floor),
process(WALK),
m_AnimePanel(0), m_bErase(false)
{
	Move[MODE::WALK] = &Sheep::Base::Walk;
	Move[MODE::CURVE] = &Sheep::Base::Curve;
	Move[MODE::PUSH] = &Sheep::Base::Push;
	Move[MODE::CRUSHED] = &Sheep::Base::Crushed;
	Move[MODE::CAUGHT] = &Sheep::Base::Caught;

	alpha = 255;
	frame = 0, animeframe = 0;
	//move = 5;/* speed */

	this->ase.animex = 0;

	memcpy_s(&m_data, sizeof(SheepData), &data, sizeof(SheepData));
	m_data.Image = data.Image;
	m_data.BoneImage = data.BoneImage;

	col_check = false;

	int addY = tdnRandom::Get(-textparam.VerticalWidth, textparam.VerticalWidth);	// �㉺������Ƃ��炷�p

	pos = Vector2((float)textparam.startX, (float)STAGE_POS_Y[floor] + (float)(LANE_WIDTH / 8) - (float)addY);
	C_MOVE.x = (float)textparam.speed;
	C_MOVE.y = 0;
}

Sheep::Base::~Base()
{}

//**************************************************

void Sheep::Base::Get_out()
{
	// Initialize�I�Ȗ����Ȃ̂���
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
	// �����p�[�e�B�N��
	Vector2 pPos = GetCenterPos();
	pPos.y += 32;
	Particle2dManager::Effect_SandCloud(pPos);

	// ���ɂ�
	if (++animeframe > m_data.Animkankaku)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	// �{���̈ړ��l
	move = C_MOVE;
	pos.x += move.x;
}

void Sheep::Base::Curve()
{
	// �����p�[�e�B�N��
	Vector2 pPos = GetCenterPos();
	pPos.y += 32;
	Particle2dManager::Effect_SandCloud(pPos);

	// ���ɂ�
	if (++animeframe > m_data.Animkankaku)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	const float pow = C_MOVE.Length();					// ���i�����������Ă�ړ��̗�
	const float addAngle = 0.011f * C_MOVE.Length();		// �T�C���J�[�u���Ȃ���̂Ɏg��

	// �T�C���J�[�u�I�Ȃ̂ŗr�̌Q����Ȃ���
	move = Vector2(cosf(m_sinAngle) * pow, sinf(m_sinAngle) * pow);
	if (m_CurveDir == DIR::UP) move.y *= -1;
	pos += move;

	// 90�x�Ȃ����Ė߂鎞
	if (m_bTurned)
	{
		// 90�x�Ȃ���I�������A�������[�h�ɖ߂�
		if ((m_sinAngle -= addAngle) <= 0) process = MODE::WALK;
	}

	// 90�x�Ȃ���O
	else
	{
		// 90�x�Ȃ���I�������A�Ȃ������t���OON
		if ((m_sinAngle += addAngle) >= PI * 0.5f) m_bTurned = true;
	}
	// �t���A�v�Z
	for (int i = STAGE_MAX - 1; i >= 0; i--)
	{
		if (pos.y >= STAGE_POS_Y[i])
		{
			m_floor = i;
			break;
		}
	}
}

void Sheep::Base::Push()
{
	// ���ɂ�
	if (++animeframe > m_data.Animkankaku)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	// �{���̈ړ��l
	move = C_MOVE;
	pos.x += move.x;
}
void Sheep::Base::Crushed()
{
	if (m_AnimePanel < 5)
	{
		if (++animeframe > 4)
		{
			animeframe = 0;
			m_AnimePanel++;
		}
	}
	else
	{
		alpha -= 2;
		if (alpha < 2)
			m_bErase = true;
	}
}
void Sheep::Base::Caught()
{
	// ���ɂ�
	if (++animeframe > m_data.Animkankaku / 2)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	// �}�E�X���W
	pos = tdnMouse::GetPos() + Vector2(-60, -60);

	// �΂̍��W

	// �}�E�X�����ꂽ�u��
	bool hanashita;
	if (m_bLeftCatch) hanashita = (tdnMouse::GetLeft() == 2);
	else hanashita = (tdnMouse::GetRight() == 2);

	if (hanashita)
	{
		// �������[�h�ɖ߂�
		process = MODE::WALK;

		// �ŒZ�t���A�ɂ���
		m_floor = FindFloor(pos.y);
		pos.y = (float)STAGE_POS_Y[m_floor] + (float)(LANE_WIDTH / 8);
	}
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
	//Get_frame_pos(animepos, m_AnimePanel);

	if (process == WALK)
	{
		/* �����Ɣ�Ⴕ�ē����ɂ��� */
		//�����āA�������Y��ɓZ�߂鏈���l����̖ʓ|�������񂾁B�B�B�����R�[�h is �т�[�Ă��ӂ�
		if( move.x > 0 ){
			shader2D->SetValue("moveAngle", .0f);
		}else{
			shader2D->SetValue("moveAngle", 0.125f);
		}
		//byu.byu->Render((int)(pos.x + ((move.x > 0) ? -64 : 64)), (int)pos.y, SIZE, SIZE, 0, 0, 128, 128, shader2D, "byun");
//		byu.byu->Render((int)(pos.x + ((move > 0) ? -64 : 64)), (int)pos.y, SIZE, SIZE, byu.animex, 0, 128, 128);
		//ase.ase->Render((int)(pos.x + ((move.x > 0) ? -32 : 32)), (int)pos.y, SIZE, SIZE, ase.animex, 0, (move.x > 0) ? -128 : 128, 512);
	}

	// �����Ă�
	if (process != CRUSHED)
	{
		m_data.Image->SetARGB(alpha, (BYTE)255, (BYTE)255, (BYTE)255);
		m_data.Image->Render((int)pos.x, (int)pos.y, m_data.SIZE, m_data.SIZE,
			(m_AnimePanel % 4) * m_data.SIZE, (m_AnimePanel / 4) * m_data.SIZE, -m_data.SIZE, m_data.SIZE,
			RS::COPY);
	}

	// ����ł�
	else
	{
		m_data.BoneImage->SetARGB(alpha, (BYTE)255, (BYTE)255, (BYTE)255);
		m_data.BoneImage->Render((int)pos.x, (int)pos.y, m_data.SIZE, m_data.SIZE,
			m_AnimePanel * m_data.SIZE, 0, (move.x >= 0) ? -m_data.SIZE : m_data.SIZE, m_data.SIZE,
			RS::COPY);
	}
}

void Sheep::Base::Be_crushed()
{
	if (col_check) return;

	m_AnimePanel = animeframe = 0;
	process = CRUSHED; 
	//obj = file[CRUSHED];
}
void Sheep::Base::Be_caught(bool bLeft)
{ 
	m_bLeftCatch = bLeft;
	process = CAUGHT; 
	//obj = file[RAN_OVER];
}


//**************************************************
//	���r
//**************************************************
Sheep::Gold::Gold(const SheepData &data, int floor, const SheepTextParam &textparam) :Base(data, floor, textparam)
{
	// ���炷
	m_seID = se->Play("����߂��r����", pos, Vector2(0, 0), true);
}
Sheep::Gold::~Gold()
{
	if(m_seID != TDNSOUND_PLAY_NONE)se->Stop("����߂��r����", m_seID);
}
void Sheep::Gold::Update()
{
	// ���N���X�̃A�b�v�f�[�g
	Sheep::Base::Update();

	// ���̍��W�ݒ�
	if(m_seID != TDNSOUND_PLAY_NONE)se->SetPos("����߂��r����", m_seID, pos);

	// ���炫��G�t�F�N�g
	if (process != CRUSHED)Particle2dManager::Effect_KiraKira(GetCenterPos(), Vector2(32, 32), 10.0f, 5.0f, 1, 90);
}


//**************************************************
//	���A���r
//**************************************************
void Sheep::Real::Update()
{
	// ���N���X�̃A�b�v�f�[�g
	Sheep::Base::Update();

	// �G�t�F�N�g
	if (process != PUSH)Particle2dManager::Effect_RealSheep(GetCenterPos() + Vector2(130, -5));
}


//**************************************************
//	�q���H���đ������r
//**************************************************
FatSheep::FatSheep(tdn2DObj *image, const Vector2 &pos, SHEEP_TYPE type) :DebuBase(image, pos, 120), m_type(type)
{

}

FatSheep::~FatSheep()
{
	// ���[�v���Ă�SE���~�߂�
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("�������r����", m_ReceiveSE);
}


//**************************************************

// manager

//**************************************************

SheepManager::SheepManager() :sp(0)
{
	m_pBoneImage = new tdn2DObj("DATA/CHR/hone_motion.png");

	g_CreateSheepFloor = 0;	// �����r�����t���A
	// ���̃����_���t���A����
	m_NextChangeFloor = MakeNextFloor(g_CreateSheepFloor);

	// �e�L�X�g����p�����[�^�ǂݍ���
	std::fstream ifs("DATA/Text/Param/sheep.txt");

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	// �o�����W�ǂݍ���
	ifs >> skip;
	ifs >> m_TextParam.startX;

	// �o���Ԋu(�~���b)�ǂݍ���
	ifs >> skip;
	ifs >> m_TextParam.AppTime;

	// �o���̏㉺���ǂݍ���
	ifs >> skip;
	ifs >> m_TextParam.VerticalWidth;

	// ���x�ǂݍ���
	ifs >> skip;
	ifs >> m_TextParam.speed;

	// �������đ������r�̑��x
	ifs >> skip;
	ifs >> m_TextParam.FatSheepAccel;

	// �r���ǂݍ���
	for (int i = 0; i < (int)SHEEP_TYPE::MAX; i++)
	{
		ifs >> skip;
		
		// �r�摜�ǂݍ���
		ifs >> skip;
		char path[MAX_PATH];
		ifs >> path;
		m_TextParam.data[i].Image = new tdn2DObj(path);
		m_TextParam.data[i].BoneImage = m_pBoneImage;

		// �łԂ�����
		ifs >> skip;
		ifs >> path;
		m_pFatSheepImages[i] = new tdn2DObj(path);

		// ���_�{���ǂݍ���
		ifs >> skip;
		ifs >> m_TextParam.data[i].magnification;

		// �R�}�̃T�C�Y
		ifs >> skip;
		ifs >> m_TextParam.data[i].SIZE;

		// �A�j������
		ifs >> skip;
		ifs >> m_TextParam.data[i].NumPanel;

		// �R�}�i�߂�t���[��
		ifs >> skip;
		ifs >> m_TextParam.data[i].Animkankaku;

		// �o����
		ifs >> skip;
		ifs >> m_TextParam.data[i].percentage;
	}
	m_List.clear();
	// �^�C�}�[�ݒ�
	m_CurrentTime = clock();
}

SheepManager::~SheepManager()
{
	for (int i = 0; i < (int)SHEEP_TYPE::MAX; i++)
	{
		SAFE_DELETE(m_TextParam.data[i].Image);
		delete m_pFatSheepImages[i];
	}
	delete m_pBoneImage;

	for (auto it : m_List) delete it;
	for (auto it : m_FatList) delete it;
	m_List.clear();
	m_FatList.clear();
}

//**************************************************

void SheepManager::create(int floor)
{
	// �����_���ɗr�̃^�C�v
	float r = tdnRandom::Get(.0f, 100.0f);
	for (int i = (int)SHEEP_TYPE::MAX - 1; i >= 0; i--)
	{
		// �����_���Ɏ擾�����p�[�Z���g���ݒ肵��percentage�͈͓��������炻���𐶐�
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
				assert(0);	// ��O����
				break;
			}

			break;
		}
		else r -= m_TextParam.data[i].percentage;
	}
}

void SheepManager::Update()
{
	// �f�o�b�O�p
	{
		if (KeyBoardTRG(KB_DOT))
		{
			//m_ChangeLaneTime = 0;
			g_CreateSheepFloor = 0;
		}
		else if (KeyBoardTRG(KB_SLASH))
		{
			//m_ChangeLaneTime = 0;
			g_CreateSheepFloor = 1;
		}
		else if (KeyBoardTRG(KB_UNDER_BAR))
		{
			//m_ChangeLaneTime = 0;
			g_CreateSheepFloor = 2;
		}
	}

	// ���[���ύX����
	//if (++m_ChangeLaneTime > CHANGE_LANE_TIME)
	//{
	//	// ���ԃ��Z�b�g
	//	m_ChangeLaneTime = 0;
	//
	//	// ���[����ς���
	//	g_CreateSheepFloor = m_NextChangeFloor;
	//
	//	// ���̃����_���t���A����
	//	m_NextChangeFloor = MakeNextFloor(g_CreateSheepFloor);
	//}
	//else if (m_ChangeLaneTime == CHANGE_LANE_TIME / 4)
	//{
	//	// ��UI�}�l�[�W���Ƀ|�b�v�A�b�v�����肢����
	//}

	// �O��̎��Ԃƍ��̎��Ԃ̍���
	UINT delta = clock() - m_CurrentTime;
	bool bCreate(false);

	// �O�񂩂�̌o�ߎ��ԂƏo���Ԋu�ɉ����āA�r�𐶐�
	for (int j = delta; j > m_TextParam.AppTime; j -= m_TextParam.AppTime)
	{
		create(g_CreateSheepFloor);
		bCreate = true;
	}

	// ���ݎ����ݒ�
	if (bCreate)m_CurrentTime = clock();

	auto CheckCatch = [](bool *OutbLeft, bool bNikuHold)
	{
		if (tdnMouse::GetRight() == 3)
		{
			*OutbLeft = false;
			return true;
		}
		else if (!g_bDogSetFrame && tdnMouse::GetLeft() == 3 && !bNikuHold)
		{
			*OutbLeft = true;
			return true;
		}
		else return false;
	};
	// �͂ޏ���
	bool bLeftCatch;
	if (CheckCatch(&bLeftCatch, NikuMgr->isNikuHold()))
	{
		for (auto& it : m_List)
		{
			if ((it->GetCenterPos() - tdnMouse::GetPos()).LengthSq() < 60 * 60)
			{
				// �r�߂܂���SE
				se->Play((it->GetType() == SHEEP_TYPE::REAL) ? "���A���͂�" : "�r�͂�", *it->Get_pos());

				// �߂܂�Ă郂�[�h�ɕύX
				it->Be_caught(bLeftCatch);
				break;
			}
		}
	}


	// �r�X�V
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

	// �q���H���đ������r�X�V
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
	// �r�`��
	for (auto it : m_List)
	{
		it->Render();
	}

	// ���H���đ������r
	for (auto it : m_FatList) it->Render();
}

void SheepManager::CreateFatSheep(Sheep::Base *sheep)
{
	sheep->Erase();	// ���̗r������
	FatSheep *set = new FatSheep(m_pFatSheepImages[(int)sheep->GetType()], Vector2(sheep->Get_pos()->x, (float)STAGE_POS_Y[sheep->Get_floor()] - 30), sheep->GetType());// �������r����
	set->SetFloor(sheep->Get_floor());	// �t���A�ݒ�
	m_FatList.push_back(set);			// ���X�g�ɓ˂�����

	// ���p�[�e�B�N��
	Particle2dManager::Effect_FatSmoke(sheep->GetCenterPos());
}

int SheepManager::MakeNextFloor(int current)
{
	int r;	// �����_���Ȓl������ϐ�
	do
	{
		// �������[���ɕς��Ȃ��悤�Ƀ��[�v������
		r = rand() % STAGE_MAX;
	} while (r == current);
	return r;
}



SheepManager *g_pSheepMgr = nullptr;