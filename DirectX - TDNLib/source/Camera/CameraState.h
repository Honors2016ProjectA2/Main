#pragma once


//*****************************************************************************************************************************
//
//			�J�����X�e�[�g�S��
//
//*****************************************************************************************************************************

// �e�L�X�g�̓ǂݍ��ݗp
#include	"../textLoader\textLoader.h"

// �O���錾
class CameraManager;
class EffectCamera;

//--------------------�O���[�o���X�e�[�g
class CameraGlobalState :public State<CameraManager>
{
public:

	//this is a �V���O���g��
	static CameraGlobalState* GetInstance();

	// ����
	virtual void Enter(CameraManager* pCamera);

	// ���s���܂�
	virtual void Execute(CameraManager* pCamera);

	// �A��
	virtual void Exit(CameraManager* pCamera);

	// �`��
	virtual void Render(CameraManager* pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

private:
	CameraGlobalState() {};
	~CameraGlobalState() {};
	CameraGlobalState(const CameraGlobalState&);
	CameraGlobalState& operator=(const CameraGlobalState&);
};


//--------------------�Œ�J����
class FixCameraState :public State<CameraManager>
{
public:

	//this is a �V���O���g��
	static FixCameraState* GetInstance();

	// ����
	virtual void Enter(CameraManager* pCamera);

	// ���s���܂�
	virtual void Execute(CameraManager* pCamera);

	// �A��
	virtual void Exit(CameraManager* pCamera);

	// �`��
	virtual void Render(CameraManager* pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

private:
	FixCameraState() {};
	~FixCameraState() {};

	FixCameraState(const FixCameraState&);
	FixCameraState& operator=(const FixCameraState&);

};

//--------------------�C���g�����o�p�J����
class IntroCameraState :public State<CameraManager>
{
public:

	//this is a �V���O���g��
	static IntroCameraState* GetInstance();

	// ����
	virtual void Enter(CameraManager* pCamera);

	// ���s���܂�
	virtual void Execute(CameraManager* pCamera);

	// �A��
	virtual void Exit(CameraManager* pCamera);

	// �`��
	virtual void Render(CameraManager* pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

private:
	int m_step;		// ���o�̃X�e�b�v
	int m_timer;	// ���o�ł̎��Ԃ��v�鎞��

	Vector3 m_StartPos, m_StartTarget;	// �J�n���̍��W(�ړ����āA�Ō�Ɍ��̈ʒu�ɖ߂�ۂɕK�v�ɂȂ�)

	Vector3 m_NextPos, m_NextTarget;	// �ڕW�ʒu(��ԂŎg��)
	float m_SlerpPow;					// ��Ԃɂ������(0 �` 1.0f)

	Vector3 m_StartPersonPos;		// �\���X�^�[�g����l�̍��W(���b�Z�[�W����󂯎��)

	bool m_bAction;	// �ŏ���false��true�ɂȂ�����ғ�����

	// �X�e�b�v��i�߂�v�`�֐�
	void NextStep()
	{
		m_step++;
		m_timer = 0;
	}

	IntroCameraState() {};
	~IntroCameraState() {};

	IntroCameraState(const IntroCameraState&);
	IntroCameraState& operator=(const IntroCameraState&);

};

//--------------------�G�t�F�N�g�J�����p�f�[�^
class TimeLineCameraPattern
{
public:
	int no;			// �����̃p�^�[���ԍ�
	int data_count;	// �f�[�^�̎����Ă鐔
	struct Data
	{
		int start_frame;		// �^�C�����C����ł̊J�n�t���[��
		int end_frame;			// �^�C�����C����ł̏I���t���[��
		int frame_len;			// 1�p�^�[��������̃t���[����(�o�[�̒���)
		Vector3 *pos_array;		// ���W(�J�n�ƏI��肾���łȂ��A���ԂƂ������̂ŉϒ�)
		Vector3 *target_array;	// �����_(�������ϒ�)
		int num_elements;		// ��L�ϒ��̗v�f��
		bool fix;
	}*data;
};

//--------------------�G�t�F�N�g�J����(�X�N���v�g)
class EffectCameraState :public State<CameraManager>, textLoader
{
public:

	//this is a �V���O���g��
	static EffectCameraState* GetInstance();

	// ����
	virtual void Enter(CameraManager* pCamera);

	// ���s���܂�
	virtual void Execute(CameraManager* pCamera);

	// �A��
	virtual void Exit(CameraManager* pCamera);

	// �`��
	virtual void Render(CameraManager* pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

private:
	EffectCameraState() {};
	~EffectCameraState() {};

	EffectCameraState(const EffectCamera&);
	EffectCameraState& operator=(const EffectCamera&);

	int pattern_no;
	int data_cursor;
	char effect_path[256];
	DWORD ptr;	// 
	int current_frame;	// ���݂̃t���[����
	TimeLineCameraPattern *time_line_data = new TimeLineCameraPattern;

	void GetTimeLineCameraPos(Vector3 *out_pos, Vector3 *out_target);
};