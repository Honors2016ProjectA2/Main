#include "AI\NPC\GoalPerson\GoalPerson.h"
#include "AI\NPC\GoalPerson\GoalPersonState.h"

GoalPerson::GoalPerson(ENTITY_ID id) :BasePerson(id)
{

	// キャラクターのモデル
	m_obj = new iex3DObj("DATA/Chara/dk.IEM");
	// 波紋の色// キャラクターによって波紋タイプ・人の種類を決める
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// 仮　newした先で決める

														 // ステートマシン
	m_PersonType = PERSON_TYPE::GOAL;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<GoalPerson>(this);
	m_pStateMachine->SetGlobalState(GoalPersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(GoalStart::GetInstance());
}

GoalPerson::~GoalPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void GoalPerson::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// ベースのUpdate
	BasePerson::Update();
}

void GoalPerson::Render()
{
	// このステートマシンの描画
	m_pStateMachine->Render();

	// ベースのRender
	BasePerson::Render();
}

// ステートマシンのメッセージ
bool GoalPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void GoalPerson::ResetState()
{
	//	一番最初に設定していたステートへ戻す
	m_pStateMachine->SetCurrentState(GoalStart::GetInstance());

}
