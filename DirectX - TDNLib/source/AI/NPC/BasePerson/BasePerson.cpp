#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\BasePerson\BasePersonState.h"// �J�[�g���b�W

#include "system\System.h"


// ������
BasePerson::BasePerson(ENTITY_ID id) :BaseGameEntity(id)
{


	m_obj = NULL;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	//m_pStateMachine = new StateMachine<BasePerson>(this);
	//m_pStateMachine->SetGlobalState(PersonGlobalState::GetInstance());// �O���[�o��
	//m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

	// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	// ���@new������Ō��߂�
	//m_PersonType = PERSON_TYPE::RED;

	//switch (m_PersonType)
	//{
	//case PERSON_TYPE::RED:	
	//	// �L�����N�^�[�̃��f��
	//	m_obj = new iex3DObj("DATA/Chara/jk.IEM");
	//	// �g��̐F
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

	//	//(TODO) �X�e�[�g�}�V���̐ݒ�������ł���B�i�����l�E�����~�܂�l�j
	//	m_pStateMachine->SetCurrentState(PersonWait::GetInstance());


	//	break;
	//case PERSON_TYPE::BLUE:
	//	// �L�����N�^�[�̃��f��
	//	m_obj = new iex3DObj("DATA/Chara/dk.IEM");
	//	// �g��̐F
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

	//	//(TODO) �X�e�[�g�}�V���̐ݒ�������ł���B�i�����l�E�����~�܂�l�j
	//	m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

	//	break;
	//case PERSON_TYPE::RED_RUN:
	//	// �L�����N�^�[�̃��f��
	//	m_obj = new iex3DObj("DATA/Chara/jk.IEM");

	//	// �g��̐F
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

	//	//(TODO) �X�e�[�g�}�V���̐ݒ�������ł���B�i�����l�E�����~�܂�l�j
	//	m_pStateMachine->SetCurrentState(PersonRun::GetInstance());

	//	break;
	//case PERSON_TYPE::BLUE_RUN:
	//	// �L�����N�^�[�̃��f��
	//	m_obj = new iex3DObj("DATA/Chara/dk.IEM");

	//	// �g��̐F
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

	//	//(TODO) �X�e�[�g�}�V���̐ݒ�������ł���B�i�����l�E�����~�܂�l�j
	//	m_pStateMachine->SetCurrentState(PersonRun::GetInstance());
	//	break;

	//default:
	//	MyAssert(0, "����Ȑl���͑��݂��Ȃ�");
	//	break;
	//}

	m_pos = VECTOR_ZERO;
	m_move = VECTOR_ZERO;

	m_angle = PI;	// 0.0��PI



	
	m_isShed = false;	// �\�𗬂�����
}


// ���
BasePerson::~BasePerson()
{

	SAFE_DELETE(m_Ripple);
	//SAFE_DELETE(m_pStateMachine);	
	SAFE_DELETE(m_obj);

}

void BasePerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	//m_pStateMachine->Update();

	// Move�X�V
	m_pos += m_move;

	// ���C
	if (m_move.z > 0.0f)
	{
		m_move.z -= 0.1f;
		if (m_move.z < 0.1f)m_move.z = 0.0f;
	}
	if (m_move.z < 0.0f)
	{
		m_move.z += 0.1f;
		if (m_move.z > 0.1f)m_move.z = 0.0f;
	}
	if (m_move.x > 0.0f)
	{
		m_move.x -= 0.1f;
		if (m_move.x < 0.1f)m_move.x = 0.0f;
	}
	if (m_move.x < 0.0f)
	{
		m_move.x += 0.1f;
		if (m_move.x > 0.1f)m_move.x = 0.0f;
	}

	// �ꏊ�X�V
	m_obj->SetPos(m_pos);
	m_obj->SetAngle(m_angle);
	m_obj->Update();
	m_obj->Animation();
}

void BasePerson::Render()
{
	m_Ripple->Render();	// �g��`��
	//m_obj->Render();
	//m_obj->Render(shader, "copy");
	//m_pStateMachine->Render();
}

// �X�e�[�g�}�V���̃��b�Z�[�W
//bool BasePerson::HandleMessage(const Message & msg)
//{
//	return m_pStateMachine->HandleMessage(msg);
//}

void BasePerson::ActionGossipRipple()
{
	// �������t���O�n�m
	m_isShed = true;
	//m_Ripple->Action();
	
	// �������̃X�e�[�g��
	//m_pStateMachine->ChangeState(PersonShedWait::GetInstance());
}

//bool BasePerson::IsShedNow()
//{
//	// ���킳�𗬂��Ă�r��
//	return (m_pStateMachine->isInState((*PersonShedRun::GetInstance())) || m_pStateMachine->isInState((*PersonShedWait::GetInstance())));
//}