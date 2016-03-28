
#include "AI\NPC\PersonManager.h"

//�@�ЂƂ̎��
#include "AI\NPC\BasePerson\BasePerson.h"



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
	BaseGameEntity(ENTITY_ID::PERSON_MNG)// ���l�̃}�l�[�W���[�p�̂h�c�ԍ���n��
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

	data = new BasePerson(id, type);
	data->SetPos(pos);

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
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
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
		//RIPPLE_INFO exInfo = (RIPPLE_INFO&)msg.ExtraInfo;
		//float exInfo = (float&)msg.ExtraInfo;

		// �����łu�r�֐����N���@to be contted 

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
void PersonManager::RippleVSPerson(int no)// ���g��
{

	// �Ώ�
	for (int b = 0; b < (int)m_PersonData.size(); b++)
	{
		if (no == b)continue;				// �܂��������͔̂������Ȃ�
		if (m_PersonData[b]->IsShed() == true)continue;// �\�𗧂Ă���͔������Ȃ�

		float ren =
			Math::Length(m_PersonData[no]->GetPos(), m_PersonData[b]->GetPos());
		if (ren <= 30)// 30m�ȓ��ɐl�����݂����
		{
			// �߂��ɂ����l�͔g����΂�
			//m_PersonData[b]->ActionGossipRipple();

			// �߂��ɂ����l�ɔg��̃��b�Z�[�W�𑗂�
		}

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

