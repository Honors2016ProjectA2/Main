#include "AI\NPC\GoalPerson\GoalPerson.h"
#include "AI\NPC\GoalPerson\GoalPersonState.h"

GoalPerson::GoalPerson(ENTITY_ID id) :BasePerson(id)
{

	// �L�����N�^�[�̃��f��
	m_obj = new iex3DObj("DATA/Chara/dk.IEM");
	// �g��̐F// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// ���@new������Ō��߂�

	
	m_goalPic = new tdn2DObj("Data/UI/goal.png");


	m_RipSize = 30;
	// �����W�̐F
	m_RangeObj = new iexMesh("DATA/Range/range.IMO");
	m_RangeObj->SetScale(m_RipSize);

														 // �X�e�[�g�}�V��
	m_PersonType = PERSON_TYPE::GOAL;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<GoalPerson>(this);
	m_pStateMachine->SetGlobalState(GoalPersonGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(GoalStart::GetInstance());
}

GoalPerson::~GoalPerson()
{
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_goalPic);
}

void GoalPerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �x�[�X��Update
	BasePerson::Update();
}

void GoalPerson::Render()
{
	// ���̃X�e�[�g�}�V���̕`��
	m_pStateMachine->Render();

	// �x�[�X��Render
	BasePerson::Render();
}

void GoalPerson::UIRender()
{
	m_goalPic->SetScale(12.0f);
	m_goalPic->Render3D(m_pos + Vector3(0, 30, 0), RS::COPY_NOZ);
}

// �X�e�[�g�}�V���̃��b�Z�[�W
bool GoalPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void GoalPerson::ResetState()
{
	//	��ԍŏ��ɐݒ肵�Ă����X�e�[�g�֖߂�
	m_pStateMachine->SetCurrentState(GoalStart::GetInstance());

}
