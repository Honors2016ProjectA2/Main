
#include "AI\NPC\StrongPerson\StrongPerson.h"
#include "AI\NPC\StrongPerson\StrongPersonState.h"// カートリッジ

StrongPerson::StrongPerson(ENTITY_ID id) :BasePerson(id)
{

	// キャラクターのモデル
	m_obj = new iex3DObj("DATA/CHR/airou_toire.IEM");
	// 波紋の色// キャラクターによって波紋タイプ・人の種類を決める
	m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);// 仮　newした先で決める

														 // ステートマシン
	m_PersonType = PERSON_TYPE::STRONG;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<StrongPerson>(this);
	m_pStateMachine->SetGlobalState(StrongPersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(StartStrong::GetInstance());
}

StrongPerson::~StrongPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void StrongPerson::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// ベースのUpdate
	BasePerson::Update();
}

void StrongPerson::Render()
{
	// このステートマシンの描画
	m_pStateMachine->Render();

	// ベースのRender
	BasePerson::Render();
}

// ステートマシンのメッセージ
bool StrongPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void StrongPerson::ResetState()
{
	//	一番最初に設定していたステートへ戻す
	m_pStateMachine->SetCurrentState(StartStrong::GetInstance());

}
