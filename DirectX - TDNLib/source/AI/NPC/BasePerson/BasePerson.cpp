#include "AI\NPC\BasePerson\BasePerson.h"



// 初期化
BasePerson::BasePerson(int id) :BaseGameEntity(id)
{

	m_obj = NULL;
	m_pos = VECTOR_ZERO;

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<BasePerson>(this);

}

// 解放
BasePerson::~BasePerson()
{

	delete m_pStateMachine;

}

void BasePerson::Update()
{
}

bool BasePerson::HandleMessage(const Message & msg)
{
	return false;
}


