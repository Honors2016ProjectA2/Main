#include "BaseGameEntity.h"
#include <cassert>

// 宣言
int BaseGameEntity::m_isNextValidID = 0;

void BaseGameEntity::SetID(int val)
{
	// ゲームIDはとても重要、したがってIDが被っていないか
	// この関数でテストする
	assert((val >= m_isNextValidID) && "SetID");
		
	m_ID = val;
	m_isNextValidID = m_ID + 1;	// インクリメント

}
