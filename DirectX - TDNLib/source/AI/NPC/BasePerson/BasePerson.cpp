#include "AI\NPC\BasePerson\BasePerson.h"



// ������
BasePerson::BasePerson(int id) :BaseGameEntity(id)
{

	m_obj = NULL;
	m_pos = VECTOR_ZERO;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<BasePerson>(this);

}

// ���
BasePerson::~BasePerson()
{

	delete m_pStateMachine;

}

void BasePerson::Update()
{
}

bool BasePerson::HandleMessage(const Message & msg)
{
	return false;
}


