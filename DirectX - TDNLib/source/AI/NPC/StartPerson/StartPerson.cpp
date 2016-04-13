
#include "AI\NPC\StartPerson\StartPerson.h"
#include "AI\NPC\StartPerson\StartPersonState.h"

StartPerson::StartPerson(ENTITY_ID id) :BasePerson(id)
{

	// �L�����N�^�[�̃��f��
	m_obj = new iex3DObj("DATA/Chara/jk.IEM");
	// �g��̐F// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// ���@new������Ō��߂�
	


	m_RipSize = 30;
	// �����W�̐F
	m_RangeObj = new iexMesh("DATA/Range/range.IMO");
	m_RangeObj->SetScale(m_RipSize);

	//m_RangeObj->SetScale()

	// �X�e�[�g�}�V��
	m_PersonType = PERSON_TYPE::START;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<StartPerson>(this);
	m_pStateMachine->SetGlobalState(StartPersonGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(FirstStart::GetInstance());
}

StartPerson::~StartPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void StartPerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �x�[�X��Update
	BasePerson::Update();
}

void StartPerson::Render()
{
	// ���̃X�e�[�g�}�V���̕`��
	m_pStateMachine->Render();

	// �x�[�X��Render
	BasePerson::Render();
}

// �X�e�[�g�}�V���̃��b�Z�[�W
bool StartPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void StartPerson::ResetState()
{
	//	��ԍŏ��ɐݒ肵�Ă����X�e�[�g�֖߂�
	m_pStateMachine->SetCurrentState(FirstStart::GetInstance());

}
