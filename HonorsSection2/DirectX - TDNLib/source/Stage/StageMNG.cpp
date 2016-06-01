//------------ include --------------
#include "StageMNG.h"
#include "TDNLIB.h"
#include "Data/DataMNG.h"
#include "MousePointer.h"
#include "../Collision/Collision.h"
#include "../Enemy/watchman.h"
#include "../Sound/SoundManager.h"
#include "../Niku/Niku.h"
#include "Effect\EffectManager.h"

//int g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MAX];			// ���[�h���ς���Ă�������

//**************************************************
//    StageManager class
//**************************************************

//========================================
// �O���[�o���̈�
int STAGE_POS_Y[3] =
{
	0, 190, 520// ����������������
};
int LANE_WIDTH = 0;
Vector2 YAKINIKU_AREA(0, 0);

int FindFloor(float posY)
{
	// �ŒZ�t���A����
	float dist(640);
	int floor(0);
	for (int i = 0; i < STAGE_MAX; i++)
	{
		float vy = posY - STAGE_POS_Y[i];
		vy = (vy < 0) ? vy * -1 : vy;	// ��Βl

		if (vy < dist)
		{
			floor = i;
			dist = vy;
		}
	}
	return floor;
}

//-------- static field ---------

//------- constructor,destructor ---------

StageManager::StageManager() :m_pDogImage(new tdn2DObj("DATA/CHR/dog.png")), m_pFireImage(new tdn2DObj("DATA/����/���̎�.png")), m_FireSelect(false), m_FireAnimFrame(0), m_FireAnimPanel(0)
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		stage[i] = nullptr;
	}

	// image load

	// �e�L�X�g������擾
	std::ifstream ifs("DATA/Text/Param/stage.txt");

	char skip[64];	// �ǂݔ�΂��p

	// �X�e�[�W�摜�ǂݍ���
	ifs >> skip;
	FOR(StageImage::MAX)
	{
		char path[MAX_PATH];
		ifs >> path;
		m_pStageImages[i] = new tdn2DObj(path);
	}

	// �����ɂ����W�ǂݍ���
	ifs >> skip;
	ifs >> YAKINIKU_AREA.x;
	ifs >> YAKINIKU_AREA.y;

	// ���[�����ǂݍ���
	ifs >> skip;
	ifs >> LANE_WIDTH;

	ifs >> skip;
	FOR(3)
	{
		static const int OFFSET = 120;
		STAGE_POS_Y[i] = OFFSET + LANE_WIDTH * i;

		// ���[������r��
		ifs >> APPEND_STAGE_BORDER[i];
	}

	ifs >> skip;

	FOR(3)
	{
		// ���[���X�R�A��
		ifs >> m_AddScore[i];
	}

	//// �΂̕ω����ԓǂݍ���
	//ifs >> skip;
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MAKI];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOEHAJIME];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOETEKITA];
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::MOETA];
	//g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::ENABLE];				// ���S�R�ă��[�h
	//ifs >> g_FireModeChangeTime[(int)CurvePoint::FIRE_MODE::HITED];

	m_FireStock = 1;

	// ���X�g�b�N���ǂݍ���
	ifs >> skip;
	ifs >> m_DogStock;


	// ���ݒu�|�C���g�ǂݍ���
	FOR(STAGE_MAX)
	{
		m_Doglists[i].clear();
		//m_Firelists[i].clear();
	}
	ifs >> skip;

	int id[STAGE_MAX] = {0};
	while (!ifs.eof())
	{
		int posX, floor;
		ifs >> floor;
		ifs >> posX;

		char cDir[5];
		DIR dir;

		ifs >> cDir;

		if (strcmp(cDir, "UP") == 0) dir = DIR::UP;
		else if (strcmp(cDir, "DOWN") == 0) dir = DIR::DOWN;
		else assert(0);	// ��O����

		m_Doglists[floor].push_back(new CurvePoint::Dog(this, m_pDogImage, Vector2((float)posX, (float)STAGE_POS_Y[floor] + 20), dir, id[floor], floor));
		//if(id[floor] == 1)m_Firelists[floor].push_back(new CurvePoint::Fire(this, m_pFireImage, Vector2((float)posX, (float)STAGE_POS_Y[floor] + 20), dir, id[floor], floor));
		id[floor]++;
	}

	// �{���̓X�R�A�Ń��[����ǉ����Ă������A����͍ŏ�����3���[��
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)
			stage[i] = new Stage;
		stage[i]->Init(Vector2((float)0, (float)STAGE_POS_Y[i]), Stage::StageState::FALL);
	}
}

StageManager::~StageManager()
{
	FOR(StageImage::MAX)delete m_pStageImages[i];
	delete m_pDogImage;
	delete m_pFireImage;

	FOR(STAGE_MAX)
	{
		SAFE_DELETE(stage[i]);

		// �����X�g�̊J��
		for (auto it : m_Doglists[i]) delete it;

		// �����X�g�̊J��
		//for (auto it : m_Firelists[i]) delete it;
	}
}

//--------- public method -----------

void StageManager::Init()
{
	// image delete
	//for (int i = 0; i < STAGE_MAX; i++)
	//{
	//	SAFE_DELETE(stageImage[i]);
	//}
	//
	//// image load
	//stageImage[0]   = new tdn2DObj("DATA/Stage/Game Stage01.png");
	//stageImage[1]   = new tdn2DObj("DATA/Stage/Game Stage02.png");
	//stageImage[2]   = new tdn2DObj("DATA/Stage/Game Stage03.png");
	Reset();
}

void StageManager::Reset()
{
	// stage all clear
	//for (int i = 0; i < STAGE_MAX; i++)
	//{
	//	SAFE_DELETE(stage[i]);
	//}

	// stage set
	//floor = 0;
	//stage[floor] = new Stage;
	//stage[floor]->Init(stageImage[floor], Vector2((float)0, (float)STAGE_POS_Y[floor]), Stage::StageState::NONE);
}

void StageManager::Update()
{
	// �z�C�[��
	//if (tdnMouse::GetWheelFlag() != WHEEL_FLAG::NONE) m_FireSelect = !m_FireSelect;	// ���]

	// �}�E�X���W
	Vector2 mPos = tdnMouse::GetPos();

	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)continue;
		stage[i]->Update();

		// �����X�g�X�V
		for (auto it : m_Doglists[i])
		{
			it->Update();
			//bool bFireOpen(false);
			//if (it->GetID() == 1)bFireOpen = m_Firelists[i][0]->IsOpening();

			// ���I��łȂ�������
			//if (!bFireOpen)
			{
				// �}�E�X�̒��ɓ����Ă邩�ǂ����̔���
				if (mPos.x >= it->GetPos().x && mPos.x <= it->GetPos().x + it->GetWidth() &&
					mPos.y >= it->GetPos().y && mPos.y <= it->GetPos().y + it->GetWidth())
				{
					// ����������t���[��
					if (!it->m_bCursorIn)
					{
						it->m_bCursorIn = true;
					}
				}
				else it->m_bCursorIn = false;

				// ���N���b�N�I�I
				if (tdnMouse::GetLeft() == 3 && it->m_bCursorIn)
				{
					// �����
					if (it->IsOpening())
					{
						se->Play("��", it->GetPos());
						m_DogStock++;	// �X�g�b�N��
						it->Change();	// ����ON_OFF
						return;
					}

					// ���ݒu
					else
					{
						// �X�g�b�N�c���Ă���
						if (m_DogStock > 0 && !m_FireSelect)
						{
							EffectMgr.AddEffect((int)it->GetPos().x+64, (int)it->GetPos().y+64,EFFECT_TYPE::DOG_EFFECT);
							se->Play("��", it->GetPos());
							m_DogStock--;
							it->Change();	// ����ON_OFF
						}
					}

				}
			}
			//else it->m_bCursorIn = false;

		}	// �����X�g

		// �΃��X�g�X�V
		//for (auto it : m_Firelists[i])
		//{
		//	it->Update();
		//
		//	// ���I��ł���
		//	if (m_FireSelect && !m_Doglists[i][it->GetID()]->IsOpening())
		//	{
		//		// �}�E�X�̒��ɓ����Ă邩�ǂ����̔���
		//		if (mPos.x >= it->GetPos().x && mPos.x <= it->GetPos().x + it->GetWidth() &&
		//			mPos.y >= it->GetPos().y && mPos.y <= it->GetPos().y + it->GetWidth())
		//		{
		//			// ����������t���[��
		//			if (!it->m_bCursorIn)
		//			{
		//				it->m_bCursorIn = true;
		//			}
		//		}
		//		else it->m_bCursorIn = false;
		//
		//		// ���N���b�N�I�I
		//		if (tdnMouse::GetLeft() == 3 && it->m_bCursorIn)
		//		{
		//			// ���ݒu
		//			if (!it->IsOpening() && m_FireStock > 0)
		//			{
		//		
		//				se->Play("�܂�", it->GetPos());		// SE�̍Đ�
		//				m_FireStock--;	// �X�g�b�N����
		//
		//				it->Change();	// ON
		//			}
		//		}
		//	}
		//	else it->m_bCursorIn = false;
		//
		//}	// �����X�g

	}	// ���[��for

	// �X�e�[�W�̃A�j���[�V����
	if (++m_FireAnimFrame > 4)
	{
		m_FireAnimFrame = 0;
		if (++m_FireAnimPanel > 2)m_FireAnimPanel = 0;
	}
}

void StageManager::RenderBack()
{
	// �w�i�`��
	m_pStageImages[StageImage::BACK]->Render(0,0);

	// �Ƃ̌��`��
	m_pStageImages[StageImage::HOUSE_BACK]->Render(0, 0);

	// ��`��
	m_pStageImages[StageImage::SAKU]->Render(0, 0);

	// �����ɂ��_�`��
	m_pStageImages[StageImage::IKENIE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);
}

void StageManager::Render()
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		//static const int col[] = { 0x40ff0000, 0x4000ff00, 0x400000ff };
		//// �X�e�[�W��
		//tdnPolygon::Rect(0, STAGE_POS_Y[i], 1280, LANE_WIDTH, RS::COPY, col[i]);

		stage[i]->Render();

		// ���Ɖ��̕`��
		for (auto it : m_Doglists[i]) it->Render();
		//for (auto it : m_Firelists[i]) it->Render();

		// ����������Z�����X�R�A
		tdnText::Draw(1200, STAGE_POS_Y[i] + 120, 0xffffffff, "%d", m_AddScore[i]);
	}
}

void StageManager::RenderFront()
{
	// �Ƃ̑O�`��
	m_pStageImages[StageImage::HOUSE_FRONT]->Render(0, 0);

	// �Ƃ̃h�A�`��

	// ���`��
	m_pStageImages[StageImage::KUSA]->Render(0, 0);

	// ���`��
	m_pStageImages[StageImage::FIRE]->Render((int)YAKINIKU_AREA.x, (int)YAKINIKU_AREA.y, 256, 256, m_FireAnimPanel * 256, 0, 256, 256);
}

void StageManager::Reflection(DataManager* data, MousePointer* mouse)
{
	// shutter check
	for (int i = 0; i < STAGE_MAX; i++)
	{
		if (stage[i] == nullptr)continue;
	}

	// stage apper
	if (data != nullptr)
	{
		data->SetpAddScore(&m_AddScore[0], &m_AddScore[1], &m_AddScore[2]);
	}
}

bool StageManager::IsOpen(int floorIdx)
{
	if (stage[floorIdx] == nullptr)return false;
	return (stage[floorIdx]->state == CurvePoint::Base::CurvePointState::OPEN);
}

Vector2 StageManager::GetArrowPos(int floorIdx)
{
	if (stage[floorIdx] == nullptr)return Vector2(0, 0);
	return Vector2((float)(stage[floorIdx]->GetWidth()/2), (float)(STAGE_POS_Y[floorIdx] + 80));
}

Vector2 StageManager::GetWarningPos(int floorIdx, bool IsRight)
{
	return Vector2((float)(IsRight ? 1200 : 80), (float)(STAGE_POS_Y[floorIdx] + 80));
}

Vector2 StageManager::GetPopupPos(int floorIdx, bool IsRight)
{
	return Vector2((float)(IsRight ? 1200 : 80), (float)(STAGE_POS_Y[floorIdx] + 120));
}

Vector2 StageManager::GetBalloonPos(int floorIdx)
{
	if (stage[floorIdx] == nullptr)return Vector2(0, 0);
	return Vector2((float)(stage[floorIdx]->GetWidth() / 2), (float)(STAGE_POS_Y[floorIdx] + 60));
}

//**************************************************
//    Stage class
//**************************************************

//------- constructor,destructor ---------

Stage::Stage() :
W(1280), H(240),
START_Y(-240), SPEED_Y(32), SHUTTER_X(541), SHUTTER_Y(18)
{
	image = nullptr;
	pos = Vector2(0, 0);
	state = StageState::NONE;
	offsetY = 0;
}

Stage::~Stage()
{
	// �}�l�[�W���[�̂��Q�Ƃ��Ă��Ă邾���Ȃ̂ŁA�폜�̓}�l�[�W���[������
	//for (auto it : m_CPlist) delete it;
	//m_CPlist.clear();
}

//---------- public method ------------

void Stage::Init(Vector2 pos, Stage::StageState state)
{
	this->state = state;
	this->pos = pos;
	switch (state)
	{
		case StageState::NONE:
			offsetY = 0;
			break;
		case StageState::FALL:
			offsetY = (int)pos.y - START_Y;
			break;
	}
}

void Stage::Update()
{
	switch (state)
	{
		case StageState::NONE:
			break;
		case StageState::FALL:
			offsetY -= SPEED_Y;
			if (offsetY <= 0)
			{
				offsetY = 0;
				state = StageState::NONE;
			}
			break;
	}
}

void Stage::Render()
{
	if (image != nullptr)
	{
		image->Render((int)pos.x, (int)pos.y - offsetY);
	}

	//for(auto it : *m_CPlist)it->Render();
}

int Stage::GetWidth()
{
	return W;
}