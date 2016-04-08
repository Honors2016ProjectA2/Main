
#include "AI\NPC\PersonManager.h"

//�@�ЂƂ̎��
#include "AI\NPC\BasePerson\BasePerson.h"
#include "../../UI/UI.h"
#include "../../Sound/SoundManager.h"

#include "WaitPerson\WaitPerson.h"
#include "StartPerson\StartPerson.h"
#include "GoalPerson\GoalPerson.h"
#include "StopPerson\StopPerson.h"
#include "StrongPerson\StrongPerson.h"
#include "GameOverPerson\GameOverPerson.h"

// �錾
PersonManager* PersonManager::pInstance = nullptr;

int PersonManager::m_IDcount = 0;

/***********/
//	���̎擾
/***********/
PersonManager & PersonManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new PersonManager();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *pInstance;
}

/***********/
//	���
/***********/
void PersonManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// ������
PersonManager::PersonManager():
	BaseGameEntity(ENTITY_ID::PERSON_MNG),// ���l�̃}�l�[�W���[�p�̂h�c�ԍ���n��
	m_NumShedPerson(0),
	m_bJudgeMoment(false),
	m_combo(0),
	m_delay(0)
{
	

}


// ���
PersonManager::~PersonManager()
{
	// �S��������
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		SAFE_DELETE(m_PersonData[i]);
	}

	//�@�f�[�^�����
	m_PersonData.clear();
}

// �L�����ǉ�
void PersonManager::AddPerson(PERSON_TYPE type,Vector3 pos)
{
	// �l
	BasePerson* data;

	ENTITY_ID id = (ENTITY_ID)(GAME_ID::id_person_first + m_IDcount);

	// �����ŃL�����N�^�[�̃f�[�^��ς���
	switch (type)
	{
	case PERSON_TYPE::WAIT:
		data = new WaitPerson(id);
		data->SetPos(pos);

		break;
	case PERSON_TYPE::START:
		data = new StartPerson(id);
		data->SetPos(pos);

		// ���ǉ� �X�^�[�g�̐l�Ԃ�������J�������o�ɕK�v�Ȃ̂ō��W�𑗂��Ă�����I(�����ŃC���g���J�����̃X�C�b�`��ON�ɂȂ�)
		//if (type == PERSON_TYPE::START)
		{
			// �\���̍쐬
			INTRO_CAMERA_INFO ex;
			ex.start_person_pos = pos;	// �\�X�^�[�g�l�̍��W

			// �J�����}�l�[�W���[�ɏ�񑗐M
			MsgMgr->Dispatch(
				MSG_NO_DELAY,
				ENTITY_ID::PERSON_MNG,
				ENTITY_ID::CAMERA_MNG,
				MESSAGE_TYPE::START_INTRO_CAMERA,	// �C���g���J�����ғ��I
				(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
				);
		}

		break;
	case PERSON_TYPE::GOAL:
		data = new GoalPerson(id);
		data->SetPos(pos);

		break;
	case PERSON_TYPE::STOP:
		data = new StopPerson(id);
		data->SetPos(pos);

		break;
	case PERSON_TYPE::STRONG:
		data = new StrongPerson(id);
		data->SetPos(pos);

		break;
	case PERSON_TYPE::GAMEOVER:
		data = new GameOverPerson(id);
		data->SetPos(pos);

		break;
	default:
		MyAssert(0, "����ȓz�͂��Ȃ�");
		break;
	}



	// �z��ɉ�����
	m_PersonData.push_back(data);

	++m_IDcount;// ���̐l�̔ԍ���
}

void PersonManager::Reset()
{
	m_IDcount = 0;

}

void PersonManager::Update()
{
	if (m_PersonData.empty())return;// �󂾂����烊�^�[��

	// �l�����X�V

	// �X�e�[�W3�̂����N���A�ɂȂ��Ă��܂��o�O�𒼂����߂̂��艟���𒴂�������
	//{
	//	if (m_delay > 0)
	//	{
	//		if (--m_delay == 0)
	//		{
	//			bool bShedNow(false);
	//			for (auto it : m_PersonData)
	//			{
	//				if (it->IsShedNow())
	//				{
	//					bShedNow = true;
	//				}
	//			}

	//			if (bShedNow)
	//			{
	//				m_delay = 10;
	//			}
	//			else
	//			{
	//				m_bJudgeMoment = true;
	//				m_combo = 0;
	//			}
	//		}

	//	}
	//	else
	//	{
	//		m_bJudgeMoment = false;
	//	}
	//}

	m_NumShedPerson = 0;

	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		if (m_PersonData[i]->IsShed()) m_NumShedPerson++;
		m_PersonData[i]->Update();
	}
}

void PersonManager::Render()
{
	if (m_PersonData.empty())return;// �󂾂����烊�^�[��


								   // �l�����`��
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		m_PersonData[i]->Render();
	}


}

// ���b�Z�[�W
bool  PersonManager::HandleMessage(const Message& msg)
{
	// �܂�����MGR�ɃX�e�[�g�}�V���͂������ɂ������ȁE�E�E
	// �Ƃ������Ƃł����Œ��Ƀ��b�Z�[�W�ɑ΂��鏈����

	switch (msg.Msg)
	{
	case MESSAGE_TYPE::RIPPLE_VS_PERSON:
	{
		// �I���W�i���\���̂̃|�C���^�\�^�Ŏ󂯎�邺�I

		RIPPLE_INFO* exInfo = (RIPPLE_INFO*)msg.ExtraInfo;
		//float* exInfo = (float*)msg.ExtraInfo;

		// �����łu�r�֐����N���@to be contted 
		RippleVSPerson(exInfo);

		return true;// [��肭���b�Z�[�W���͂���!]
		break;
	}
	default:
		MyAssert(0, "����Ȗ��߂͎󂯕t���ĂȂ��ł�");
		break;
	}


	return false;// [��肭���b�Z�[�W���͂��Ȃ�����]
}

// ���Z�b�g
void PersonManager::ResetState()
{

	// �S���̃X�e�[�g������
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		m_PersonData[i]->SetIsShed(false);	// �\�������t���O=False��
		m_PersonData[i]->ResetState();
	}
}
// �X�^�[�g�̐l�̔g����o���I
void PersonManager::StartGossip()
{
	// �S���̃X�e�[�g������
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		// �S���ɑ��M
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			m_PersonData[i]->GetID(),
			START_GOSSIP,
			MSG_NO_EXINFO
			);
	}
}

/*	(����)��l�ł��n�j�E�g��͈�񂵂��������Ȃ��@*/
// �g�� vs �l
// �����蔻��
void PersonManager::RippleVSPerson(RIPPLE_INFO* pRipData)// ���g��
{
	// �Ώ�
	int count(0);
	//int count2(0);

	// ���l�������Ă��邩
	//for (int b = 0; b < (int)m_PersonData.size(); b++)
	//{
	//	if (m_PersonData[b]->IsShed() == true)continue;// �\�𗧂Ă���͔������Ȃ�
	//	if (m_PersonData[b]->GetPersonType() == pRipData->type) continue;// �����^�C�v�̓o�C�o�C

	//	float ren = Math::Length(pRipData->pos, m_PersonData[b]->GetPos());
	//	if (ren <= pRipData->size)// 30m�ȓ��ɐl�����݂����
	//	{
	//		count2++;// (��)
	//	}
	//}	

	// ���~�߂�l��������g����o���Ȃ�
	for (int b = 0; b < (int)m_PersonData.size(); b++)
	{
		if (m_PersonData[b]->IsShed() == true)continue;// �\�𗧂Ă���͔������Ȃ�

		float ren = Math::Length(pRipData->pos, m_PersonData[b]->GetPos());
		if (ren <= pRipData->size)// 30m�ȓ��ɐl�����݂����
		{
			if (m_PersonData[b]->GetPersonType() == PERSON_TYPE::STOP)
			{
				return;// �~�߂�l��������R���e�B�j���[
			}

		}
	}

	// ��l�ȏ�Ȃ炱�Ȃ�
	//if (count2 >= 2)
	//{

		for (int b = 0; b < (int)m_PersonData.size(); b++)
		{

			if (m_PersonData[b]->IsShed() == true)continue;// �\�𗧂Ă���͔������Ȃ�
			//if (m_PersonData[b]->GetPersonType() == pRipData->type) continue;// �����^�C�v�̓o�C�o�C
		

			float ren =
				Math::Length(pRipData->pos, m_PersonData[b]->GetPos());
			if (ren <= pRipData->size)// 30m�ȓ��ɐl�����݂����
			{
				// ��l�ȏ�Ȃ炱�Ȃ�
				//if (count2 >= 2)
				{
					// �߂��ɂ����l�͔g����΂�
					m_PersonData[b]->ActionGossipRipple();

					// �������̐����o��
					UIMgr.PushHukidashi(m_PersonData[b]->GetPos(), HUKIDASHI_TYPE::SUCCESS);

					// �R���{���𑝂₷(2�l�J�E���g������A2�g�[���グ���܂��̂ł��̑Ώ�)
					if (count == 0) se->SetTone("�g��o��", ++m_combo);

					count++;
				}
				//else
				//{
				//	// ���s���̐����o��
				//	UIMgr.PushHukidashi(m_PersonData[b]->GetPos(), HUKIDASHI_TYPE::FAILED);
				//}
			}

		}

	//}

	if (count == 0)
	{
		// ���肵�Ă܂���[�I
		//m_bJudgeMoment = true;
		m_delay = 10;
	}
}

BasePerson * PersonManager::GetPerson(int no)
{
	if (m_PersonData.empty())
	{
		MyAssert(0, "�f�[�^����");
	}

	// �G���[����
	if (no >= (int)m_PersonData.size())
	{
		MyAssert(0, "�w�肵���ꏊ�Ƀf�[�^���Ȃ�");
	}

	return  m_PersonData[no];

}

void PersonManager::ProductRipple()
{
	// �S���ɉ��o�p�̃��b�Z�[�W����
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		// �S���ɑ��M
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			m_PersonData[i]->GetID(),
			PRODUCT_RIPPLE,
			MSG_NO_EXINFO
			);
	}

}

