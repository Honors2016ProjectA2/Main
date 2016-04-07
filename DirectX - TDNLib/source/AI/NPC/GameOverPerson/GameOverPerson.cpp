
#include "AI\NPC\GameOverPerson\GameOverPerson.h"
#include "AI\NPC\GameOverPerson\GameOverPersonState.h"// カートリッジ

GameOverPerson::GameOverPerson(ENTITY_ID id) :BasePerson(id)
{

	// キャラクターのモデル
	m_obj = new iex3DObj("DATA/CHR/ハスボー/hasu_BX.IEM");
	// 波紋の色// キャラクターによって波紋タイプ・人の種類を決める
	m_Ripple = new GossipRipple(RIPPLE_TYPE::WHITE, m_pos);// 仮　newした先で決める

														 // ステートマシン
	m_PersonType = PERSON_TYPE::GAMEOVER;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<GameOverPerson>(this);
	m_pStateMachine->SetGlobalState(GameOverPersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(StartGameOver::GetInstance());
}

GameOverPerson::~GameOverPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void GameOverPerson::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// ベースのUpdate
	BasePerson::Update();
}

void GameOverPerson::Render()
{
	// このステートマシンの描画
	m_pStateMachine->Render();

	// ベースのRender
	BasePerson::Render();
}

// ステートマシンのメッセージ
bool GameOverPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void GameOverPerson::ResetState()
{
	//	一番最初に設定していたステートへ戻す
	m_pStateMachine->SetCurrentState(StartGameOver::GetInstance());

}
