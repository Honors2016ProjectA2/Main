#pragma once

//-------- include ---------
#include "IEX_Expansion.h"
#include "../CurvePoint/CurvePoint.h"

#if(0)// shutter type (true: �A�j���[�V����, false: �ꖇ�G)
#define _SPIC2015_SHUTTER_TYPE_ANIMATION_
#endif

//--------- class definition ----------
class tdn2DObj;
class DataManager;
class MousePointer;
class EnemyManager;

// ���̃��W�I�{�^��
//struct CurvePointRadio
//{
//	CurvePoint *p[2];	// ����
//	bool bCheck[2];		// �`�F�b�N������
//
//	CurvePointRadio(){ FOR(2){ p[i] = nullptr; bCheck[i] = false; } }
//	~CurvePointRadio(){ FOR(2)SAFE_DELETE(p[i]); }
//	void Update(){ FOR(2)p[i]->Update(); }
//	void Render(){ FOR(2)p[i]->Render(); }
//};

//-------- Stage class ------
class Stage
{
	//------ field ---------
private:
	tdn2DObj* image;
	Vector2 pos;
	const int W;
	const int H;
	const int START_Y;
	const int SPEED_Y;
	const int SHUTTER_X;
	const int SHUTTER_Y;
	int offsetY;
	int m_RecastTime;	// ����̃��L���X�g
	tdn2DAnim *m_pHouseRipple;
public:
	enum StageState { NONE, FALL };
	StageState state;

	bool m_bPoint;		// �}�E�X�|�C���g���Ă邩�B
	int m_TenmetsuFrame;	// �������܂����_��
	bool m_bTenmtsuUp;
	const int TENMETSU;

	//------- method --------
public:
	Stage();
	~Stage();
	void Init(Vector2 pos, StageState startState);
	void Update();
	void Render();
	int GetWidth();
	int GetRecastTime(){ return m_RecastTime; }
	void SetRecastTime(int val){ m_RecastTime = val; }
};

//-------- StageManager class ---------
class StageManager
{
	//---- field ------
public:
	//int floor;         // �o�����̊K�w��-1
private:
	// �X�e�[�W�̉摜�n
	enum StageImage
	{
		BACK,			// �X�e�[�W�̊�{�w�i
		HOUSE_BACK,		// �X�e�[�W�̉Ƃ̌��̕���

		// �`�����ɃQ�[���L�����B�`

		HOUSE_FRONT,	// �X�e�[�W�̉Ƃ̑O�̕���
		DOOR_OPEN1,
		DOOR_OPEN2,
		DOOR_OPEN3,
		DOOR_CLOSE,

		SAKU,			// ��

		IKENIE,			// �����ɂ��_
		FIRE,			// ��
		KIETA_FIRE,		// ��������

		MAX
	};


	tdn2DObj *m_pStageImages[StageImage::MAX];
	int m_FireAnimFrame, m_FireAnimPanel;

	tdn2DObj *m_pDogImage, *m_pFireImage;

	enum ImageSrc { STAGE, SHUTTER };

	// �X�R�A���[���ύX
	int m_CHANGE_SCORE_TIME;				// ���[�����ς�鎞�Ԃ̌Œ�l
	int m_ChangeScoreTime;				// ���[�����ς�鎞��
	void ChangeScoreLane();

	int m_RECAST_TIME;	// �����̃��L���X�g�^�C��
	int APPEND_STAGE_BORDER[STAGE_MAX]; // stage�ǉ��^�C�~���O�̃X�R�A
	int m_AddScore[STAGE_MAX];			// ���[���S�[�������Ƃ��ɉ��Z�����X�R�A
	Stage* stage[STAGE_MAX];

	std::vector<CurvePoint::Dog*> m_Doglists[STAGE_MAX];
	//std::vector<CurvePoint::Fire*> m_Firelists[STAGE_MAX];

	// �����[��
	void SetDogFloor(int floor);

	int m_DogStock;		// ���̃X�g�b�N

	// ���X�V
	void UpdateDogs(const Vector2 &pos);

	//------ method --------
public:
	StageManager();
	~StageManager();
	void Init();
	void Reset();
	void Update();

	// �`��
	void Render();
	void RenderBack();	// �w�i�Ƃ��`��
	void RenderFront();	// ���Ƃ���Ƃ��`��
	void RenderFront2();	// ����ɑO

	void CurvePointLock(int floorIdx);

	///<summary>�e��f�[�^��荞��</summary>
	void Reflection(DataManager* data, MousePointer* mouse);
	///<summary>�V���b�^�[�����S�ɊJ���Ă��邩</summary>
	///<param name="floorIdx">���肷��t���A�̓Y��</param>
	bool IsOpen(int floorIdx);

	std::vector<CurvePoint::Dog*> *GetDogList(int floor){ return &m_Doglists[floor]; }
	//std::vector<CurvePoint::Fire*> *GetFireList(int floor){ return &m_Firelists[floor]; }
	CurvePoint::Dog *GetDog(int floor, UINT id){ for (UINT i = 0; i < m_Doglists[floor].size(); i++){ if (m_Doglists[floor][i]->GetID() == id)return m_Doglists[floor][i]; } return nullptr; }
	//CurvePoint::Fire *GetFire(int floor, UINT id){ for (UINT i = 0; i < m_Firelists[floor].size(); i++){ if (m_Firelists[floor][i]->GetID() == id)return m_Firelists[floor][i]; } return nullptr; }

	Vector2 GetArrowPos(int floorIdx);
	Vector2 GetWarningPos(int floorIdx, bool IsRight);
	Vector2 GetPopupPos(int floorIdx, bool IsRight);
	Vector2 GetBalloonPos(int floorIdx);
};

// �ŒZ�t���A�����֐�
int FindFloor(float posY);

extern int STAGE_POS_Y[3];
extern int LANE_WIDTH;
extern Vector2 YAKINIKU_AREA;
extern bool g_bDogSetFrame;