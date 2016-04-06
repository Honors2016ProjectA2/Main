
#include "AI\NPC\StrongPerson\StrongPerson.h"
#include "AI\NPC\StrongPerson\StrongPersonState.h"// �J�[�g���b�W

StrongPerson::StrongPerson(ENTITY_ID id) :BasePerson(id)
{

	// �L�����N�^�[�̃��f��
	m_obj = new iex3DObj("DATA/CHR/airou_toire.IEM");
	// �g��̐F// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);// ���@new������Ō��߂�

														 // �X�e�[�g�}�V��
	m_PersonType = PERSON_TYPE::STRONG;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<StrongPerson>(this);
	m_pStateMachine->SetGlobalState(StrongPersonGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(StartStrong::GetInstance());
}

StrongPerson::~StrongPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void StrongPerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �x�[�X��Update
	BasePerson::Update();
}

void StrongPerson::Render()
{
	// ���̃X�e�[�g�}�V���̕`��
	m_pStateMachine->Render();

	// �x�[�X��Render
	BasePerson::Render();
}

// �X�e�[�g�}�V���̃��b�Z�[�W
bool StrongPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void StrongPerson::ResetState()
{
	//	��ԍŏ��ɐݒ肵�Ă����X�e�[�g�֖߂�
	m_pStateMachine->SetCurrentState(StartStrong::GetInstance());

}
