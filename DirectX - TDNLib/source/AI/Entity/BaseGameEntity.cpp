#include "BaseGameEntity.h"
#include <cassert>

// �錾
int BaseGameEntity::m_isNextValidID = 0;

void BaseGameEntity::SetID(int val)
{
	// �Q�[��ID�͂ƂĂ��d�v�A����������ID������Ă��Ȃ���
	// ���̊֐��Ńe�X�g����
	assert((val >= m_isNextValidID) && "SetID");
		
	m_ID = val;
	m_isNextValidID = m_ID + 1;	// �C���N�������g

}
