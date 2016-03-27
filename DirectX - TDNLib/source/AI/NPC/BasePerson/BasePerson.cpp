#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\BasePerson\BasePersonState.h"// �J�[�g���b�W


// ������
BasePerson::BasePerson(ENTITY_ID id, PERSON_TYPE type) :BaseGameEntity(id)
{


	m_obj = NULL;


	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<BasePerson>(this);
	m_pStateMachine->SetGlobalState(PersonGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

	// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	switch (type)
	{
	case PERSON_TYPE::RED:	
		// �L�����N�^�[�̃��f��
		m_obj = new iex3DObj("DATA/CHR/airou_toire.IEM");
		// �g��̐F
		m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

		//(TODO) �X�e�[�g�}�V���̐ݒ�������ł���B�i�����l�E�����~�܂�l�j

		break;
	case PERSON_TYPE::BLUE:
		// �L�����N�^�[�̃��f��
		m_obj = new iex3DObj("DATA/CHR/nazenaraba_toire/nazenaraba_toire.IEM");
		// �g��̐F
		m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

		//(TODO) �X�e�[�g�}�V���̐ݒ�������ł���B�i�����l�E�����~�܂�l�j

		break;
	default:
		MyAssert(0, "����Ȑl���͑��݂��Ȃ�");
		break;
	}

	m_pos = VECTOR_ZERO;
	m_angle = 0.0f;



	
	m_isShed = false;	// �\�𗬂�����
}


// ���
BasePerson::~BasePerson()
{

	SAFE_DELETE(m_Ripple);
	SAFE_DELETE(m_pStateMachine);	
	SAFE_DELETE(m_obj);

}

void BasePerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �g��
	m_Ripple->SetPos(m_pos);// ��Ƀv���C���[�Ǐ]
	m_Ripple->Update();

	//if (m_Ripple->IsEnd() == true)
	//{

	//}

	// �ꏊ�X�V
	m_obj->SetPos(m_pos);
	m_obj->SetAngle(m_angle);
	m_obj->Update();
	m_obj->Animation();
}

void BasePerson::Render()
{
	m_Ripple->Render();	// �g��`��
	m_obj->Render();
}

// �X�e�[�g�}�V���̃��b�Z�[�W
bool BasePerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void BasePerson::ActionGossipRipple()
{
	// �������t���O�n�m
	m_isShed = true;
	m_Ripple->Action();
}


