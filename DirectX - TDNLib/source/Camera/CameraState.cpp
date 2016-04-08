#include "TDNLIB.h"

#include "Camera.h"
#include "CameraState.h"
#include "../AI/Message/Message.h"
#include "../AI/Entity/BaseGameEntity.h"
#include "../AI/Entity/EntityManager.h"
#include "../AI/State/state.h"
#include "../AI/State/StateMachine.h"
#include "../AI/Message/MessageDispatcher.h"


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
void CameraGlobalState::Enter(CameraManager *pCamera)
{

}

// ���s��
void CameraGlobalState::Execute(CameraManager *pCamera)
{

}

// �o��
void CameraGlobalState::Exit(CameraManager *pCamera)
{

}

void CameraGlobalState::Render(CameraManager * pCamera)
{
}

//
bool CameraGlobalState::OnMessage(CameraManager *pCamera, const Message &msg)
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
void FixCameraState::Enter(CameraManager *pCamera)
{
	/* �e�L�X�g����f�[�^�ǂݍ��� */
	std::ifstream ifs("DATA/Text/Camera/Fix.txt");
	assert(ifs);	// �t�@�C�������ĂȂ��I

	char skip[32];	// �ǂݔ�΂��p

	ifs >> skip;

	// ���W�ǂݍ���
	Vector3 pos;
	ifs >> pos.x, ifs >> pos.y, ifs >> pos.z;

	// �����_�ǂݍ���
	Vector3 target;
	ifs >> target.x, ifs >> target.y, ifs >> target.z;

	// �{�̂ɃZ�b�g
	pCamera->SetPos(pos);
	pCamera->SetTarget(target);
}

// ���s��
void FixCameraState::Execute(CameraManager *pCamera)
{
	// �Œ�J�����Ȃ̂Ŋ�{�I�ɉ������Ȃ�
}

// �o��
void FixCameraState::Exit(CameraManager *pCamera)
{

}

void FixCameraState::Render(CameraManager * pCamera)
{
}

bool FixCameraState::OnMessage(CameraManager *pCamera, const Message &msg)
{
	// �o�Ă����I
	return false;
}


//*****************************************************************************************************************************
//			�C���g���J����
//*****************************************************************************************************************************
IntroCameraState* IntroCameraState::GetInstance()
{
	// �����ɕϐ������
	static IntroCameraState instance;
	return &instance;
}

// �����
void IntroCameraState::Enter(CameraManager *pCamera)
{
	// ���������Ȃ��ł�����
	m_bAction = false;

	// �X�e�b�v������
	m_step = 0;

	// �^�C�}�[������
	m_timer = 0;

	// ��Ԃ̗͂�������
	m_SlerpPow = .1f;

	// �e�L�X�g����ŏ��̍��W���Ƃ��Ă���
	std::ifstream ifs("DATA/Text/Camera/Intro.txt");

	char skip[32];	// �ǂݔ�΂��p�ϐ�

	ifs >> skip;
	ifs >> skip;
	ifs >> skip;
	ifs >> skip;

	// ���W�ǂݍ���
	ifs >> m_StartPos.x, ifs >> m_StartPos.y, ifs >> m_StartPos.z;

	// �����_�ǂݍ���
	Vector3 target;
	ifs >> m_StartTarget.x, ifs >> m_StartTarget.y, ifs >> m_StartTarget.z;

	// �ڕW���W��ݒ�
	m_NextPos = m_StartPos, m_NextTarget = m_StartTarget;
}

// ���s��
void IntroCameraState::Execute(CameraManager *pCamera)
{
	// ���N���b�N�ŃC���g���X�L�b�v
	if (tdnMouse::GetLeft() == 3)
	{
		// �ʏ�̌Œ�J�����ɐ؂�ւ�(���̓C���g���̃X�e�[�g����Ȃ���Ƃ����̂�sceneMainState�ɑ΂��郁�b�Z�[�W)
		pCamera->GetFSM()->ChangeState(FixCameraState::GetInstance());
	}

	// �܂��d�����ߗ��Ă��Ȃ�����o�Ă������I
	if (!m_bAction) return;

	// �J�����̏��Ƃ��Ă���
	Vector3 cPos = pCamera->GetPos();
	Vector3 cTarget = pCamera->GetTarget();

	// ��ԏ���
	cPos = cPos * (1 - m_SlerpPow) + m_NextPos * m_SlerpPow;
	cTarget = cTarget * (1 - m_SlerpPow) + m_NextTarget * m_SlerpPow;


	switch (m_step)
	{
	case 0:
		/* ��莞�ԃX�e�[�W�������낷�����̎d�� */
		if (++m_timer > 120)		// ���ԉ߂�����
		{
			// ���̃X�e�b�v�ցI
			NextStep();

			// ���̖ڕW���W��ݒ�(�\���X�^�[�g������l)
			m_NextTarget = m_StartPersonPos + Vector3(0, 10, 0);
			m_NextPos = m_StartPersonPos + Vector3(0, 30, -30);
		}
		break;
	case 1:
		/* ��莞�Ԃ��킳�𗬂��l�����Ă邾���̎d�� */
		if (++m_timer > 120)
		{
			// ���̃X�e�b�v�ցI
			NextStep();

			// ���ʂ̂Ȃ�TDN�g����o���Ă��炤�I


		}
		break;
	case 2:
		/* ��莞��TDN�g������Ă邾���̎d�� */
		if (++m_timer > 180)
		{
			// ���̃X�e�b�v�ցI
			NextStep();

			// ���̖ڕW���W��ݒ�(�ŏ��̈ʒu)
			m_NextPos = m_StartPos, m_NextTarget = m_StartTarget;
		}
		break;
	case 3:
		/* �X�^�[�g�n�_�ɋA�邾���̎d�� */
		if (++m_timer > 120)
		{
			// �J�����̎d���͏I���I
			//MsgMgr->Dispatch(
			//	MSG_NO_DELAY,
			//	ENTITY_ID::CAMERA_MNG,
			//	ENTITY_ID::,
			//	MESSAGE_TYPE::END_INTRO_CAMRA,
			//	nullptr	// �ǋL�Ȃ�
			//	);

			// �ʏ�̌Œ�J�����ɐ؂�ւ�(���̓C���g���̃X�e�[�g����Ȃ���Ƃ����̂�sceneMainState�ɑ΂��郁�b�Z�[�W)
			pCamera->GetFSM()->ChangeState(FixCameraState::GetInstance());
		}
		break;
	}

	// �J�����̏��X�V
	pCamera->SetPos(cPos);
	pCamera->SetTarget(cTarget);
}

// �o��
void IntroCameraState::Exit(CameraManager *pCamera)
{

}

void IntroCameraState::Render(CameraManager * pCamera)
{
}

bool IntroCameraState::OnMessage(CameraManager *pCamera, const Message &msg)
{
	if (msg.Msg == MESSAGE_TYPE::START_INTRO_CAMERA)
	{
		// ���߂������̂œ����܂��I
		m_bAction = true;

		// �I���W�i���\���̂̃|�C���^�\�^�Ŏ󂯎�邺�I
		INTRO_CAMERA_INFO* exInfo = (INTRO_CAMERA_INFO*)msg.ExtraInfo;

		// �\���X�^�[�g������l�̍��W���󂯎����
		m_StartPersonPos = exInfo->start_person_pos;
		return true;
	}

	else
	{
		MyAssert(0,"��O: �Ӑ}���Ȃ����b�Z�[�W�����M����܂����@by�C���g���J����")
		return false;
	}
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
void EffectCameraState::Enter(CameraManager *pCamera)
{
	// �X�N���v�gIN
	textLoader::Open("DATA/Text/Camera/Effect.txt");
	time_line_data = nullptr;
}

// ���s��
void EffectCameraState::Execute(CameraManager *pCamera)
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
void EffectCameraState::Exit(CameraManager *pCamera)
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

void EffectCameraState::Render(CameraManager * pCamera)
{
}

bool EffectCameraState::OnMessage(CameraManager *pCamera, const Message &msg)
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