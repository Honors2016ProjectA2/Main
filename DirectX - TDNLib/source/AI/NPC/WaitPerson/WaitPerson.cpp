
#include "AI\NPC\WaitPerson\WaitPerson.h"
#include "AI\NPC\WaitPerson\WaitPersonState.h"// カートリッジ

WaitPerson::WaitPerson(ENTITY_ID id) :BasePerson(id)
{

	// キャラクターのモデル
	m_obj = new iex3DObj("DATA/Chara/jk.IEM");
	// 波紋の色// キャラクターによって波紋タイプ・人の種類を決める
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// 仮　newした先で決める
	
	// ステートマシン
	m_PersonType = PERSON_TYPE::WAIT;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<WaitPerson>(this);
	m_pStateMachine->SetGlobalState(WaitPersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(StartWait::GetInstance());
}

WaitPerson::~WaitPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void WaitPerson::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// ベースのUpdate
	BasePerson::Update();
}

void WaitPerson::Render()
{
	// このステートマシンの描画
	m_pStateMachine->Render();

	// ベースのRender
	BasePerson::Render();
}

// ステートマシンのメッセージ
bool WaitPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}