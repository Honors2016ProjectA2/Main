
#include "AI\NPC\StopPerson\StopPerson.h"
#include "AI\NPC\StopPerson\StopPersonState.h"// �J�[�g���b�W

StopPerson::StopPerson(ENTITY_ID id) :BasePerson(id)
{

	// �L�����N�^�[�̃��f��
	m_obj = new iex3DObj("DATA/CHR/nazenaraba_toire/nazenaraba_toire.IEM");
	// �g��̐F// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// ���@new������Ō��߂�

	m_RipSize = 30;
	// �����W�̐F
	m_RangeObj = new iexMesh("DATA/range/range.IMO");
	m_RangeObj->SetScale(m_RipSize);

														 // �X�e�[�g�}�V��
	m_PersonType = PERSON_TYPE::STOP;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<StopPerson>(this);
	m_pStateMachine->SetGlobalState(StopPersonGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(StartStop::GetInstance());
}

StopPerson::~StopPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void StopPerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �x�[�X��Update
	BasePerson::Update();
}

void StopPerson::Render()
{
	// ���̃X�e�[�g�}�V���̕`��
	m_pStateMachine->Render();

	// �x�[�X��Render
	BasePerson::Render();
}

// �X�e�[�g�}�V���̃��b�Z�[�W
bool StopPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void StopPerson::ResetState()
{
	//	��ԍŏ��ɐݒ肵�Ă����X�e�[�g�֖߂�
	m_pStateMachine->SetCurrentState(StartStop::GetInstance());

}
