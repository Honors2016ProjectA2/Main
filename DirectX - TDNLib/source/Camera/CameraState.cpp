#include "TDNLIB.h"

#include "Camera.h"
#include "CameraState.h"
#include "../AI/Message/Message.h"
#include "../AI/Entity/BaseGameEntity.h"
#include "../AI/Entity/EntityManager.h"
#include "../AI/State/state.h"
#include "../AI/State/StateMachine.h"



//*****************************************************************************************************************************
//			�O���[�o���X�e�[�g
//*****************************************************************************************************************************
CameraGlobalState* CameraGlobalState::GetInstance()
{
	// �����ɕϐ������
	static CameraGlobalState instance;
	return &instance;
}

// �����
void CameraGlobalState::Enter(Camera *pCamera)
{

}

// ���s��
void CameraGlobalState::Execute(Camera *pCamera)
{

}

// �o��
void CameraGlobalState::Exit(Camera *pCamera)
{

}

void CameraGlobalState::Render(Camera * pCamera)
{
}

//
bool CameraGlobalState::OnMessage(Camera *pCamera, const Message &msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	////case MESSAGE_TYPE::
	//
	//	break;
	//default:
	//
	//	break;
	//}//end switch

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g�Ȃ̂Ő��^�����̏I��
	return false;
}


//*****************************************************************************************************************************
//			�Œ�J����
//*****************************************************************************************************************************
FixCameraState* FixCameraState::GetInstance()
{
	// �����ɕϐ������
	static FixCameraState instance;
	return &instance;
}

// �����
void FixCameraState::Enter(Camera *pCamera)
{
	std::ifstream ifs("DATA/Camera/Fix.txt");
	assert(ifs);	// �t�@�C�������ĂȂ��I

	char skip[32];	// �ǂݔ�΂��p

	ifs >> skip;

	// ���W�ǂݍ���
	Vector3 pos;
	ifs >> pos.x, ifs >> pos.y, ifs >> pos.z;

	// �����_�ǂݍ���
	Vector3 target;
	ifs >> target.x, ifs >> target.y, ifs >> target.z;
}

// ���s��
void FixCameraState::Execute(Camera *pCamera)
{
	// �Œ�J�����Ȃ̂Ŋ�{�I�ɉ������Ȃ�
}

// �o��
void FixCameraState::Exit(Camera *pCamera)
{

}

void FixCameraState::Render(Camera * pCamera)
{
}

bool FixCameraState::OnMessage(Camera *pCamera, const Message &msg)
{
	// �o�Ă����I
	return false;
}

//*****************************************************************************************************************************
//			�G�t�F�N�g�J����
//*****************************************************************************************************************************
EffectCameraState* EffectCameraState::GetInstance()
{
	// �����ɕϐ������
	static EffectCameraState instance;
	return &instance;
}

// �����
void EffectCameraState::Enter(Camera *pCamera)
{
	// �X�N���v�gIN
	textLoader::Open("DATA/Text/Camera/Effect.txt");
	time_line_data = nullptr;
}

// ���s��
void EffectCameraState::Execute(Camera *pCamera)
{
	data_cursor = -1;

	if (time_line_data)
	{
		for (int i = 0; i < time_line_data->data_count; i++)
		{
			if (current_frame >= time_line_data->data[i].start_frame &&
				current_frame <= time_line_data->data[i].end_frame)
			{
				data_cursor = i;
				Vector3 pos, target;
				GetTimeLineCameraPos(&pos, &target);

				// �J�����̃A���O���ɉ����č��W��]
				pos.z = (pos.z * cosf(pCamera->GetOrgAngle())) - (pos.x * sinf(pCamera->GetOrgAngle()));
				pos.x = (pos.z * sinf(pCamera->GetOrgAngle())) + (pos.x * cosf(pCamera->GetOrgAngle()));

				// �J�������ݒ肵�����_��0�Ƃ���
				pos += pCamera->GetOrgPos();
				target += pCamera->GetOrgPos();

				// �J�����ɏ��Z�b�g
				pCamera->SetPos(pos);
				pCamera->SetTarget(target);
				break;
			}
		}

		// �Đ��I��������
		if (++current_frame > time_line_data->data[time_line_data->data_count - 1].end_frame)
		{
			// �Đ��t���[����0�ɂ���
			current_frame = 0;

			// �X�e�[�g��O�̃X�e�[�g�ɖ߂�
			pCamera->GetFSM()->ChangeState(pCamera->GetFSM()->GetPreviousState());

			// �������
			for (int i = 0; i < time_line_data->data_count; i++)
			{
				delete time_line_data->data[i].pos_array;
				delete time_line_data->data[i].target_array;
			}
			delete time_line_data->data;
			delete time_line_data, time_line_data = nullptr;
		}
	}
}

// �o��
void EffectCameraState::Exit(Camera *pCamera)
{
	if (time_line_data)
	{
		for (int i = 0; i < time_line_data->data_count; i++)
		{
			delete time_line_data->data[i].pos_array;
			delete time_line_data->data[i].target_array;
		}
		delete time_line_data->data;
		delete time_line_data;
	}
}

void EffectCameraState::Render(Camera * pCamera)
{
}

bool EffectCameraState::OnMessage(Camera *pCamera, const Message &msg)
{
	// �o�Ă����I
	return false;
}


void EffectCameraState::GetTimeLineCameraPos(Vector3 *out_pos, Vector3 *out_target)
{
	if (data_cursor == -1) return;
	if (time_line_data->data[data_cursor].fix)
	{
		*out_pos = time_line_data->data[data_cursor].pos_array[0];
		*out_target = time_line_data->data[data_cursor].target_array[0];
	}

	// �x�W�G�v�Z�֐��Ɋۓ���
	Math::Bezier(
		out_pos,																	// �󂯎M
		time_line_data->data[data_cursor].pos_array,								// �n�_�A���ԁA�I�_�������Ă���W�z��
		time_line_data->data[data_cursor].num_elements,								// �̗v�f��
		(float)(current_frame - time_line_data->data[data_cursor].start_frame) / time_line_data->data[data_cursor].frame_len
		);

	Math::Bezier(
		out_target,
		time_line_data->data[data_cursor].target_array,								// �n�_�A���ԁA�I�_�������Ă���W�z��
		time_line_data->data[data_cursor].num_elements,								// �̗v�f��
		(float)(current_frame - time_line_data->data[data_cursor].start_frame) / time_line_data->data[data_cursor].frame_len
		);
}