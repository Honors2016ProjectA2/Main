#pragma once

#include	"system/BaseScene.h"
#include	"IEX_Expansion.h"
class Ready;
class MousePointer;
class StageManager;
class DataManager;

class sceneTutorial	: public BaseScene
{
public:

	//�������E���
	bool Initialize();
	~sceneTutorial();
	//����
	bool Update();
	//�`��
	void Render();
private:
	tdn2DObj* back = nullptr;
	Ready* ready = nullptr;

	MousePointer* pointer = nullptr;
	StageManager* stage = nullptr;
	DataManager* dataMNG = nullptr;

	Surface* backUp;
	tdn2DObj* renderTarget = nullptr;

	int state = 0;
	int m_TutorialStep;	// �`���[�g���A���̃X�e�b�v
	bool m_bPause;	// �`���[�g���A���E�U�E���[���h
	int m_WaitTimer;	// ��������̌o�ߎ���

	bool isResultFlag;



	//INIT, TITLE, EXPLAIN, READY, MAIN, END, RESULT
	void Init();
	//�X�V����
	void ReadyEvent();
	bool MainUpdate();

	void DataDelivery();	//���󂯓n��

	//�`�揈��
	void ReadyRender();
	void MainRender();

	void DebugText();

public:
	// �|�[�Y���j���[
	struct POSE
	{
		tdn2DAnim* pic;
		int x, y;
		bool isAction;

		tdn2DAnim* rip;
	};
	POSE m_stop;
	POSE m_again;

	struct POSE_TIMER 
	{
		tdn2DAnim* one;
		tdn2DAnim* two;
		tdn2DAnim* three;
	};
	POSE_TIMER m_poseTimer;

	enum class POSE_STATE
	{
		START, EXE, COUNT, TITLE
	};
	POSE_STATE m_poseState;

	struct PoseIcon
	{
		tdn2DObj* pic;
		int x, y;

	};
	PoseIcon m_poseIcon;


	// �ǉ��@����

	enum class BLACK
	{
		DOG, DOG_USE,HOUSE,CENTER,
		POINT,MEET,GRASS, GRASS_GAGE,DRAG,HOLD,END
	};

	tdn2DObj* m_blackflame[(int)BLACK::END];





private:

	void PoseUpdate();
	void PoseRender();
};