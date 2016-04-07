
#include "AI\NPC\GameOverPerson\GameOverPerson.h"
#include "AI\NPC\GameOverPerson\GameOverPersonState.h"// �J�[�g���b�W

GameOverPerson::GameOverPerson(ENTITY_ID id) :BasePerson(id)
{

	// �L�����N�^�[�̃��f��
	m_obj = new iex3DObj("DATA/CHR/�n�X�{�[/hasu_BX.IEM");
	// �g��̐F// �L�����N�^�[�ɂ���Ĕg��^�C�v�E�l�̎�ނ����߂�
	m_Ripple = new GossipRipple(RIPPLE_TYPE::WHITE, m_pos);// ���@new������Ō��߂�

														 // �X�e�[�g�}�V��
	m_PersonType = PERSON_TYPE::GAMEOVER;

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<GameOverPerson>(this);
	m_pStateMachine->SetGlobalState(GameOverPersonGlobalState::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(StartGameOver::GetInstance());
}

GameOverPerson::~GameOverPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void GameOverPerson::Update()
{
	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	// �x�[�X��Update
	BasePerson::Update();
}

void GameOverPerson::Render()
{
	// ���̃X�e�[�g�}�V���̕`��
	m_pStateMachine->Render();

	// �x�[�X��Render
	BasePerson::Render();
}

// �X�e�[�g�}�V���̃��b�Z�[�W
bool GameOverPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void GameOverPerson::ResetState()
{
	//	��ԍŏ��ɐݒ肵�Ă����X�e�[�g�֖߂�
	m_pStateMachine->SetCurrentState(StartGameOver::GetInstance());

}
