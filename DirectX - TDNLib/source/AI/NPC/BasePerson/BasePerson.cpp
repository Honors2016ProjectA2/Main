#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\BasePerson\BasePersonState.h"// カートリッジ

#include "system\System.h"


// 初期化
BasePerson::BasePerson(ENTITY_ID id) :BaseGameEntity(id)
{


	m_obj = NULL;

	// ステートマシン　引数は自分自身のポインタ
	//m_pStateMachine = new StateMachine<BasePerson>(this);
	//m_pStateMachine->SetGlobalState(PersonGlobalState::GetInstance());// グローバル
	//m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

	// キャラクターによって波紋タイプ・人の種類を決める
	// 仮　newした先で決める
	//m_PersonType = PERSON_TYPE::RED;

	//switch (m_PersonType)
	//{
	//case PERSON_TYPE::RED:	
	//	// キャラクターのモデル
	//	m_obj = new iex3DObj("DATA/Chara/jk.IEM");
	//	// 波紋の色
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

	//	//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
	//	m_pStateMachine->SetCurrentState(PersonWait::GetInstance());


	//	break;
	//case PERSON_TYPE::BLUE:
	//	// キャラクターのモデル
	//	m_obj = new iex3DObj("DATA/Chara/dk.IEM");
	//	// 波紋の色
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

	//	//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
	//	m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

	//	break;
	//case PERSON_TYPE::RED_RUN:
	//	// キャラクターのモデル
	//	m_obj = new iex3DObj("DATA/Chara/jk.IEM");

	//	// 波紋の色
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

	//	//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
	//	m_pStateMachine->SetCurrentState(PersonRun::GetInstance());

	//	break;
	//case PERSON_TYPE::BLUE_RUN:
	//	// キャラクターのモデル
	//	m_obj = new iex3DObj("DATA/Chara/dk.IEM");

	//	// 波紋の色
	//	m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

	//	//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
	//	m_pStateMachine->SetCurrentState(PersonRun::GetInstance());
	//	break;

	//default:
	//	MyAssert(0, "そんな人物は存在しない");
	//	break;
	//}

	m_pos = VECTOR_ZERO;
	m_move = VECTOR_ZERO;

	m_angle = PI;	// 0.0→PI



	
	m_isShed = false;	// 噂を流したか
}


// 解放
BasePerson::~BasePerson()
{

	SAFE_DELETE(m_Ripple);
	//SAFE_DELETE(m_pStateMachine);	
	SAFE_DELETE(m_obj);

}

void BasePerson::Update()
{
	// ★ステートマシン更新
	//m_pStateMachine->Update();

	// Move更新
	m_pos += m_move;

	// 摩擦
	if (m_move.z > 0.0f)
	{
		m_move.z -= 0.1f;
		if (m_move.z < 0.1f)m_move.z = 0.0f;
	}
	if (m_move.z < 0.0f)
	{
		m_move.z += 0.1f;
		if (m_move.z > 0.1f)m_move.z = 0.0f;
	}
	if (m_move.x > 0.0f)
	{
		m_move.x -= 0.1f;
		if (m_move.x < 0.1f)m_move.x = 0.0f;
	}
	if (m_move.x < 0.0f)
	{
		m_move.x += 0.1f;
		if (m_move.x > 0.1f)m_move.x = 0.0f;
	}

	// 場所更新
	m_obj->SetPos(m_pos);
	m_obj->SetAngle(m_angle);
	m_obj->Update();
	m_obj->Animation();
}

void BasePerson::Render()
{
	m_Ripple->Render();	// 波紋描画
	//m_obj->Render();
	//m_obj->Render(shader, "copy");
	//m_pStateMachine->Render();
}

// ステートマシンのメッセージ
//bool BasePerson::HandleMessage(const Message & msg)
//{
//	return m_pStateMachine->HandleMessage(msg);
//}

void BasePerson::ActionGossipRipple()
{
	// 流したフラグＯＮ
	m_isShed = true;
	//m_Ripple->Action();
	
	// 流し中のステートへ
	//m_pStateMachine->ChangeState(PersonShedWait::GetInstance());
}

//bool BasePerson::IsShedNow()
//{
//	// うわさを流してる途中
//	return (m_pStateMachine->isInState((*PersonShedRun::GetInstance())) || m_pStateMachine->isInState((*PersonShedWait::GetInstance())));
//}