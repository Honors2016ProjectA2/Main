#include "AI\NPC\BasePerson\BasePerson.h"
#include "AI\NPC\BasePerson\BasePersonState.h"// カートリッジ


// 初期化
BasePerson::BasePerson(ENTITY_ID id, PERSON_TYPE type) :BaseGameEntity(id)
{


	m_obj = NULL;


	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<BasePerson>(this);
	m_pStateMachine->SetGlobalState(PersonGlobalState::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(PersonWait::GetInstance());

	// キャラクターによって波紋タイプ・人の種類を決める
	switch (type)
	{
	case PERSON_TYPE::RED:	
		// キャラクターのモデル
		m_obj = new iex3DObj("DATA/CHR/airou_toire.IEM");
		// 波紋の色
		m_Ripple = new GossipRipple(RIPPLE_TYPE::RED, m_pos);

		//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）

		break;
	case PERSON_TYPE::BLUE:
		// キャラクターのモデル
		m_obj = new iex3DObj("DATA/CHR/nazenaraba_toire/nazenaraba_toire.IEM");
		// 波紋の色
		m_Ripple = new GossipRipple(RIPPLE_TYPE::BLUE, m_pos);

		//(TODO) ステートマシンの設定をここでする。（歩く人・立ち止まる人）

		break;
	default:
		MyAssert(0, "そんな人物は存在しない");
		break;
	}

	m_pos = VECTOR_ZERO;
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

	// 波紋
	m_Ripple->SetPos(m_pos);// 常にプレイヤー追従
	m_Ripple->Update();

	//if (m_Ripple->IsEnd() == true)
	//{

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
	m_obj->Render();
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
	m_Ripple->Action();
}


