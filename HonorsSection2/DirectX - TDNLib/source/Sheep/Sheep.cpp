
#include "IEX_Expansion.h"
#include "MousePointer.h"
#include "../CurvePoint/CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "Sheep.h"
#include "../Data/DataMNG.h"
#include "../system/system.h"

Sheep::Sheep(const SheepData &data, tdn2DObj *byu, tdn2DObj *ase, int floor, const SheepTextParam &textparam) :
animepos(0, 0),
floor(floor),
process(GETOUT),
m_AnimePanel(0)
{
	Move[0] = &Sheep::Get_out;
	Move[1] = &Sheep::Walk;
	Move[2] = &Sheep::Curve;
	Move[3] = &Sheep::Caught;
	Move[4] = &Sheep::Crushed;
	Move[5] = &Sheep::Ran_over;

	alpha = 0xffffffff;
	frame = 0, animeframe = 0;
	//move = 5;/* speed */

	this->byu.byu = byu;
	this->byu.animex = 0;

	this->ase.ase = ase;
	this->ase.animex = 0;

	memcpy_s(&m_data, sizeof(SheepData), &data, sizeof(SheepData));
	m_data.Image = data.Image;

	col_check = false;

	int addY = tdnRandom::Get(0, textparam.VerticalWidth);	// �㉺������Ƃ��炷�p

	pos = Vector2((float)textparam.startX, 575.0f - (240.0f * floor) - (float)addY);
	C_MOVE.x = (float)textparam.speed;
	C_MOVE.y = 0;
}

Sheep::~Sheep()
{}

//**************************************************

bool Sheep::Get_out()
{
	// Initialize�I�Ȗ����Ȃ̂���
	animeframe++;
	if (animeframe > 6)
	{
		process = WALK;
		//obj = file[WALK];
		animeframe = 0;
	}

	return true;
}
bool Sheep::Walk()
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

	byu.animex += (int)move.x * 3;

	return true;
}

bool Sheep::Curve()
{
	animeframe++;
	if (animeframe > m_data.Animkankaku)
	{
		animeframe = 0;
		ase.animex += 128;
		if (++m_AnimePanel >= m_data.NumPanel)m_AnimePanel = 0;
	}

	const float pow = C_MOVE.Length();	// ���i�����������Ă�ړ��̗�
	const float addAngle = 0.030f;		// �T�C���J�[�u���Ȃ���̂Ɏg��

	// �T�C���J�[�u�I�Ȃ̂ŗr�̌Q����Ȃ���
	move = Vector2(cosf(m_sinAngle) * pow, sinf(m_sinAngle) * pow);
	if (m_CurveDir == DIR::UP) move.y *= -1;
	pos += move;

	// 90�x�Ȃ����Ė߂鎞
	if (m_bTurned)
	{
		m_sinAngle -= addAngle;

		// 90�x�Ȃ���I�������A�Ȃ������t���OON
		if ((m_sinAngle -= addAngle) <= 0) process = MODE::WALK;
	}

	// 90�x�Ȃ���O
	else
	{
		// 90�x�Ȃ���I�������A�Ȃ������t���OON
		if ((m_sinAngle += addAngle) >= PI * 0.5f) m_bTurned = true;
	}
	// �t���A�v�Z
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (pos.y >= STAGE_POS_Y[i])
		{
			floor = i;
			break;
		}
	}

	byu.animex += (int)move.x * 3;

	return true;
}

bool Sheep::Caught()
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
			return false;
	}

	return true;
}
bool Sheep::Crushed()
{
	if (animeframe < 11)
	{
		if (frame > 2)
			animeframe++, frame = 0;
		frame++;
	}
	else
	{
		return false;
	}

	return true;
}
bool Sheep::Ran_over()
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
		return false;

	return true;
}

bool Sheep::Update()
{
	return (this->*Move[process])();
}

void Sheep::Get_frame_pos(Vector2 &pos, int animePanel)
{
	pos.x = (float)(animePanel % 4);
	pos.y = (float)(animePanel / 4);

	pos.x *= m_data.SIZE;
	pos.y *= m_data.SIZE;
}

void Sheep::Render()
{
	Get_frame_pos(animepos, m_AnimePanel);

	if (process == WALK)
	{
		/* �����Ɣ�Ⴕ�ē����ɂ��� */
		//�����āA�������Y��ɓZ�߂鏈���l����̖ʓ|�������񂾁B�B�B�����R�[�h is �т�[�Ă��ӂ�
		if( move.x > 0 ){
			shader2D->SetValue("moveAngle", .0f);
			anim += 0.005f;
			if( anim > 1.0f ) anim -= 1.0f;
		}else{
			shader2D->SetValue("moveAngle", 0.125f);
			anim -= 0.005f;
			if( anim < 0.0f ) anim += 1.0f;
		}
		shader2D->SetValue("animMove", anim);
		//byu.byu->Render((int)(pos.x + ((move.x > 0) ? -64 : 64)), (int)pos.y, SIZE, SIZE, 0, 0, 128, 128, shader2D, "byun");
//		byu.byu->Render((int)(pos.x + ((move > 0) ? -64 : 64)), (int)pos.y, SIZE, SIZE, byu.animex, 0, 128, 128);
		//ase.ase->Render((int)(pos.x + ((move.x > 0) ? -32 : 32)), (int)pos.y, SIZE, SIZE, ase.animex, 0, (move.x > 0) ? -128 : 128, 512);
	}

	m_data.Image->SetARGB(alpha);
	m_data.Image->Render((int)pos.x, (int)pos.y, m_data.SIZE, m_data.SIZE,
		(int)animepos.x, (int)animepos.y, (move.x >= 0) ? -m_data.SIZE : m_data.SIZE, m_data.SIZE,
		RS::COPY);
}

void Sheep::Be_caught(int type)
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
void Sheep::Be_crushed()
{
	process = CRUSHED; 
	//obj = file[CRUSHED];
}
void Sheep::Be_ran_over()
{ 
	process = RAN_OVER; 
	//obj = file[RAN_OVER];
}

//**************************************************

// manager

//**************************************************

SheepManager::SheepManager() :
mp(0),
sp(0)
{
	byu = new tdn2DObj("DATA/byu-n.png");
	ase = new tdn2DObj("DATA/ase.png");

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

	// �r���ǂݍ���
	for (int i = 0; i < (int)SHEEP_TYPE::MAX; i++)
	{
		ifs >> skip;
		
		// �r�摜�ǂݍ���
		ifs >> skip;
		char path[MAX_PATH];
		ifs >> path;
		m_TextParam.data[i].Image = new tdn2DObj(path);

		// �X�R�A�ǂݍ���
		ifs >> skip;
		ifs >> m_TextParam.data[i].score;

		// �R�}�̃T�C�Y
		ifs >> skip;
		ifs >> m_TextParam.data[i].SIZE;

		// �A�j������
		ifs >> skip;
		ifs >> m_TextParam.data[i].NumPanel;

		// �R�}�i�߂�t���[��
		ifs >> skip;
		ifs >> m_TextParam.data[i].Animkankaku;
	}
	m_List.clear();
	// �^�C�}�[�ݒ�
	m_CurrentTime = 0;
}

SheepManager::~SheepManager()
{
	for (int i = 0; i < (int)SHEEP_TYPE::MAX; i++)
	{
		SAFE_DELETE(m_TextParam.data[i].Image);
	}
	SAFE_DELETE(byu);
	SAFE_DELETE(ase);

	for (auto it : m_List) delete it;
	m_List.clear();
}

//**************************************************

void SheepManager::create(int floor)
{
	// �����_���ɗr�̃^�C�v
	int r = tdnRandom::Get(0, 2);
	m_List.push_back(new Sheep(m_TextParam.data[r], byu, ase, floor, m_TextParam));
}

void SheepManager::Update()
{
	if (sp)
	{
		// �O��̎��Ԃƍ��̎��Ԃ̍���
		UINT delta = clock() - m_CurrentTime;
		bool bCreate(false);

		for (int i = 0; i <= sp->floor; i++)
		{
			//if (sp->IsOpen(i))
			{
				// �O�񂩂�̌o�ߎ��ԂƏo���Ԋu�ɉ����āA�r�𐶐�
				for (int j = delta; j > m_TextParam.AppTime; j -= m_TextParam.AppTime)
				{
					create(i);
					bCreate = true;
				}
			}
		}

		// ���ݎ����ݒ�
		if(bCreate)m_CurrentTime = clock();
	}
	else m_CurrentTime = clock();

	for (auto it = m_List.begin(); it != m_List.end(); )
	{
		if (!(*it)->Update() || (*it)->Get_pos()->x < -2048 || (*it)->Get_pos()->x > 2048)
		{
			delete (*it);
			it = m_List.erase(it);
		}
		else it++;
	}
}

//**************************************************

void SheepManager::Render()
{
	for (auto it : m_List)
	{
		it->Render();
	}
}
