
#include "AI\NPC\StopPerson\StopPerson.h"
#include "AI\NPC\StopPerson\StopPersonState.h"// カートリッジ

StopPerson::StopPerson(ENTITY_ID id) :BasePerson(id)
{

	// キャラクターのモデル
	m_obj = new iex3DObj("DATA/CHR/nazenaraba_toire/nazenaraba_toire.IEM");
	// 波紋の色// キャラクターによって波紋タイプ・人の種類を決める
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// 仮　newした先で決める

	m_RipSize = 30;
	// レンジの色
	m_RangeObj = new iexMesh("DATA/range/range.IMO");
	m_RangeObj->SetScale(m_RipSize);

														 // ステートマシン
	m_PersonType = PERSON_TYPE::STOP;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<StopPerson>(this);
	m_pStateMachine->SetGlobalState(StopPersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(StartStop::GetInstance());
}

StopPerson::~StopPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void StopPerson::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// ベースのUpdate
	BasePerson::Update();
}

void StopPerson::Render()
{
	// このステートマシンの描画
	m_pStateMachine->Render();

	// ベースのRender
	BasePerson::Render();
}

// ステートマシンのメッセージ
bool StopPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void StopPerson::ResetState()
{
	//	一番最初に設定していたステートへ戻す
	m_pStateMachine->SetCurrentState(StartStop::GetInstance());

}
