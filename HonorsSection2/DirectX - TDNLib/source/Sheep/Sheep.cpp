#include "IEX_Expansion.h"
#include "../Sound/SoundManager.h"
#include "MousePointer.h"
#include "../CurvePoint/CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "Sheep.h"
#include "../Data/DataMNG.h"
#include "../system/system.h"

Sheep::Base::Base(const SheepData &data, int floor, const SheepTextParam &textparam) :
animepos(0, 0),
floor(floor),
process(GETOUT),
m_AnimePanel(0), m_bErase(false)
{
	Move[0] = &Sheep::Base::Get_out;
	Move[1] = &Sheep::Base::Walk;
	Move[2] = &Sheep::Base::Curve;
	Move[3] = &Sheep::Base::Caught;
	Move[4] = &Sheep::Base::Crushed;
	Move[5] = &Sheep::Base::Ran_over;

	alpha = 0xffffffff;
	frame = 0, animeframe = 0;
	//move = 5;/* speed */

	this->ase.animex = 0;

	memcpy_s(&m_data, sizeof(SheepData), &data, sizeof(SheepData));
	m_data.Image = data.Image;

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
	animeframe++;
	if (animeframe > m_data.Animkankaku)
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
	animeframe++;
	if (animeframe > m_data.Animkankaku)
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
			floor = i;
			break;
		}
	}
}

void Sheep::Base::Caught()
{
	if (animeframe < 7)
	{
		if (frame > 1)
			animeframe++, frame = 0;
		frame++;
	}
	else
	{
		alpha -= 0x0f000000;
		if (alpha <= 0x00ffffff)
			m_bErase = true;
	}
}
void Sheep::Base::Crushed()
{
	if (animeframe < 11)
	{
		if (frame > 2)
			animeframe++, frame = 0;
		frame++;
	}
	else
	{
		m_bErase = true;;
	}
}
void Sheep::Base::Ran_over()
{
	if (animeframe < 9)
	{
		if (frame > 1)
			animeframe++, frame = 0;

		if (animeframe > 3)
		{
			alpha -= 0x0f000000;
		}
		frame++;
	}
	else
		m_bErase = true;
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
	Get_frame_pos(animepos, m_AnimePanel);

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

	m_data.Image->SetARGB(alpha);
	m_data.Image->Render((int)pos.x, (int)pos.y, m_data.SIZE, m_data.SIZE,
		(int)animepos.x, (int)animepos.y, (move.x >= 0) ? -m_data.SIZE : m_data.SIZE, m_data.SIZE,
		RS::COPY);
}

void Sheep::Base::Be_caught(int type)
{
	if (col_check) return;
	switch (type)
	{
	case 0:
		process = CAUGHT;
		//obj = file[CAUGHT];
		break;

	case 1:
		Be_crushed();
		break;

	case 2:
		Be_ran_over();
		break;

	default:
		break;
	}
	animeframe = frame = 0;
}
void Sheep::Base::Be_crushed()
{
	process = CRUSHED; 
	//obj = file[CRUSHED];
}
void Sheep::Base::Be_ran_over()
{ 
	process = RAN_OVER; 
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
	se->Stop("����߂��r����", m_seID);
}
void Sheep::Gold::Update()
{
	// ���N���X�̃A�b�v�f�[�g
	Sheep::Base::Update();

	// ���̍��W�ݒ�
	se->SetPos("����߂��r����", m_seID, pos);
}


//**************************************************
//	�q���H���đ������r
//**************************************************
FatSheep::FatSheep(tdn2DObj *image, const Vector2 &pos) :m_image(image), m_pos(pos), m_angle(0), m_bErase(false), m_moveX(0), m_ReceiveSE(TDNSOUND_PLAY_NONE)
{

}

FatSheep::~FatSheep()
{
	// ���[�v���Ă�SE���~�߂�
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("�������r����", m_ReceiveSE);
}

void FatSheep::Update()
{
	// SE�炵�ĂȂ��Ƃ��̏��
	if (m_ReceiveSE == TDNSOUND_PLAY_NONE)
	{
		// �����Ă���
		if (m_moveX != 0) m_ReceiveSE = se->Play("�������r����", m_pos, Vector2(0, 0), true);	// SE�Đ�(�f�X�g���N�^�Ŏ~�߂�)
	}
	else
	{
		// SE�̍��W��ݒ�
		se->SetPos("�������r����", m_ReceiveSE, m_pos);
	}
	// �ړ��l�𑫂��Ă���
	m_pos.x += m_moveX;
	// �ړ��l�ɉ����ėr����]������
	m_angle -= m_moveX * 0.01f;
}

void FatSheep::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, 0, 0, 240, 240);
}


//**************************************************

// manager

//**************************************************

SheepManager::SheepManager() :
m_ChangeLaneTime(0), sp(0)
{
	m_pFatSheepImage = new tdn2DObj("DATA/CHR/fat_sheep.png");

	m_CreateFloor = 0;	// �����r�����t���A
	// ���̃����_���t���A����
	m_NextChangeFloor = MakeNextFloor(m_CreateFloor);

	// �e�L�X�g����p�����[�^�ǂݍ���
	std::fstream ifs("DATA/Text/Param/sheep.txt");

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	// �r���[�����ς�鎞�ԓǂݍ���
	ifs >> skip;
	ifs >> CHANGE_LANE_TIME;

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
	}
	delete m_pFatSheepImage;

	for (auto it : m_List) delete it;
	m_List.clear();
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
			m_ChangeLaneTime = 0;
			m_CreateFloor = 0;
		}
		else if (KeyBoardTRG(KB_SLASH))
		{
			m_ChangeLaneTime = 0;
			m_CreateFloor = 1;
		}
		else if (KeyBoardTRG(KB_UNDER_BAR))
		{
			m_ChangeLaneTime = 0;
			m_CreateFloor = 2;
		}
	}

	// ���[���ύX����
	if (++m_ChangeLaneTime > CHANGE_LANE_TIME)
	{
		// ���ԃ��Z�b�g
		m_ChangeLaneTime = 0;

		// ���[����ς���
		m_CreateFloor = m_NextChangeFloor;

		// ���̃����_���t���A����
		m_NextChangeFloor = MakeNextFloor(m_CreateFloor);
	}
	else if (m_ChangeLaneTime == CHANGE_LANE_TIME / 4)
	{
		// ��UI�}�l�[�W���Ƀ|�b�v�A�b�v�����肢����
	}

	// �O��̎��Ԃƍ��̎��Ԃ̍���
	UINT delta = clock() - m_CurrentTime;
	bool bCreate(false);

	// �O�񂩂�̌o�ߎ��ԂƏo���Ԋu�ɉ����āA�r�𐶐�
	for (int j = delta; j > m_TextParam.AppTime; j -= m_TextParam.AppTime)
	{
		create(m_CreateFloor);
		bCreate = true;
	}

	// ���ݎ����ݒ�
	if (bCreate)m_CurrentTime = clock();

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
	FatSheep *set = new FatSheep(m_pFatSheepImage, *sheep->Get_pos() + Vector2(-64, -64));// �������r����
	set->SetFloor(sheep->Get_floor());	// �t���A�ݒ�
	m_FatList.push_back(set);			// ���X�g�ɓ˂�����
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