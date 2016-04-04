
#include "AI\NPC\PersonManager.h"

//�@�ЂƂ̎��
#include "AI\NPC\BasePerson\BasePerson.h"
#include "../../UI/UI.h"
#include "../../Sound/SoundManager.h"

#include "WaitPerson\WaitPerson.h"


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

// �g�� vs �l
// �����蔻��
void PersonManager::RippleVSPerson(RIPPLE_INFO* pRipData)// ���g��
{
	// �Ώ�
	int count(0);
	int count2(0);

	// ���l�������Ă��邩
	for (int b = 0; b < (int)m_PersonData.size(); b++)
	{
		if (m_PersonData[b]->IsShed() == true)continue;// �\�𗧂Ă���͔������Ȃ�
		if (m_PersonData[b]->GetPersonType() == pRipData->type) continue;// �����^�C�v�̓o�C�o�C

		float ren = Math::Length(pRipData->pos, m_PersonData[b]->GetPos());
		if (ren <= pRipData->size)// 30m�ȓ��ɐl�����݂����
		{
			count2++;// (��)
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
				if (count2 >= 2)
				{
					// �߂��ɂ����l�͔g����΂�
					m_PersonData[b]->ActionGossipRipple();

					// �������̐����o��
					UIMgr.PushHukidashi(m_PersonData[b]->GetPos(), HUKIDASHI_TYPE::SUCCESS);

					// �R���{���𑝂₷(2�l�J�E���g������A2�g�[���グ���܂��̂ł��̑Ώ�)
					if (count == 0) se->SetTone("�g��o��", ++m_combo);

					count++;
				}
				else
				{
					// ���s���̐����o��
					UIMgr.PushHukidashi(m_PersonData[b]->GetPos(), HUKIDASHI_TYPE::FAILED);
				}
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

