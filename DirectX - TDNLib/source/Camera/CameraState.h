#pragma once


//*****************************************************************************************************************************
//
//			�J�����X�e�[�g�S��
//
//*****************************************************************************************************************************

// �e�L�X�g�̓ǂݍ��ݗp
#include	"../textLoader\textLoader.h"

// �O���錾
class Camera;
class EffectCamera;

//--------------------�O���[�o���X�e�[�g
class CameraGlobalState:public State<Camera>
{
public:

	//this is a �V���O���g��
	static CameraGlobalState* GetInstance();

	// ����
	virtual void Enter(Camera* pCamera);

	// ���s���܂�
	virtual void Execute(Camera* pCamera);

	// �A��
	virtual void Exit(Camera* pCamera);

	// �`��
	virtual void Render(Camera* pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(Camera* pCamera, const Message& msg);

private:
	CameraGlobalState() {};
	~CameraGlobalState() {};
	CameraGlobalState(const CameraGlobalState&);
	CameraGlobalState& operator=(const CameraGlobalState&);
};


//--------------------�Œ�J����
class FixCameraState :public State<Camera>
{
public:

	//this is a �V���O���g��
	static FixCameraState* GetInstance();

	// ����
	virtual void Enter(Camera* pCamera);

	// ���s���܂�
	virtual void Execute(Camera* pCamera);

	// �A��
	virtual void Exit(Camera* pCamera);

	// �`��
	virtual void Render(Camera* pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(Camera* pCamera, const Message& msg);

private:
	FixCameraState() {};
	~FixCameraState() {};

	FixCameraState(const FixCameraState&);
	FixCameraState& operator=(const FixCameraState&);

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
class EffectCameraState :public State<Camera>,textLoader
{
public:

	//this is a �V���O���g��
	static EffectCameraState* GetInstance();

	// ����
	virtual void Enter(Camera* pCamera);

	// ���s���܂�
	virtual void Execute(Camera* pCamera);

	// �A��
	virtual void Exit(Camera* pCamera);

	// �`��
	virtual void Render(Camera* pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(Camera* pCamera, const Message& msg);

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