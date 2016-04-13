
#include "AI\NPC\StartPerson\StartPerson.h"
#include "AI\NPC\StartPerson\StartPersonState.h"

StartPerson::StartPerson(ENTITY_ID id) :BasePerson(id)
{

	// キャラクターのモデル
	m_obj = new iex3DObj("DATA/Chara/jk.IEM");
	// 波紋の色// キャラクターによって波紋タイプ・人の種類を決める
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// 仮　newした先で決める
	


	m_RipSize = 30;
	// レンジの色
	m_RangeObj = new iexMesh("DATA/Range/range.IMO");
	m_RangeObj->SetScale(m_RipSize);

	//m_RangeObj->SetScale()

	// ステートマシン
	m_PersonType = PERSON_TYPE::START;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<StartPerson>(this);
	m_pStateMachine->SetGlobalState(StartPersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(FirstStart::GetInstance());
}

StartPerson::~StartPerson()
{
	SAFE_DELETE(m_pStateMachine);
}

void StartPerson::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// ベースのUpdate
	BasePerson::Update();
}

void StartPerson::Render()
{
	// このステートマシンの描画
	m_pStateMachine->Render();

	// ベースのRender
	BasePerson::Render();
}

// ステートマシンのメッセージ
bool StartPerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void StartPerson::ResetState()
{
	//	一番最初に設定していたステートへ戻す
	m_pStateMachine->SetCurrentState(FirstStart::GetInstance());

}
