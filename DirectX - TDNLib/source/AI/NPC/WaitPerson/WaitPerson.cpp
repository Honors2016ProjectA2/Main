
#include "AI\NPC\WaitPerson\WaitPerson.h"
#include "AI\NPC\WaitPerson\WaitPersonState.h"// �J�[�g���b�W

WaitPerson::WaitPerson(ENTITY_ID id) :BasePerson(id)
{

	// �L�����N�^�[�̃��f��
	m_obj = new iex3DObj("DATA/Chara/jk.IEM");
	// �g��̐F// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// ���@new������Ō��߂�
	
	// �X�e�[�g�}�V��
	m_PersonType = PERSON_TYPE::WAIT;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<WaitPerson>(this);
	m_pStateMachine->SetGlobalState(WaitPersonGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(StartWait::GetInstance());
}

WaitPerson::~WaitPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void WaitPerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �x�[�X��Update
	BasePerson::Update();
}

void WaitPerson::Render()
{
	// ���̃X�e�[�g�}�V���̕`��
	m_pStateMachine->Render();

	// �x�[�X��Render
	BasePerson::Render();
}

// �X�e�[�g�}�V���̃��b�Z�[�W
bool WaitPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}