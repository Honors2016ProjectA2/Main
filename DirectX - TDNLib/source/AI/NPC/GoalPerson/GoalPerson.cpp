#include "AI\NPC\GoalPerson\GoalPerson.h"
#include "AI\NPC\GoalPerson\GoalPersonState.h"

GoalPerson::GoalPerson(ENTITY_ID id) :BasePerson(id)
{

	// キャラクターのモデル
	m_obj = new iex3DObj("DATA/Chara/dk.IEM");
	// 波紋の色// キャラクターによって波紋タイプ・人の種類を決める
	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);// 仮　newした先で決める

	
	m_goalPic = new tdn2DObj("Data/UI/goal.png");


	m_RipSize = 30;
	// レンジの色
	m_RangeObj = new iexMesh("DATA/Range/range.IMO");
	m_RangeObj->SetScale(m_RipSize);

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
	SAFE_DELETE(m_goalPic);
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

void GoalPerson::UIRender()
{
	m_goalPic->SetScale(12.0f);
	m_goalPic->Render3D(m_pos + Vector3(0, 30, 0), RS::COPY_NOZ);
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
