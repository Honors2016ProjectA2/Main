#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\BasePerson\BasePersonState.h"// カートリッジ

#include "system\System.h"


// 初期化
BasePerson::BasePerson(ENTITY_ID id, PERSON_TYPE type) :BaseGameEntity(id)
{


	m_obj = NULL;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<BasePerson>(this);
	m_pStateMachine->SetGlobalState(PersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

	// キャラクターによって波紋タイプ・人の種類を決める
	m_PersonType = type;
	switch (m_PersonType)
	{
	case PERSON_TYPE::RED:	
		// キャラクターのモデル
		m_obj = new iex3DObj("DATA/CHR/airou_toire.IEM");
		// 波紋の色
		m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

		//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
		m_pStateMachine->SetCurrentState(PersonWait::GetInstance());


		break;
	case PERSON_TYPE::BLUE:
		// キャラクターのモデル
		m_obj = new iex3DObj("DATA/CHR/nazenaraba_toire/nazenaraba_toire.IEM");
		// 波紋の色
		m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

		//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
		m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

		break;
	case PERSON_TYPE::RED_RUN:
		// キャラクターのモデル
		m_obj = new iex3DObj("DATA/CHR/airou_toire.IEM");

		// 波紋の色
		m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

		//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
		m_pStateMachine->SetCurrentState(PersonRun::GetInstance());

		break;
	case PERSON_TYPE::BLUE_RUN:
		// キャラクターのモデル
		m_obj = new iex3DObj("DATA/CHR/nazenaraba_toire/nazenaraba_toire.IEM");

		// 波紋の色
		m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

		//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）
		m_pStateMachine->SetCurrentState(PersonRun::GetInstance());
		break;

	default:
		MyAssert(0, "そんな人物は存在しない");
		break;
	}

	m_pos = VECTOR_ZERO;
	m_move = VECTOR_ZERO;

	m_angle = 0.0f;



	
	m_isShed = false;	// 噂を流したか
}


// 解放
BasePerson::~BasePerson()
{

	SAFE_DELETE(m_Ripple);
	SAFE_DELETE(m_pStateMachine);	
	SAFE_DELETE(m_obj);

}

void BasePerson::Update()
{
	// ★ステートマシン更新
	m_pStateMachine->Update();

	// Move更新
	m_pos += m_move;

	//if (m_move.x >= 0.3f)
	//{
	//	m_move.x -= 0.1f;
	//}
	//if (m_move.x <= 0.3f)
	//{
	//	m_move.x += 0.1f;
	//}
	//if (m_move.z >= 0.3f)
	//{
	//	m_move.z -= 0.1f;
	//}
	//if (m_move.z >= 0.3f)
	//{
	//	m_move.z += 0.1f;
	//}

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
	m_obj->Render(shader, "copy");
}

// ステートマシンのメッセージ
bool BasePerson::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void BasePerson::ActionGossipRipple()
{
	// 流したフラグＯＮ
	m_isShed = true;
	//m_Ripple->Action();
	
	// 流し中のステートへ
	//m_pStateMachine->ChangeState(PersonShedWait::GetInstance());
}


